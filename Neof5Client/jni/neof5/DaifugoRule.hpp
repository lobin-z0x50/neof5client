/*
 * DaifugoRule.hpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */

#ifndef __DaiFugoRule_hpp__
#define __DaiFugoRule_hpp__

#include <vector>
#include "Cards.hpp"

typedef const char* chk_result_t;

namespace ChkResult {
	static const chk_result_t OK = NULL;
}

class IDaifugoRule {
protected:
	bool _isKakumei;

public:
	void setKakumei(bool isKakumei) { _isKakumei = isKakumei; }

	// 革命を考慮したカードの強弱判定
	virtual int compareCards(const Card& a, const Card& b) =0;

	// 場にカードが出せるかどうかを判定する
	virtual chk_result_t canPut(const CardSet& cards, const CardSet& ba_last) =0;

	// 上がれるかどうかを判定する
	virtual chk_result_t canAgari(const CardSet& cards) =0;
};


/**
 * クラスＡルール
 */
class DaifugoRuleA : public IDaifugoRule {
public:
	virtual int compareCards(const Card& a, const Card& b) override;
	virtual chk_result_t canPut(const CardSet& cards, const CardSet& ba_last) override;
	virtual chk_result_t canAgari(const CardSet& cards) override;

	static const chk_result_t ERR_DIFFERENT_NUM;
	static const chk_result_t ERR_COUNT_UNMATCH;
	static const chk_result_t ERR_INVALID_BA;
	static const chk_result_t ERR_NOT_STRONG;
};

/**
 * クラスＢルール
 */
class DaifugoRuleB : public DaifugoRuleA {
	typedef DaifugoRuleA base;
public:
	virtual chk_result_t canPut(const CardSet& cards, const CardSet& ba_last) override;
	virtual chk_result_t canAgari(const CardSet& cards) override;

	virtual bool isKaidan(const CardSet& cards);

	static const chk_result_t ERR_JOKER_AGARI;
	static const chk_result_t ERR_2_AGARI;
	static const chk_result_t ERR_NOT_KAIDAN;
	static const chk_result_t ERR_BA_ISNOT_KAIDAN;
};

#endif
