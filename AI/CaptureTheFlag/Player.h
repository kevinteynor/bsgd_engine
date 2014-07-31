#ifndef PLAYER_H
#define PLAYER_H

#include <AStarPath.h>
#include <DecisionTree.h>
#include <EngineTypeDefs.h>
#include <FastDelegate.h>
#include <glm\glm.hpp>
#include <NativeLevel.h>

using namespace DecisionTree;

class CaptureTheFlag;

class Player
{
private:
	float decisionTreeTimeout;
	float decisionTreeCooldown;

	float newPathCooldown;

#pragma region DECISION TREE

	void InitializeDecisionTree();

	#pragma region heirarchy
	// States:
	//	- MovingTowardsEnemyFlag
	//	- CapturingEnemyFlag
	//	- RetrievingOwnFlag
	//	- GuardingOwnFlag
	
	// -- Offensive --
	// Decisions:
	//	- (0) Does our team have their flag?
	//		- Y: (1) Do I have their flag?
	//			- Y: (STATE) CapturingEnemyFlag
	//			- N: -- goto to node 3 --
	//		- N: (2) Am I the closest one to their flag?
	//			- Y: (STATE) MovingTowardsEnemyFlag
	//			- (3) N: Is our flag taken?
	//				- Y: (STATE) RetrievingOwnFlag
	//				- N: (STATE) GuardingOwnFlag

	// -- Deffensive --
	// Decisions:
	//	- (0) Is our flag taken?
	//		- Y: (STATE) RetrievingOwnFlag
	//		- N: (1) Does our team have their flag?
	//			- Y: (2) Do I have their flag?
	//				- Y: (STATE) CapturingEnemyFlag
	//				- N: (STATE) GuardingOwnFlag
	//			- N: (3) Am I the closest one to their flag?
	//				- Y: (STATE) MovingTowardsEnemyFlag
	//				- N: (STATE) GuardingOwnFlag
	#pragma endregion

	DecisionNode* NodeTree;
	DecisionNode* NodeTree_Defensive;

	bool EnemyFlagTaken();
	bool PlayerHasEnemyFlag();
	bool PlayerClosestToEnemyFlag();
	bool TeamFlagTaken();
	bool PlayerClosestToEnemyFlagCarrier();

	State CapturingEnemyFlag;
	State MovingTowardsEnemyFlag;
	State RetrievingOwnFlag;
	State GuardingOwnFlag;

	void update_CapturingEnemyFlag();
	void update_MovingTowardsEnemyFlag();
	void update_RetrievingOwnFlag();
	void update_GuardingOwnFlag();

#pragma endregion

	void UpdatePosition();

public:
	bool defensive;

	CaptureTheFlag* game;
	bool needsNewPath;
	AStarPath path;
	uint currentNodeID;
	uint endTargetNodeID;

	static uint NEXT_PLAYER_ID;
	uint playerID;
	uint teamID;	
	uint shapeID;
	uint homeNodeID;
	float movementSpeed;
	glm::vec3 position;

	bool hasEnemyFlag;
	bool needsNewState;
	State* currentState;

	Player(CaptureTheFlag* g, uint teamid = 0, uint homenodeid = 0, float movespeed = 1.0f, glm::vec3 color = glm::vec3(1,0,0));
	~Player();
	void Update();

	float NodeHeuristic_Flee(uint nodeIndex, uint targetIndex);
	float NodeHeuristic_Attack(uint nodeIndex, uint targetIndex);
	fastdelegate::FastDelegate2<uint, uint, float> HeuristicDelegate_Flee;
	fastdelegate::FastDelegate2<uint, uint, float> HeuristicDelegate_Attack;
};

#endif //PLAYER_H