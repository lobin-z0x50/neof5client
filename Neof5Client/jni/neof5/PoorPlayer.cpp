/*
 * PoorPlayer.cpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#include <string>
#include <vector>
#include <algorithm>

#include "mydef.h"
#include "PoorPlayer.hpp"

#define LOG_HEAD "PoorPlayer::think() "

CardSet PoorPlayer::think(const DaifugoContext& ctx)
{
	LOGV(LOG_HEAD "tefuda=[%s]", ctx.getDeck().toString().c_str());

	auto tefuda = ctx.getDeck().conv2Vector();
	std::sort(tefuda.begin(), tefuda.end());
	
	LOGV(LOG_HEAD "IsKakumei = %d", ctx.isKakumei());
	if (ctx.isKakumei()) std::reverse(tefuda.begin(), tefuda.end());

	int n=1;
	if (!ctx.getBa().empty()) {
		auto lastba = ctx.getBa().back();
		n = lastba.size();
		LOGV(LOG_HEAD "Ba=[%s] n=%d", lastba.toString().c_str(), n);
	}

	// 弱いカードから順番に出す
	for (int i=0; i < tefuda.size() - (n - 1); i++) {
		CardSet cards;
		for (int j=i; j < i+n; j++) {
			cards += tefuda[j];
		}
	
		LOGV(LOG_HEAD "trying [%s]", cards.toString().c_str());
		
		_rule->setKakumei(ctx.isKakumei());
		CardSet ba_last;
		if (!ctx.getBa().empty()) ba_last = ctx.getBa().back();
		auto chk_result = _rule->canPut(cards, ba_last);
		if (chk_result == ChkResult::OK) return cards;
	}

	LOGV(LOG_HEAD "No more available hand ! There is no choise but to Pass !");
	
	return CardSet();
}

