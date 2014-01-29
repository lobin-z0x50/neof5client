/*
 * WsThread.cpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>

#include "mydef.h"
#include "WsThread.hpp"

#define WS_SERVICE_TIMEOUT 500

#define WS_MESSAGE_SIZE_MAX 6000

static unsigned char buf[LWS_SEND_BUFFER_PRE_PADDING + WS_MESSAGE_SIZE_MAX + LWS_SEND_BUFFER_POST_PADDING];

static int
callback_func(struct libwebsocket_context *ctx,
			struct libwebsocket *wsi,
			enum libwebsocket_callback_reasons reason,
					       void *user, void *in, size_t len)
{
	WsThread* _this = (WsThread*)libwebsocket_context_user(ctx);
	//LOGD("user=%p", _this);

	switch (reason) {

	case LWS_CALLBACK_CLIENT_ESTABLISHED:
		LOGI("LWS_CALLBACK_CLIENT_ESTABLISHED");
		//_this->mHandler.onOpen();
		break;

	case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
		LOGE("LWS_CALLBACK_CLIENT_CONNECTION_ERROR");
		_this->was_closed = 1;
		_this->mHandler.onError();
		break;

	case LWS_CALLBACK_CLOSED:
		LOGI("LWS_CALLBACK_CLOSED");
		_this->was_closed = 1;
		_this->mHandler.onClose();
		break;

	case LWS_CALLBACK_CLIENT_RECEIVE:
		((char *)in)[len] = '\0';
		//LOGV("rx %d '%s'", (int)len, (char *)in);
		_this->mHandler.onMessage(in, len);
		break;

	case LWS_CALLBACK_CLIENT_WRITEABLE:
		while (!_this->sendQueue.empty()) {
			std::string text = _this->sendQueue.front();
			_this->sendQueue.pop();
		
			strncpy((char*)buf+LWS_SEND_BUFFER_PRE_PADDING, text.c_str(), WS_MESSAGE_SIZE_MAX);
			LOGI("LWS_CALLBACK_CLIENT_WRITEABLE text=%s len=%d", buf+LWS_SEND_BUFFER_PRE_PADDING, (int)text.length());
			int n = libwebsocket_write(wsi, buf+LWS_SEND_BUFFER_PRE_PADDING, text.length(), (libwebsocket_write_protocol)(LWS_WRITE_TEXT));
			LOGI("LWS_CALLBACK_CLIENT_WRITEABLE send ok. bytes=%d", n);

			if (n < 0)
				return -1;
			if (n < text.length()) {
				LOGE("Partial write LWS_CALLBACK_CLIENT_WRITEABLE\n");
				return -1;
			}
		}
		//	libwebsocket_callback_on_writable(context, wsi);
		break;

	case LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED:
		return 1;

	default:
		break;
	}

	return 0;
}


static struct libwebsocket_protocols protocols[] = {
	{
		"daif5",
		callback_func,
		WS_MESSAGE_SIZE_MAX,
		WS_MESSAGE_SIZE_MAX,
	},
	{ NULL, NULL, 0, 0 } /* end */
};

static int force_exit = 0;


void sighandler(int sig)
{
	force_exit = 1;
}

void WsThread::sendText(const std::string& txt, bool flush)
{
	sendQueue.push(txt);
	if (flush)
		libwebsocket_callback_on_writable(mContext, mSocket);
}

int WsThread::run(const char* host, int port, const char* path)
{
	int n = 0;
	int ret = 0;
	struct lws_context_creation_info info;

	lws_set_log_level(9, NULL);

	signal(SIGINT, sighandler);

	memset(&info, 0, sizeof info);

	info.port = CONTEXT_PORT_NO_LISTEN;
	info.protocols = protocols;
	info.gid = -1;
	info.uid = -1;
	info.user = this;
	info.extensions = libwebsocket_get_internal_extensions();
	//LOGD("this=%p", this);

	mContext = libwebsocket_create_context(&info);
	
	if (mContext == NULL) {
		LOGE("Creating libwebsocket context failed");
		return 1;
	}
	LOGI("Connecting to ws://%s:%d%s", host, port, path);

	n = 0;
	while (n >= 0 && !was_closed && !force_exit) {
		n = libwebsocket_service(mContext, WS_SERVICE_TIMEOUT);

		//LOGV("n=%d", n);
		if (n < 0) continue;

		if (mSocket) continue;

		mSocket = libwebsocket_client_connect(mContext,
			host, port, 0, path,
			host, "origin",
			NULL, -1);

		if (mSocket == NULL) {
			LOGE("Connect failed");
			ret = -1;
			goto bail;
		}
	}
bail:
	LOGI("Exiting");
	return ret;
}

WsThread::WsThread(IWsHandler& handler) : mHandler(handler) {
}

WsThread::~WsThread() {
	if (mContext){
		libwebsocket_context_destroy(mContext);
	}
}

