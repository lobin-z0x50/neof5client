/*
 * Neof5Protocol.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#ifndef __NEOF5PROTOCOL_H__
#define __NEOF5PROTOCOL_H__

#include "IWsHandler.hpp"
#include "IPlayer.hpp"

class Neof5Protocol : public IWsHandler {
	IPlayer* _player;

public:
	void setPlayer(IPlayer* player) { _player = player; }

	void onOpen(void) override;
	void onClose(void) override;
	void onError(void) override;
	void onMessage(void* data, int len) override;

	void putCard(const CardSet& cards);
	void tweet(const std::string& s);
};
#endif
