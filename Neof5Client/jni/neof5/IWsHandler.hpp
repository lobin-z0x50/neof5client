/*
 * IWsHandler.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#ifndef __WS_HANDLER_H__
#define __WS_HANDLER_H__

#include <functional>

class IWsHandler {
protected:
	std::function<void(const std::string&, bool)> _sendFunc;

public:
	virtual void setSendFunc(std::function<void(const std::string&, bool)> sendFunc) { _sendFunc = sendFunc; }

	virtual void onOpen(void) =0;
	virtual void onClose(void) =0;
	virtual void onError(void) =0;
	virtual void onMessage(void* data, int len) =0;
};
#endif
