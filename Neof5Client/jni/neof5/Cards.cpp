/*
 * Cards.cpp
 *
 * Copyright (C) 2012 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#include "Cards.hpp"

static uint8_t _cards[] = {
	CLV | 3,
	DIA | 3,
	HRT | 3,
	SPD | 3,
	CLV | 4,
	DIA | 4,
	HRT | 4,
	SPD | 4,
	CLV | 5,
	DIA | 5,
	HRT | 5,
	SPD | 5,
	CLV | 6,
	DIA | 6,
	HRT | 6,
	SPD | 6,
	CLV | 7,
	DIA | 7,
	HRT | 7,
	SPD | 7,
	CLV | 8,
	DIA | 8,
	HRT | 8,
	SPD | 8,
	CLV | 9,
	DIA | 9,
	HRT | 9,
	SPD | 9,
	CLV | 10,
	DIA | 10,
	HRT | 10,
	SPD | 10,
	CLV | 11,
	DIA | 11,
	HRT | 11,
	SPD | 11,
	CLV | 12,
	DIA | 12,
	HRT | 12,
	SPD | 12,
	CLV | 13,
	DIA | 13,
	HRT | 13,
	SPD | 13,
	CLV | 1,  // A
	DIA | 1,  // A
	HRT | 1,  // A
	SPD | 1,  // A
	CLV | 2,
	DIA | 2,
	HRT | 2,
	SPD | 2,
	JKR  // ジョーカー
};

/**
 * 文字列からカードIDを計算する関数
 */
static int _calcCardIdFromString(const char* str)
{
	Suit s = JKR;
	switch (str[0]) {
	case 'C': s = CLV; break;
	case 'D': s = DIA; break;
	case 'H': s = HRT; break;
	case 'S': s = SPD; break;
	}

	int n = 0;
	switch (str[1]) {
	case '2': n =12; break;
	case 'A': n =11; break;
	case 'K': n =10; break;
	case 'Q': n =9; break;
	case 'J': n =8; break;
	case '0': n =7; break;
	default: n = str[1] - '3';
	}
	int id = s == JKR ? ARRAY_SIZEOF(_cards)-1 : n*4 + (s-0x20)/0x10;
	if (id >= ARRAY_SIZEOFI(_cards)) throw "カードエラー";
	return id;
}


// ========== class Card ==========

Card::Card(uint8_t id)
{
	if (id >= ARRAY_SIZEOFI(_cards)) throw "カードIDエラー";
	idx = id;
}

Card Card::fromString(const char* str)
{
	int xxidx = _calcCardIdFromString(str);
	return Card(xxidx);
}

std::string Card::toString() const
{
	std::string s(suitToString(getSuit()));

	switch (getNum()) {
	case 0 : s+='K'; break;
	case 1 : s+='A'; break;
	case 10 : s+='0'; break;
	case 11 : s+='J'; break;
	case 12 : s+='Q'; break;
	case 13 : s+='K'; break;
	default: s+='0' + getNum(); break;
	}
	return s;
}

Card::Card(const Card& other)
{
	idx = other.idx;
}

Suit Card::getSuit() const
{
	return (Suit)(_cards[idx] & 0xf0);
}

int  Card::getNum() const
{
	return _cards[idx] & 0x0f;
}
/*
bool Card::isEfuda() const
{
	return (_cards[idx] & 0x0f) >= 10 || (_cards[idx] & 0x0f) == 1;
}
*/
bool Card::isJoker() const
{
	return (_cards[idx] & 0xf0) == JKR;
}

uint8_t Card::getValue() const
{
	return _cards[idx];
}

uint8_t Card::getId() const
{
	return idx;
}

int Card::getStrength() const
{
	return isJoker() ? CARD_STRENGTH_MAX : 1 + (getNum()+10) % 13;
}

Card Card::get(Suit s, int n)
{
//	return (Card&)(s == JKR ? _cards[ARRAY_SIZEOF(_cards)-1] : _cards[(s-0x20)/0x10*13 + n-1]);
	int id = s == JKR ? ARRAY_SIZEOF(_cards)-1 : (n*4 + (s-0x20)/0x10);
	if (id >= ARRAY_SIZEOFI(_cards)) throw "カードエラー";
	return Card(id);
}

size_t Card::getAllCount()
{
	return ARRAY_SIZEOF(_cards);
}

std::string Card::suitToString(Suit st)
{
	std::string s("");
	switch (st) {
	case SPD : s="S"; break;
	case HRT : s="H"; break;
	case DIA : s="D"; break;
	case CLV : s="C"; break;
	default: s="J"; break;
	}
	return s;
}

int compare(const Card& a, const Card& b)
{
	////LOGV("compare() a=[%s] %p, b=[%s] %p", a.toString().c_str(), &a, b.toString().c_str(), &b);
	//// 同じなら0
	//if (&a == &b) return 0;
	//// ジョーカーなら最強
	//if (a.isJoker()) return (a.getNum() + 10) % 13;
	//if (b.isJoker()) return (b.getNum() + 10) % 13 - 13;
	//// 数字で比較（3→2）
	//int anum = (a.getNum() + 10) % 13;
	//int bnum = (b.getNum() + 10) % 13;
	//if (anum != bnum) return anum - bnum;
	//// スートで比較
	//return (a.getSuit() - b.getSuit()) >> 8;
	return a.idx - b.idx;
}

/**
 * カードの比較
 * 2 > A > K > Q > J > 10 ... 3
 */
int Card::compareTo(const Card& other) const { return compare(*this, other); }
bool Card::operator<(const Card& other) const { return compare(*this, other) < 0; }
bool Card::operator>(const Card& other) const { return compare(*this, other) > 0; }
bool Card::operator<=(const Card& other) const { return compare(*this, other) <= 0; }
bool Card::operator>=(const Card& other) const { return compare(*this, other) >= 0; }
bool Card::operator==(const Card& other) const { return compare(*this, other) == 0; }
bool Card::operator!=(const Card& other) const { return compare(*this, other) != 0; }


// ========== class CardSet ==========

CardSet::CardSet(const std::vector<Card>& cards) : bits(0)
{
	for (size_t i=0; i<cards.size(); i++) {
		uint64_t mask = 1l;
		mask <<= cards[i].getId();
		bits |= mask;
	}
}

CardSet::CardSet(const char* str) : bits(0)
{
	for (const char* p = str; *p; p++) {
		if (*p == ' ') continue;
		uint64_t mask = 1l;
		mask <<= _calcCardIdFromString(p);
		bits |= mask;
		p++;
	}
}

CardSet CardSet::allCards()
{
	uint64_t bit = 1l;
	bit <<= 53;
	return CardSet(bit-1);
}

size_t CardSet::size() const
{
	size_t count = 0;
	uint64_t b = bits;
	for (size_t i=0; i<Card::getAllCount(); i++) {
		if (b & 0x01) count++;
		b >>= 1;
	}
	return count;
}

void CardSet::operator-=(const CardSet& other)
{
	this->bits &= ~other.bits;
}

void CardSet::operator+=(const CardSet& other)
{
	this->bits |= other.bits;
}

void CardSet::operator-=(const Card& other)
{
	uint64_t tmp = 1l;
	tmp <<= other.getId();
	this->bits &= ~(tmp);
}

void CardSet::operator+=(const Card& other)
{
	uint64_t tmp = 1l;
	tmp <<= other.getId();
	this->bits |= (tmp);
}

bool CardSet::has(const Card& c) const
{
	uint64_t tmp = 1l;
	tmp <<= c.getId();
	return (this->bits & (tmp)) != 0;
}

bool CardSet::hasNumOf(int num, CardSet* output) const
{
	uint64_t b = 0x0f;
	uint64_t new_bits = 0;
	b <<= 4*((num+10)%13);
	new_bits |= b;
	new_bits &= bits;
	if (output) output->bits |= new_bits;
	return new_bits!=0;
}

int CardSet::extractJoker(void)
{
	Card jk = Card::get(JKR, 0);
	if (!has(jk)) return false;
	(*this) -= jk;
	return 1;
}

std::vector<Card> CardSet::conv2Vector() const
{
	std::vector<Card> v;
	conv2Vector(v);
	return v;
}

void CardSet::conv2Vector(std::vector<Card>& v) const
{
	uint64_t b = bits;
	int idx=0;
	v.clear();
	for (size_t i=0; b; i++) {
		if (b & 0x01) v.push_back(Card(idx));
		b >>= 1;
		idx++;
	}
}

Card CardSet::at(int index) const
{
	int hit = 0, idx = 0;
	uint64_t b = bits;
	for (size_t i=0; i<Card::getAllCount(); i++) {
		if (b & 0x01) {
			if (hit++ == index) return Card(idx);
		}
		b >>= 1;
		idx++;
	}
	throw "IndexOutOfRange";
}

std::string CardSet::toString() const
{
	std::string s;
//	std::vector<Card> v = this->conv2Vector();
//	for (std::vector<Card>::iterator i = v.begin(); i != v.end(); i++) {
//		s += i->toString();
//		s += " ";
//	}
	for (int i=0; i < size(); i++) {
		if (i) s += " ";
		s += at(i).toString();
	}
	return s;
}

