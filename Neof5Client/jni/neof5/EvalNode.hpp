/*
 * EvalNode.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#ifndef __EvalNode_hpp__
#define __EvalNode_hpp__

#include <cstdint>
#include <vector>
#include <string>

#include "Cards.hpp"

struct EvalHand {
	CardSet hand;
	CardSet tefda; // 手札
	double p1;     // パラメータ1
	double p2;    // パラメータ2

	EvalHand(void) : hand(), tefda(), p1(0.0f), p2(0.0f) {}
};

class EvalNode {
protected:
	EvalNode* _parent;
	std::vector<EvalNode*>* _children;

public:
	EvalHand hand;
	bool isKakumei;

	// constructor
	EvalNode(const CardSet& tefda, bool isKakumei);
	virtual ~EvalNode();

	// ルートノードか？
	bool isRoot(void) const { return _parent==NULL; }

	// 親ノードの取得
	EvalNode* getParent(void) { return _parent; }

	// ルート直下の子ノードまで遡る
	EvalNode* getHeirOfRoot(void);

	// 子ノードの追加
	EvalNode* createChild(const EvalHand& eh);

	// 子ノードの数
	const int numOfChildren(void) const { return _children ? _children->size() : 0; }

	// 子ノードの取得（index指定）
	EvalNode* getChildAt(int idx) const { return _children->at(idx); }

	// 最も良いp1パラメータを持つ子ノードを探索する
	EvalNode* findBestParamChild(void);

	// ルートからこのノードまでの手をvectorに保存する
	void saveHandsTree(std::vector<CardSet>& buff);

	// このノードからルートノードまでの木構造を文字列化する
	std::string treeToString(void) const;
};

#endif

