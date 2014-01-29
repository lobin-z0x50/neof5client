/*
 * PoorPlayer.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#ifndef __PoorPlayer_hpp__
#define __PoorPlayer_hpp__

#include "IPlayer.hpp"


class PoorPlayer : public IPlayer {

public:
	virtual ~PoorPlayer(){};
	virtual CardSet think(const DaifugoContext& ctx) override;

};

#endif
