#ifndef DECISION_TREE_H
#define DECISION_TREE_H

#include "EngineTypeDefs.h"
#include "FastDelegate.h"
#include "FastDelegateBind.h"

namespace DecisionTree { struct State; }
typedef fastdelegate::FastDelegate0<bool> CheckConditionDelegate;
typedef fastdelegate::FastDelegate0<DecisionTree::State*> EvaluateNodeDelegate;

namespace DecisionTree
{
	struct State
	{
		Delegate Update;
	};
	
	struct DecisionNode
	{
		DecisionNode* trueNode;
		DecisionNode* falseNode;
		State* trueState;
		State* falseState;
		CheckConditionDelegate CheckCondition;
		EvaluateNodeDelegate Evaluate;

		DecisionNode() : trueNode(0), falseNode(0), trueState(0), falseState(0) {}
		~DecisionNode()
		{
			trueNode = 0;
			falseNode = 0;
			trueState = 0;
			falseState = 0;
		}

		void Initialize(DecisionNode* tn, DecisionNode* fn, CheckConditionDelegate ccdn)
		{
			trueNode = tn;
			falseNode = fn;

			trueState = 0;
			falseState = 0;
			CheckCondition = ccdn;
			
			Evaluate = fastdelegate::MakeDelegate(this, &DecisionNode::evalNodeNode);
		}
		void Initialize(State* ts, DecisionNode* fn, CheckConditionDelegate ccdn)
		{
			trueNode = 0;
			falseNode = fn;
			trueState = ts;
			falseState = 0;
			CheckCondition = ccdn;
			
			Evaluate = fastdelegate::MakeDelegate(this, &DecisionNode::evalStateNode);
		}
		void Initialize(DecisionNode* tn, State* fs, CheckConditionDelegate ccdn)
		{
			trueNode = tn;
			falseNode = 0;
			trueState = 0;
			falseState = fs;
			CheckCondition = ccdn;
			
			Evaluate = fastdelegate::MakeDelegate(this, &DecisionNode::evalNodeState);
		}
		void Initialize(State* ts, State* fs, CheckConditionDelegate ccdn)
		{
			trueNode = 0;
			falseNode = 0;
			trueState = ts;
			falseState = fs;
			CheckCondition = ccdn;
			
			Evaluate = fastdelegate::MakeDelegate(this, &DecisionNode::evalStateState);
		}
		

	private:
		State* evalNodeNode()
		{
			return CheckCondition() ? trueNode->Evaluate() : falseNode->Evaluate();
		}
		State* evalStateNode()
		{
			return CheckCondition() ? trueState : falseNode->Evaluate();
		}
		State* evalNodeState()
		{
			return CheckCondition() ? trueNode->Evaluate() : falseState;
		}
		State* evalStateState()
		{
			return CheckCondition() ? trueState : falseState;
		}
	};
}

#endif // DECISION_TREE_H