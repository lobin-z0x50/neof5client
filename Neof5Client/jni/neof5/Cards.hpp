/*
 * Cards.hpp
 *
 * Copyright (C) 2012 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#ifndef __Card_Hpp__
#define __Card_Hpp__

#include "mydef.h"
#include <vector>
#include <string>

#define CARD_STRENGTH_MAX 14

/**
 * スートを表す列挙体
 */
enum Suit {
	SPD = 0x50,   // スペード
	HRT = 0x40,   // ハード
	DIA = 0x30,   // ダイヤ
	CLV = 0x20,   // クローバー
	JKR = 0x10    // ジョーカー
};

class CardSet;

/**
 * カードを表すクラス
 */
class Card {
	// フレンドクラス
	friend class CardSet;

	// フレンド関数
	friend int compare(const Card& a, const Card& b);

	uint8_t idx;
	Card(uint8_t id);

public:
	Card(const Card& other);

	Suit getSuit() const;
	int  getNum() const;
	bool isEfuda() const;
	bool isJoker() const;
	std::string toString() const;

	// カードの強さ（1〜14）
	int getStrength() const;

	uint8_t getValue() const;
	uint8_t getId() const;
	
	/**
	 * カードを取得する
	 */
	static Card get(Suit s, int n);
	static Card fromString(const char* str);

	/**
	 * 全カード枚数
	 */
	static size_t getAllCount();

	/**
	 * スートの文字列化（ログ出力やデバッグ用）
	 */
	static std::string suitToString(Suit s);

	/**
	 * カードの比較
	 */
	int compareTo(const Card& other) const;
	bool operator<(const Card& other) const;
	bool operator>(const Card& other) const;
	bool operator<=(const Card& other) const;
	bool operator>=(const Card& other) const;
	bool operator==(const Card& other) const;
	bool operator!=(const Card& other) const;
};

/**
 * カードの集合を表すクラス
 */
class CardSet {
	uint64_t bits;  // 所持しているカードをビットセットで表す

	CardSet(uint64_t b) : bits(b){};
	
public:
	CardSet() : bits(0l){};
	CardSet(const std::vector<Card>& source);
	CardSet(const char* str);

	static CardSet allCards();

	size_t size() const;

	bool empty() const { return bits == 0; }

	/**
	 * カードセットの差
	 */
	void operator-=(const CardSet&);

	/**
	 * カードセットの和
	 */
	void operator+=(const CardSet&);

	/**
	 * カードの削除
	 */
	void operator-=(const Card&);

	/**
	 * カードの追加
	 */
	void operator+=(const Card&);

	/**
	 * 指定スートの一括削除
	 */
//	void operator-=(const std::vector<Suit>&);

	/**
	 * カードの包含判定
	 */
	bool has(const Card& c) const;
	bool has(const CardSet& other) const {return (bits & other.bits) == other.bits;}
	bool hasNumOf(int num, CardSet* output=NULL) const;
	
	/**
	 * ジョーカーを取り除く
	 */
	int extractJoker(void);

	/**
	 * セットの比較
	 */
	bool operator<(const CardSet& other) const {return this->bits < other.bits;}
	bool operator>(const CardSet& other) const {return this->bits > other.bits;}
	bool operator==(const CardSet& other) const {return this->bits == other.bits;}

	/**
	 * セットの交わり・積
	 */
	bool communion(const CardSet& other) const {return this->bits & other.bits;}

	/**
	 * Cardオブジェクトの配列に変換する
	 */
	std::vector<Card> conv2Vector() const;
	void conv2Vector(std::vector<Card>& buff) const;

	/**
	 * 要素アクセス
	 * （非常に効率が悪いので、[0]などのアクセスのみに留めておき、多用する場合はvector化すること）
	 */
	Card at(int index) const;
	Card operator[](int index) const { return at(index); }
	
	/**
	 * カードのサブセット（組み合わせ）
	 */
	//std::vector<CardSet> getSubSets(int num) const;

	std::string toString() const;

private:
	//void getSubSets(uint64_t start, const uint64_t& bits, int num, const CardSet& work, std::vector<CardSet>* buff) const;
};

#endif

