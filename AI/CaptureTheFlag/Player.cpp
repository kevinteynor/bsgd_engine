#include "Player.h"
#include <EngineTypeDefs.h>
#include <DebugShapes.h>
#include <FastDelegate.h>
#include <FastDelegateBind.h>
#include <float.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <UpdateClock.h>

#include "CaptureTheFlag.h"

uint Player::NEXT_PLAYER_ID = 0;

Player::Player(CaptureTheFlag* g, uint teamid, uint homenodeid, float movespeed, glm::vec3 color)
{
	game = g;
	playerID = NEXT_PLAYER_ID;
	teamID = teamid;
	homeNodeID = homenodeid;
	currentNodeID = homeNodeID;
	position = game->level.nodes[currentNodeID].position;
	shapeID = DebugShapes::MakeArrow(glm::translate(g->level.nodes[homeNodeID].position) * glm::scale(0.45f, 0.45f, 0.45f), -1.0f, true, color);
	movementSpeed = movespeed;
	hasEnemyFlag = false;
	NEXT_PLAYER_ID++;

	endTargetNodeID = 1;
	InitializeDecisionTree();
}
Player::~Player()
{
	game = 0;
	delete [] NodeTree;
	NodeTree = 0;
}

void Player::InitializeDecisionTree()
{
	decisionTreeCooldown = 0.0f;
	decisionTreeTimeout = 0.1f;
	newPathCooldown = 0.0f;
	needsNewPath = true;

	// set the update functions for each state
	CapturingEnemyFlag.Update = fastdelegate::MakeDelegate(this, &Player::update_CapturingEnemyFlag);
	MovingTowardsEnemyFlag.Update = fastdelegate::MakeDelegate(this, &Player::update_MovingTowardsEnemyFlag);
	RetrievingOwnFlag.Update = fastdelegate::MakeDelegate(this, &Player::update_RetrievingOwnFlag);
	GuardingOwnFlag.Update = fastdelegate::MakeDelegate(this, &Player::update_GuardingOwnFlag);

	// build the tree
	NodeTree = new DecisionNode[4];
	NodeTree[0].Initialize(&NodeTree[1], &NodeTree[2], fastdelegate::MakeDelegate(this, &Player::EnemyFlagTaken));
	NodeTree[1].Initialize(&CapturingEnemyFlag, &NodeTree[3], fastdelegate::MakeDelegate(this, &Player::PlayerHasEnemyFlag));
	NodeTree[2].Initialize(&MovingTowardsEnemyFlag, &NodeTree[3], fastdelegate::MakeDelegate(this, &Player::PlayerClosestToEnemyFlag));
	NodeTree[3].Initialize(&RetrievingOwnFlag, &GuardingOwnFlag, fastdelegate::MakeDelegate(this, &Player::TeamFlagTaken));

	NodeTree_Defensive = new DecisionNode[4];
	NodeTree_Defensive[0].Initialize(&RetrievingOwnFlag, &NodeTree_Defensive[1], fastdelegate::MakeDelegate(this, &Player::TeamFlagTaken));
	NodeTree_Defensive[1].Initialize(&NodeTree_Defensive[2], &NodeTree_Defensive[3], fastdelegate::MakeDelegate(this, &Player::EnemyFlagTaken));
	NodeTree_Defensive[2].Initialize(&CapturingEnemyFlag, &GuardingOwnFlag, fastdelegate::MakeDelegate(this, &Player::PlayerHasEnemyFlag));
	NodeTree_Defensive[3].Initialize(&MovingTowardsEnemyFlag, &GuardingOwnFlag, fastdelegate::MakeDelegate(this, &Player::PlayerClosestToEnemyFlag));

	HeuristicDelegate_Flee = fastdelegate::MakeDelegate(this, &Player::NodeHeuristic_Flee);
	HeuristicDelegate_Attack = fastdelegate::MakeDelegate(this, &Player::NodeHeuristic_Attack);
}

void Player::Update()
{
	float dt = UpdateClock::DeltaTime();
	decisionTreeCooldown -= dt;

	if(decisionTreeCooldown <= 0 || needsNewState)
	{
		// run decision tree		
		currentState = defensive ? NodeTree_Defensive->Evaluate() : NodeTree->Evaluate();
		decisionTreeCooldown = decisionTreeTimeout;
		needsNewState = false;
	}

	currentState->Update();
	UpdatePosition();

	if(hasEnemyFlag)
	{
		for(uint i = 0; i < game->NUM_PLAYERS; i++)
		{
			if(game->players[i]->teamID != teamID)
			{
				if(glm::length(game->players[i]->position - position) < 0.75f)
				{
					game->ResetFlag(teamID == 0 ? 1 : 0);
					position = game->level.nodes[game->TeamFlags[teamID].homeNodeID].position;
					needsNewPath = true;
					needsNewState = true;
					hasEnemyFlag = false;
				}
			}
		}
	}
}
void Player::UpdatePosition()
{
	if(needsNewState) return;

	glm::vec3 direction = glm::normalize(game->level.nodes[path.nodeIndices[path.currentIndex]].position - position);
	if(direction.x * 0 != 0)
		direction = glm::vec3(0,0,0);

#pragma region FOLLOW PATH
	if(glm::length(game->level.nodes[path.nodeIndices[path.currentIndex]].position - position) <= (UpdateClock::DeltaTime() * movementSpeed))
	{
		uint currentNodeID = path.nodeIndices[path.currentIndex];
		path.currentIndex++;

		if(path.currentIndex >= path.indexCount)
		{
			path = 0;
			needsNewPath = true;
		}
	}
#pragma endregion

#pragma region POSITIONING + ROTATION
	glm::vec3 original = glm::vec3(0,0,-1);		// arrow's default orientation
	glm::vec3 newOrientation = direction;		// the player's new orientation

	glm::vec3 newPosition = position + (direction * UpdateClock::DeltaTime() * movementSpeed);

	if(currentState == &GuardingOwnFlag) newPosition -= (direction * UpdateClock::DeltaTime() * movementSpeed * 0.8f);

	glm::mat4 p_mat = glm::translate(newPosition);
	glm::mat4 r_mat = glm::mat4();

	float yaw = acos(glm::dot(original, glm::normalize(glm::vec3(newOrientation.x, 0, newOrientation.z)))) * 180 / PI;
	float pitch = 0.0f;

	if(yaw * 0 != 0)
	{
		yaw = 0;
		pitch = acos(glm::dot(original, glm::normalize(newOrientation))) * 180 / PI;
	}
	else
	{
		if(newOrientation.x < 0)
			yaw = -yaw;
		glm::vec3 pitchVector = glm::vec3(glm::vec4(original, 0) * glm::rotate(yaw, glm::vec3(0,1,0)));
		pitch = acos(glm::dot(pitchVector, glm::normalize(newOrientation))) * 180 / PI;
	}
	if(pitch * 0 != 0)
	{
		pitch = 0;
	}
	else if(newOrientation.y > 0)
	{
		pitch = -pitch;
	}

	r_mat = glm::rotate(pitch, 1.0f, 0.0f, 0.0f) * glm::rotate(-yaw, 0.0f, 1.0f, 0.0f);

	position = newPosition;
	DebugShapes::SetShapeTransform(shapeID, p_mat * r_mat);

#pragma endregion
}

float Player::NodeHeuristic_Flee(uint nodeIndex, uint targetIndex)
{
	float h_enemyPlayers = 0;
	for(uint i = 0; i < game->NUM_PLAYERS; i++)
	{
		if(game->players[i]->teamID != teamID)
		{
			float l = glm::length(game->players[i]->position - game->level.nodes[nodeIndex].position);
			if(l != 0)
				h_enemyPlayers += 1.0f / l * 50;
			else h_enemyPlayers += 500;
		}
	}

	float h_distFromTarget = glm::length(game->level.nodes[nodeIndex].position - game->level.nodes[targetIndex].position);

	return h_enemyPlayers + h_distFromTarget;
}
float Player::NodeHeuristic_Attack(uint nodeIndex, uint targetIndex)
{
	float h_enemyPlayers = 0;
	for(uint i = 0; i < game->NUM_PLAYERS; i++)
	{
		if(game->players[i]->teamID != teamID)
		{
			if(game->players[i]->hasEnemyFlag)
			{
				h_enemyPlayers += glm::length(game->players[i]->position - game->level.nodes[nodeIndex].position);
			}
		}
	}

	float h_distFromTarget = glm::length(game->level.nodes[nodeIndex].position - game->level.nodes[targetIndex].position);

	return h_enemyPlayers + h_distFromTarget;
}

bool Player::EnemyFlagTaken()
{
	for(uint i = 0; i < game->NUM_PLAYERS; i++)
	{
		if(game->players[i]->hasEnemyFlag && game->players[i]->teamID == teamID)
			return true;
	}
	return false;
}
bool Player::PlayerHasEnemyFlag()
{
	return hasEnemyFlag;
}
bool Player::PlayerClosestToEnemyFlag()
{
	float shortest_dist = FLT_MAX;
	uint closestPlayerID = (uint)-1;

	for(uint i = 0; i < game->NUM_PLAYERS; i++)
	{
		if(game->players[i]->teamID != teamID) continue;

		float d = glm::length(game->players[i]->position - game->TeamFlags[teamID == 0 ? 1 : 0].position);
		if(d < shortest_dist)
		{
			closestPlayerID = game->players[i]->playerID;
			shortest_dist = d;
		}
	}

	return playerID == closestPlayerID;
}
bool Player::TeamFlagTaken()
{
	for(uint i = 0; i < game->NUM_PLAYERS; i++)
	{
		if(game->players[i]->teamID != teamID && game->players[i]->hasEnemyFlag)
			return true;
	}
	return false;
}
bool Player::PlayerClosestToEnemyFlagCarrier()
{
	uint enemyFlagCarrierIndex = (uint)-1;
	for(uint i = 0; i < game->NUM_PLAYERS; i++)
	{
		if(game->players[i]->teamID != teamID && game->players[i]->hasEnemyFlag)
		{
			enemyFlagCarrierIndex = i;
			break;
		}
	}
	if(enemyFlagCarrierIndex == (uint)-1) return false;	// no flag carrier on the enemy team.

	float shortest_dist = FLT_MAX;
	uint closestPlayerID = (uint)-1;
	for(uint i = 0; i < game->NUM_PLAYERS; i++)
	{
		if(game->players[i]->teamID != teamID) continue;
		
		float d = glm::length(game->players[i]->position - game->players[enemyFlagCarrierIndex]->position);
		if(d < shortest_dist)
		{
			closestPlayerID = game->players[i]->playerID;
			shortest_dist = d;
		}
	}

	return playerID == closestPlayerID;
}

void Player::update_CapturingEnemyFlag()
{
	// taking enemy flag back to base
	
	if(glm::length(position - game->level.nodes[homeNodeID].position) < 1)
	{
		game->CaptureFlag(this);

		hasEnemyFlag = false;
		game->TeamFlags[teamID == 0 ? 1 : 0].carrier = 0;
		needsNewState = true;
		return;
	}

	if(newPathCooldown <= 0 || needsNewPath)
	{
		if(path.indexCount != 0 && !needsNewPath)
			path = AStarPath::FindPath(game->level.nodes, game->level.numNodes, game->level.nodes[path.nodeIndices[path.currentIndex]].position, game->level.nodes[homeNodeID].position, HeuristicDelegate_Flee);
		else path = AStarPath::FindPath(game->level.nodes, game->level.numNodes, position, game->level.nodes[homeNodeID].position, HeuristicDelegate_Flee);
		currentNodeID = *path.nodeIndices;
		needsNewPath = false;
		newPathCooldown = 0.03f;
	}
	else newPathCooldown -= UpdateClock::DeltaTime();
}
void Player::update_MovingTowardsEnemyFlag()
{
	glm::vec3 flagPos = game->TeamFlags[teamID == 0 ? 1 : 0].position;

	if(glm::length(position - flagPos) < 1)
	{
		// pick up flag
		hasEnemyFlag = true;
		game->TeamFlags[teamID == 0 ? 1 : 0].carrier = this;
		needsNewState = true;
		return;
	}

	if(newPathCooldown <= 0 || needsNewPath)
	{
		if(path.indexCount != 0 && !needsNewPath)
			path = AStarPath::FindPath(game->level.nodes, game->level.numNodes, game->level.nodes[path.nodeIndices[path.currentIndex]].position, game->TeamFlags[teamID == 0 ? 1 : 0].position, HeuristicDelegate_Flee);
		else path = AStarPath::FindPath(game->level.nodes, game->level.numNodes, position, flagPos, HeuristicDelegate_Flee);
		currentNodeID = *path.nodeIndices;
		needsNewPath = false;
		newPathCooldown = 0.15f;
	}
	else newPathCooldown -= UpdateClock::DeltaTime();
}
void Player::update_RetrievingOwnFlag()
{
	if(game->TeamFlags[teamID].carrier == 0)
	{
		needsNewState = true;
		return;
	}

	Player* carrier = game->TeamFlags[teamID].carrier;
	glm::vec3 carrierPos = carrier->position;
	
	if(carrier->path.currentIndex < carrier->path.indexCount - 1 && carrier->path.indexCount != 0)
	{
		// get a path to where the carrier is going, not where he is.
		carrierPos = game->level.nodes[carrier->path.nodeIndices[carrier->path.currentIndex + 1]].position;
		if(glm::length(carrierPos - position) > glm::length(game->level.nodes[carrier->path.nodeIndices[carrier->path.currentIndex]].position - position))
			carrierPos = game->level.nodes[carrier->path.nodeIndices[carrier->path.currentIndex]].position;
	}

	if(newPathCooldown <= 0 || needsNewPath)
	{
		if(path.indexCount != 0 && !needsNewPath)
			path = AStarPath::FindPath(game->level.nodes, game->level.numNodes, game->level.nodes[path.nodeIndices[path.currentIndex]].position, carrierPos, HeuristicDelegate_Attack);
		else path = AStarPath::FindPath(game->level.nodes, game->level.numNodes, position, carrierPos);
		currentNodeID = *path.nodeIndices;
		needsNewPath = false;
		newPathCooldown = 0.03f;
	}
	else newPathCooldown -= UpdateClock::DeltaTime();
}
void Player::update_GuardingOwnFlag()
{
	// path back and forth between random connections and the home node

	// if it's at the last node in its path, it needs a new path.
	if(path.indexCount <= 0) needsNewPath = true;
	else if (glm::length(position - game->level.nodes[path.nodeIndices[path.indexCount - 1]].position) < 1) needsNewPath = true;

	if(needsNewPath)
	{
		if(glm::length(position - game->level.nodes[game->TeamFlags[teamID].homeNodeID].position) < 1)
		{
			// it's at the home node, go to a connection
			uint connectionID = (std::rand() % game->level.nodes[game->TeamFlags[teamID].homeNodeID].numConnections);
			path = AStarPath::FindPath(game->level.nodes, game->TeamFlags[teamID].homeNodeID, game->level.nodes[game->TeamFlags[teamID].homeNodeID].connections[connectionID].connectorID);
		}
		else
		{
			// it's at a connection, go to the home node.
			path = AStarPath::FindPath(game->level.nodes, game->level.numNodes, position, game->TeamFlags[teamID].position);
		}

		currentNodeID = *path.nodeIndices;
		needsNewPath = false;
		newPathCooldown = 0.05f;
	}
	else newPathCooldown -= UpdateClock::DeltaTime();
}