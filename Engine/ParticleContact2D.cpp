#include <cfloat>

#include "ParticleContact2D.h"
#include "UpdateClock.h"
#include "EngineTypeDefs.h"

#include <iostream>

ParticleContact2D::ParticleContact2D(TPhysics::Particle2D* a, TPhysics::Particle2D* b, float restitution)
	: p1(a), p2(b), restitution(restitution){}

float ParticleContact2D::calculateSeparatingVelocity() const
{
	glm::vec2 relativeVelocity = p1->Velocity;
	if(p2) relativeVelocity -= p2->Velocity;	
	return glm::dot(relativeVelocity, contactNormal);
}

void ParticleContact2D::Resolve()
{
	resolveVelocity();
	resolveInterpenetration();
}
void ParticleContact2D::resolveVelocity()
{
	if(p2 && contactNormal == glm::vec2(0,0))
		contactNormal = glm::normalize(p1->Position - p2->Position);
	
	float separatingVelocity = calculateSeparatingVelocity();
	if(separatingVelocity > 0) return;
	
	float velocityFromAcceleration = glm::dot(p1->Acceleration, contactNormal) * UpdateClock::DeltaTime();
	if(p2) velocityFromAcceleration += glm::dot(p2->Acceleration, contactNormal) * UpdateClock::DeltaTime();
	
	if(velocityFromAcceleration < 0)
	{
		// remove any closing velocity due to acceleration.
		separatingVelocity -= velocityFromAcceleration;
	}
	
	float totalInverseMass = p1->InverseMass;
	if(p2) totalInverseMass += p2->InverseMass;
	float totalImpulse = (-separatingVelocity) * (restitution + 1) / totalInverseMass;

	glm::vec2 directionalImpulse = totalImpulse * contactNormal;
	
	p1->AddImpulse(directionalImpulse);
	if(p2) p2->AddImpulse(-directionalImpulse);
	
}
void ParticleContact2D::resolveInterpenetration()
{
	if(penetration <= 0) return;

	if(p2 && contactNormal == glm::vec2(0,0))
		contactNormal = glm::normalize(p1->Position - p2->Position);

	float totalInverseMass = p1->InverseMass;
	if(p2) totalInverseMass += p2->InverseMass;
	
	glm::vec2 totalMovement = contactNormal * (penetration / totalInverseMass);

	particleMovement[0] = totalMovement * p1->InverseMass;
	if(p2) particleMovement[1] = totalMovement * -p2->InverseMass;
	else particleMovement[1] = glm::vec2(0,0);

	p1->Position += particleMovement[0];
	if(p2) p2->Position += particleMovement[1];
}

void ParticleContactResolver2D::SetIterations(uint i)
{
	iterations = i;
}
void ParticleContactResolver2D::ResloveContacts(ParticleContact2D * contactArray, uint numContacts)
{
	uint i;

	iterationsUsed = 0;
	while(iterationsUsed < iterations)
	{
		// find contact with largest closing velocity.
		#pragma warning( disable : 4003)
		float max = FLT_MAX;
		uint maxIndex = numContacts;
		for(i = 0; i < numContacts; i++)
		{
			float sepVel = contactArray[i].calculateSeparatingVelocity();
			if(sepVel < max && (sepVel < 0 || contactArray[i].penetration > 0))
			{
				max = sepVel;
				maxIndex = i;
			}
		}

		if(maxIndex == numContacts) break;

		contactArray[maxIndex].Resolve();

		glm::vec2* move = contactArray[maxIndex].particleMovement;
		for(i = 0; i < numContacts; i++)
		{
			if(contactArray[i].p1 == contactArray[maxIndex].p1)
				contactArray[i].penetration -= glm::dot(move[0], contactArray[i].contactNormal);
			else if(contactArray[i].p1 == contactArray[maxIndex].p2)
				contactArray[i].penetration -= glm::dot(move[1], contactArray[i].contactNormal);

			if(contactArray[i].p2 == contactArray[maxIndex].p1)
				contactArray[i].penetration += glm::dot(move[0], contactArray[i].contactNormal);
			else if(contactArray[i].p2 == contactArray[maxIndex].p2)
				contactArray[i].penetration += glm::dot(move[1], contactArray[i].contactNormal);

		}

		iterationsUsed++;
	}
}