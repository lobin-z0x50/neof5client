/*
 * PlayerInfo.cpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#include "mydef.h"
#include "PlayerInfo.hpp"

PlayerInfo::PlayerInfo(const std::string& name, int havingCardCount, int orderOfFinish)
	: _name(name), _havingCardCount(havingCardCount), _orderOfFinish(orderOfFinish)
{
}

