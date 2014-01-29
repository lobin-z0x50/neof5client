/*
 * PlayerInfo.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#ifndef __PlayerInfo_hpp__
#define __PlayerInfo_hpp__

#include <string>

class PlayerInfo {
	std::string _name;
	int _havingCardCount;
	int _orderOfFinish;

public:
	PlayerInfo(const std::string& name, int havingCardCount, int orderOfFinish);

	const std::string& getName() const { return _name; }
	int getHavingCardCount() const { return _havingCardCount; }
	int getOrderOfFinish() const { return _orderOfFinish; }
};

#endif
