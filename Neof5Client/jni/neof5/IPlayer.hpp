/*
 * IPlayer.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#ifndef __IPlayer_hpp__
#define __IPlayer_hpp__

#include <functional>

#include "DaifugoContext.hpp"
#include "DaifugoRule.hpp"

class IPlayer {
protected:
	IDaifugoRule* _rule;
	std::function<void(const std::string&)> _tweetFunc;

public:
	virtual CardSet think(const DaifugoContext& ctx) =0;

	virtual void setRule(IDaifugoRule* rule) { _rule = rule; }
	
	virtual void setTweetFunc(std::function<void(const std::string&)> tweetFunc) { _tweetFunc = tweetFunc; }
};

#endif
