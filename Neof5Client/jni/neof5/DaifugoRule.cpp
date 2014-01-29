/*
 * DaifugoRule.cpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#include <string>
#include <memory>
#include <algorithm>

#include "mydef.h"
#include "DaifugoRule.hpp"

/*==============================================================================
 * クラスＡルール
 *============================================================================*/

const chk_result_t DaifugoRuleA::ERR_DIFFERENT_NUM = "数字が異なるカードが含まれています。";
const chk_result_t DaifugoRuleA::ERR_COUNT_UNMATCH = "枚数が一致していません。";
const chk_result_t DaifugoRuleA::ERR_INVALID_BA = "強いカードではありません。";
const chk_result_t DaifugoRuleA::ERR_NOT_STRONG = "場のカードが不正です。";

static bool _tryGetFirstCard(const CardSet& cards, std::unique_ptr<Card>& buff)
{
	switch (cards.size()) {
	case 0: return false;
	case 1: buff.reset(new Card(cards[0])); return true;
	}
	// ジョーカー以外に2種類以上の数字が含まれている場合はダメ
	Card* check = NULL;
	for (size_t i=0; i < cards.size(); i++) {
		Card c = cards[i];
		if (c.isJoker()) continue;
		if (check == NULL) {
			check = new Card(c);
			continue;
		}
		if (check->getNum() != c.getNum()) return false;
	}
	buff.reset(check);
	return true;
}

int DaifugoRuleA::compareCards(const Card& a, const Card& b)
{
	// 同じ数字かチェック
	if (a.getNum() == b.getNum()) return 0;

	// 革命に関係なくジョーカーが最強
	if (a.isJoker()) return -1;
	if (b.isJoker()) return 1;

	// 数字で比較
	int ret = a > b ? -1 : 1;
	// 革命時は判定逆転
	return _isKakumei ? -ret : ret;
}

chk_result_t DaifugoRuleA::canPut(const CardSet& cards, const CardSet& ba_last)
{
	// パス？
	if (cards.empty()) return ChkResult::OK;

	std::unique_ptr<Card> firstCard;
	if (!_tryGetFirstCard(cards, firstCard)) return ERR_DIFFERENT_NUM;

	// 場にカードが出ていない？
	if (ba_last.empty()) return ChkResult::OK;

	// 場と枚数が一致しているか？
	if (ba_last.size() != cards.size()) return ERR_COUNT_UNMATCH;

	std::unique_ptr<Card> baCard;
	if (!_tryGetFirstCard(ba_last, baCard)) return ERR_INVALID_BA;

	// 場より強いカードか？
	if (compareCards(*baCard, *firstCard) <= 0) return ERR_NOT_STRONG;
	return ChkResult::OK;
}

chk_result_t DaifugoRuleA::canAgari(const  CardSet& cards) { return ChkResult::OK; }


/*==============================================================================
 * クラスＢルール
 *============================================================================*/

const chk_result_t DaifugoRuleB::ERR_JOKER_AGARI = "ジョーカーあがりは禁止です。";
const chk_result_t DaifugoRuleB::ERR_2_AGARI = "２あがり(革命時は３あがり）禁止です。";
const chk_result_t DaifugoRuleB::ERR_BA_ISNOT_KAIDAN = "場が階段でない。";
const chk_result_t DaifugoRuleB::ERR_NOT_KAIDAN = "カードが階段でない。";

static bool _isKaidan(const CardSet& cards, std::unique_ptr<std::vector<Card>>& listcards)
{
	if (cards.size() < 3) return false;

	// ジョーカーを取り除く
	CardSet tmpc = cards;
	int hasJoker = tmpc.extractJoker();
	listcards.reset(new std::vector<Card>());
	tmpc.conv2Vector(*listcards);

	// 同じスートか？
	auto first = listcards->at(0);
	for (int i=1; i < listcards->size(); i++) {
		if (first.getSuit() != listcards->at(i).getSuit()) return false;
	}

	// 階段順になっているか？
	int pj = 0;
	std::sort(listcards->begin(), listcards->end());
	for (int i=0; i < listcards->size()-1; i++) {
		int cmp = listcards->at(i+1).compareTo(listcards->at(i));
		cmp--;
		if (cmp==0) continue;
		if (cmp <= (hasJoker-pj)) { pj+=cmp; continue; } // ジョーカーを持っている場合はジョーカーを消費
		return false;
	}
	return true;
}

bool DaifugoRuleB::isKaidan(const CardSet& cards)
{
	std::unique_ptr<std::vector<Card>> v_cards;
	return _isKaidan(cards, v_cards);
}

chk_result_t DaifugoRuleB::canPut(const CardSet& cards, const CardSet& ba_last)
{
	auto ret = base::canPut(cards, ba_last);
	if (ret == ChkResult::OK) return ret;

	std::unique_ptr<std::vector<Card>> v_cards;
	std::unique_ptr<std::vector<Card>> v_balast;
	if (!ba_last.empty()) {
		auto balast = ba_last;
		if (_isKaidan(balast, v_balast)) {
			if (_isKaidan(cards, v_cards)) {
				// 強さを比較
				auto c1 = std::max_element(v_balast->begin(), v_balast->end());
				auto c2 = std::min_element(v_cards->begin(), v_cards->end());
				if (compareCards(*c2, *c1) > 0) return ChkResult::OK;
				return ERR_NOT_STRONG;
			} else {
				return ERR_NOT_KAIDAN;
			}
		} else {
			if (_isKaidan(cards, v_cards)) return ERR_BA_ISNOT_KAIDAN;
		}
	} else {
		if (_isKaidan(cards, v_cards)) return ChkResult::OK;
	}
	return ret;
}

chk_result_t DaifugoRuleB::canAgari(const CardSet& cards)
{
	CardSet tmpc = cards;
	int jokers = tmpc.extractJoker();

	// ジョーカーあがり？
	if (tmpc.empty()) return ERR_JOKER_AGARI;

	// ２あがり？
	if (tmpc.hasNumOf(_isKakumei ? 3 : 2)) return ERR_2_AGARI;
	return ChkResult::OK;
}
