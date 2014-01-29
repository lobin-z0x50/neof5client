/*
 * SagePlayer.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#ifndef __SagePlayer_hpp__
#define __SagePlayer_hpp__

#include "IPlayer.hpp"
#include "EvalNode.hpp"
#include <set>
#include <vector>

/*
class SageEvalResult : public EvalResult {
protected:
	virtual int32_t calcTefdaPoint(void);
public:
	SageEvalResult(void) : EvalResult(CardSet(), CardSet()){}
	SageEvalResult(const CardSet& hand, const CardSet& tefda, double pw=0.0) : EvalResult(hand, tefda, pw){}
};
*/

/**
 * 賢いプレイヤークラス
 */
class SagePlayer : public IPlayer {
	std::vector<CardSet> _savedHandsTree;

	void _enumrateAvairableHands(std::set<CardSet>& buff, const CardSet& tefda, CardSet ba_last, bool isKakumei);

	void _genAllCombination(std::set<CardSet>& buff, const CardSet baseHand, const CardSet& deck, CardSet& ba_last);

	double _calcPotentialToBeFightback(const DaifugoContext& ctx, const CardSet& hand, bool isKakumei);

	EvalNode* _think_recursive(long timelimit, const DaifugoContext& ctx, const CardSet& ba_cards, std::set<EvalNode*>& nodes);

	CardSet think2(const DaifugoContext& ctx, CardSet& ba_last);

	// 代入禁止
	void operator=(SagePlayer& other){}
	SagePlayer(SagePlayer& other){}

	// 手の評価値の最大ポイント（局面が進みに連れて低くなっていく）
	double hand_potential_max;
	
	// 明らかになっていないカード（＝他のプレーヤーが持っているカード）
	CardSet _notappears_cards;

public:
	SagePlayer(void);
	virtual ~SagePlayer() {}

	virtual CardSet think(const DaifugoContext& ctx) override;
};

#endif
