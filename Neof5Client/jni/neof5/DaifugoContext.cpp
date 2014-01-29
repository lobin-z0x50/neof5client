/*
 * DaifugoContext.cpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#include "mydef.h"
#include "DaifugoContext.hpp"
	
DaifugoContext::DaifugoContext(const std::string& deck, int teban, const std::vector<std::string>& ba, const std::string& yama, bool isKakumei, const std::vector<PlayerInfo>& playerInfo)
	: _deck(deck.c_str()), _teban(teban), _yama(yama.c_str()), _isKakumei(isKakumei), _playerInfo(playerInfo)
{
	for (auto& str : ba) {
		_ba.push_back(CardSet(str.c_str()));
	}
}

int DaifugoContext::getNumOfAvailablePlayers() const
{
	int ret = 0;
	for (auto& pi : _playerInfo) {
		if (pi.getHavingCardCount() > 0) ret++;
	}
	return ret;
}

