/*
 * DaifugoContext.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#ifndef __DaifugoContext_hpp__
#define __DaifugoContext_hpp__

#include <vector>
#include <string>
#include "Cards.hpp"
#include "PlayerInfo.hpp"


class DaifugoContext {
	CardSet _deck;
	int _teban;
	std::vector<CardSet> _ba;
	CardSet _yama;
	bool _isKakumei;
	std::vector<PlayerInfo> _playerInfo;

public:
	// constructor
	DaifugoContext(const std::string& deck, int teban, const std::vector<std::string>& ba, const std::string& yama, bool isKakumei, const std::vector<PlayerInfo>& playerInfo);

	// getter
	const CardSet& getDeck() const { return _deck; }
	int getTeban() const { return _teban; }
	const std::vector<CardSet>& getBa() const { return _ba; }
	const CardSet& getYama() const { return _yama; }
	bool isKakumei() const { return _isKakumei; }
	const std::vector<PlayerInfo>& getPlayerInfo() const { return _playerInfo; }

	int getNumOfAvailablePlayers() const;

};

#endif
