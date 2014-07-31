#ifndef PHYSICS_Particle2D_H
#define PHYSICS_Particle2D_H

#include <glm\glm.hpp>
#include "ExportHeader.h"

namespace TPhysics
{
	class ENGINE_SHARED Particle2D
	{
	private:
		glm::vec2 accumulatedForces;

	public:
		Particle2D();

		float Damping;
		float InverseMass;
		glm::vec2 Position;
		glm::vec2 Velocity;
		glm::vec2 Acceleration;

		void Initialize(glm::vec2 position);
		void Update();

		void AddForce(const glm::vec2& force);
		void AddImpulse(const glm::vec2& impulse);

		inline bool HasFiniteMass() { return InverseMass != 0.0f; }
		inline float Mass() { return 1.0f / InverseMass; }

	};
}

#endif // "PHYSICS_Particle2D_H"