/*
 * EvalNode.cpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#include "EvalNode.hpp"
#include "mydef.h"

EvalNode::EvalNode(const CardSet& tefda, bool isKakumei)
	: _children(NULL), _parent(NULL), hand(), isKakumei(isKakumei)
{
	hand.tefda = tefda;
}

EvalNode::~EvalNode()
{
	if (_children) {
		for (EvalNode* p : *_children) {
			delete p;
		}
		delete _children;
	}
}

EvalNode* EvalNode::getHeirOfRoot(void)
{
	EvalNode* result = this;
	if (!result->isRoot()){
		while (!result->getParent()->isRoot()) result = result->getParent();
	}
	return result;
}

EvalNode* EvalNode::createChild(const EvalHand& eh)
{
	// 同じ手のchildが既にないかチェックする
	if (_children) for (auto c : *_children) {
		if (c->hand.hand == eh.hand) return c;
	}

	CardSet dmy;

	EvalNode* child = new EvalNode(dmy, isKakumei);
	child->_parent = this;
	child->hand = eh;
	child->hand.tefda -= hand.hand;

	// カード4枚出たなら革命ステータスを反転
	if (child->hand.hand.size() >= 4) child->isKakumei = !child->isKakumei;

	// 木構造に追加
	if (_children == NULL) _children = new std::vector<EvalNode*>();
	_children->push_back(child);

	return child;
}

EvalNode* EvalNode::findBestParamChild(void)
{
	EvalNode* bestNode = NULL;
	if (_children) {
		for (auto child : *_children) {
			//LOGV("findBestParamChild() this=%p child=%p %s", this, child, child->treeToString().c_str());
			if (child->hand.p2 == 0.0f) {
				child = child->findBestParamChild();
				if (!child) continue;
			}
			if (!bestNode || child->hand.p2 < bestNode->hand.p2) { bestNode = child; }
		}
	} else {
		LOGD("findBestParamChild() nochild. this=%p %s", this, treeToString().c_str());
	}
	return bestNode;
}

void EvalNode::saveHandsTree(std::vector<CardSet>& buff)
{
	if (hand.hand.empty()) return;
	buff.push_back(hand.hand);
	getParent()->saveHandsTree(buff);
}

std::string EvalNode::treeToString() const
{
	char buff[64];
	sprintf(buff, "[%s](%1.2f_%1.2f%s) ", hand.hand.toString().c_str(), hand.p1, hand.p2, isKakumei?"K":"");
	if (_parent) {
		return _parent->treeToString() + std::string(buff);
	}
	return std::string(buff);
}

