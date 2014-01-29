/*
 * WsThread.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#ifndef __WS_THREAD_H__
#define __WS_THREAD_H__

#include <string>
#include <queue>

#include "IWsHandler.hpp"
#include "../libwebsockets-1.23/lib/libwebsockets.h"

class WsThread {
	struct libwebsocket_context *mContext = NULL;

	int deny_deflate=1;
	int deny_mux=1;
	struct libwebsocket *mSocket = NULL;


public:
	std::queue<std::string> sendQueue;

	int was_closed=0;
	IWsHandler& mHandler;

	WsThread(IWsHandler& handler);
	~WsThread();

	int run(const char* host, int port, const char* path);

	void sendText(const std::string& txt, bool flush);
};

#endif
