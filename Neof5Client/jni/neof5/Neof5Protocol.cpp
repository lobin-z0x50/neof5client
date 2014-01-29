/*
 * Neof5Protocol.cpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#include <string>

#include "mydef.h"
#include "Neof5Protocol.hpp"
#include "picojson.h"

#include "DaifugoContext.hpp"


using namespace std;

void Neof5Protocol::onOpen(void)
{
}

void Neof5Protocol::onClose(void)
{
}

void Neof5Protocol::onError(void)
{
}

void Neof5Protocol::onMessage(void* data, int len)
{
	const char* _data = (const char*)data;

	// parse JSON
	picojson::value json;
	picojson::parse(json, _data, _data+len);
	
	// get Kind
	auto& obj = json.get<picojson::object>();
	string kind = obj["Kind"].get<string>();
	LOGV("Kind:=%s", kind.c_str());

	if (kind == "ProcessTurn") {
		// get Deck
		auto deck = obj["Deck"].get<string>(); LOGV("Deck:=%s", deck.c_str());
		// get Yama
		auto yama = obj["Yama"].get<string>();
		// get IsKakumei
		auto isKakumei = obj["IsKakumei"].get<bool>();
		// get Ba
		auto& ba = obj["Ba"].get<picojson::array>();
		vector<string> v_ba;
		for (auto& ba_elem : ba) {
			auto cards = ba_elem.get<string>(); LOGV("Ba:=%s",cards.c_str());
			v_ba.push_back(cards);
		}
	/*
		// get History
		auto& history = obj["History"].get<picojson::array>();
		for (auto& history_elem : history) {
			auto hist = history_elem.get<string>(); // LOGV("History:=%s", hist.c_str());
		}
	*/
		// get Teban
		int teban = obj["Teban"].get<double>();
		// get PlayerInfo
		std::vector<PlayerInfo> v_pi;
		for (auto& pi_elem : obj["PlayerInfo"].get<picojson::array>()) {
			auto& objPi = pi_elem.get<picojson::object>();
			v_pi.push_back(PlayerInfo(objPi["Name"].get<string>(), objPi["HavingCardCount"].get<double>(), objPi["OrderOfFinish"].get<double>()));
		}

		// set Tweet Callback Function
		_player->setTweetFunc([&](const std::string& message) -> void { tweet(message); });

		// Create Context Object
		DaifugoContext ctx(deck, teban, v_ba, yama, isKakumei, v_pi);

		// Call Think Method
		CardSet hand = _player->think(ctx);

		// Return response
		string dat = hand.toString();
		putCard(hand);
	}
}

void Neof5Protocol::putCard(const CardSet& cards) {
	// create JSON
	picojson::object obj;

	obj.insert(make_pair("Kind", picojson::value(string("Put"))));
	obj.insert(make_pair("Cards", picojson::value(cards.toString())));
	
	picojson::value json(obj);
	_sendFunc(json.serialize(), true);
	LOGFLUSH;
}

void Neof5Protocol::tweet(const string& s) {
	// create JSON
	picojson::object obj;

	obj.insert(make_pair("Kind", picojson::value(string("Tweet"))));
	obj.insert(make_pair("Message", picojson::value(s)));
	
	picojson::value json(obj);
	_sendFunc(json.serialize(), false);
	LOGFLUSH;
}

