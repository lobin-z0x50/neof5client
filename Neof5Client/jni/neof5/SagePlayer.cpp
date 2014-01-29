/*
 * SagePlayer.cpp
 *
 * Copyright (C) 2014 W.Maeda <w.maeda@neogenia.co.jp>
 *
 */
#include <ctime>
#include <algorithm>
#include <set>
#include <list>
#include <random>

#include "mydef.h"
#include "SagePlayer.hpp"

#define LOG_HEAD "SagePlayer:: "

#define THINK_TIME_LIMIT 18

#define HAND_POTENTIAL_MAX (-1.21)
#define IS_STRONG_HAND(p1) (p1 < (hand_potential_max+0.5))

static std::mt19937 _engine;
static std::uniform_int_distribution<int> _rdist(0, 1000);

// 乱数の生成
static int getRand(int max)
{
	return _rdist(_engine) % max;
}

SagePlayer::SagePlayer(void)
	:_savedHandsTree()
{
}

// すべてのカードの組み合わせを生成する。
void SagePlayer::_genAllCombination(std::set<CardSet>& buff, const CardSet baseHand, const CardSet& deck, CardSet& ba_last)
{
//	LOGV(LOG_HEAD "_genAllCombination(baseHand=%s)", baseHand.toString().c_str());

	int n = baseHand.size();
	auto tmp = deck.conv2Vector();  // ここでtempは強い順に並んでいるはず
	for (auto i=tmp.rbegin(); i!=tmp.rend(); i++) {
		CardSet hand = baseHand;
		hand += *i;
		
		if (buff.find(hand) != buff.end()) continue;

		if (_rule->canPut(hand, ba_last) == ChkResult::OK) buff.insert(hand);
		else if (n>=2) continue;  // 3枚以上で出せなければこれより枚数を増やしてもしかたない。
		
		//if (n>=5) continue;  // これ以上の組み合わせは考えない。

		// 更に枚数を増やして再帰
		auto deck2 = deck;
		deck2 -= *i;
		_genAllCombination(buff, hand, deck2, ba_last);
	}
}

// 出せる手を全部列挙する
void SagePlayer::_enumrateAvairableHands(std::set<CardSet>& buff, const CardSet& tefda, CardSet ba_last, bool isKakumei)
{
	// 手札を逆順にソート（ジョーカーから先に調べるため）

	_rule->setKakumei(isKakumei);

//	LOGV(LOG_HEAD "_enumrateAvairableHands() start... tefda=[%s] ba_last=[%s]", tefda.toString().c_str(), ba_last.toString().c_str());

	// 可能な手を全て生成
	CardSet dmy;
	_genAllCombination(buff, dmy, tefda, ba_last);
	
//	LOGV(LOG_HEAD "_enumrateAvairableHands() finish. num of hands = %lu", buff.size());
}

// 返される可能性を計算する
double SagePlayer::_calcPotentialToBeFightback(const DaifugoContext& ctx, const CardSet& hand, bool isKakumei)
{
	double result = hand_potential_max;

	Card firstCard = hand[0];
	if (firstCard.isJoker()) return result;
	
	const double pn = ctx.getNumOfAvailablePlayers() -1;
	const double joker_count = _notappears_cards.hasNumOf(CARD_STRENGTH_MAX);
	const double hand_count = hand.size();
	int st = firstCard.getStrength();

	//LOGV(LOG_HEAD "_calcPotential... jkcount=%1.0lf st=%d", joker_count, st);

	if (hand_count >= 3 && hand[1].getStrength() != st) {
		// 階段の場合は手抜き
		double p = (50+(200*joker_count) / hand_count / st / st);
		LOGD(LOG_HEAD "_calcPotential... it's KAIDAN !!! point = %lf", p);
		return result + p;
	}

	// より強いカードが残っているか
	while(1) {
		st += isKakumei ? -1 : 1;
		if(st < 1 || CARD_STRENGTH_MAX <= st) break;

		CardSet strengthCards;
		if (_notappears_cards.hasNumOf(st % 13, &strengthCards)) {
			double strength_count = strengthCards.size();

			// カード１枚の場合はそのまま残り枚数をポイント計上
			if (hand_count==1) {
				result += strength_count;
				continue;
			}

			// 複数枚出しの場合は、ジョーカーの残り枚数を考慮する
			strength_count += joker_count;
			if (hand_count <= strength_count) {
				// 残っているカード枚数 / 残っている人数 / 枚数
				result += strength_count / pn / (pn==1 ? 1 : hand_count);
			}
		}
	}

	// ジョーカーが残っているか
	if (hand_count == 1) result += joker_count;

	return result;
}

/**
 * 思考ルーチンのエントリポイント
 */
CardSet SagePlayer::think(const DaifugoContext& ctx)
{
	CardSet ba_last;
	if (!ctx.getBa().empty()) ba_last = ctx.getBa().back();
	
	if (ctx.getNumOfAvailablePlayers() == 2 && getRand(8)==1) {
		_tweetFunc("まずいのう。。。");
	}

	if (!_savedHandsTree.empty()) {
		LOGV(LOG_HEAD "there is saved hands tree. size=%d", (int)_savedHandsTree.size());
		// 保存された手順があればそれを使う
		auto hand = _savedHandsTree.back();
		_rule->setKakumei(ctx.isKakumei());
		if (ctx.getDeck().has(hand) && _rule->canPut(hand, ba_last) == ChkResult::OK) {
			_savedHandsTree.pop_back();
			LOGI(LOG_HEAD "use saved hand ba_last=%s deck=%s", ba_last.toString().c_str(), ctx.getDeck().toString().c_str());
			if (getRand(9) == 1) _tweetFunc("ほれっ！");
			return hand;
		}
		_savedHandsTree.clear();
	}

	return think2(ctx, ba_last);
}

CardSet SagePlayer::think2(const DaifugoContext& ctx, CardSet& ba_last)
{
	time_t timelimit;
	time(&timelimit);
	timelimit += THINK_TIME_LIMIT;

	// 明らかになったカード
	CardSet appears_cards = ctx.getYama();
	appears_cards += ctx.getDeck();
	for (auto ba : ctx.getBa()) appears_cards += ba;
	// 明らかになっていないカードはそれの逆
	_notappears_cards = CardSet::allCards();
	_notappears_cards -= appears_cards;

	// 局面の進行状況に合わせて手札評価の最大ポイントを計算
	int appears_cards_count = appears_cards.size();
	hand_potential_max = HAND_POTENTIAL_MAX - (appears_cards_count <  8 ? 8.0
	                                          :appears_cards_count < 30 ? (7.0 - appears_cards.size()/4.8)
											  :                           0);

	EvalNode root(ctx.getDeck(), ctx.isKakumei());
	std::set<EvalNode*> v;
	v.insert(&root);

	LOGI(LOG_HEAD "think start. hand_potential_max=%1.2lf ba_last=%s deck=%s", hand_potential_max, ba_last.toString().c_str(), ctx.getDeck().toString().c_str());
	
	auto result = _think_recursive(timelimit, ctx, ba_last, v);

	LOGI(LOG_HEAD "think finish. result node = %p / num of child node = %d", result, root.numOfChildren());
	
	if (result) {
		// 探索解が見つかった！
		LOGI(LOG_HEAD "found = %s", result->treeToString().c_str());
		_savedHandsTree.clear();
		result->saveHandsTree(_savedHandsTree);  // 保存
		if (!_savedHandsTree.empty()) _savedHandsTree.pop_back();
		auto hh = result->getHeirOfRoot()->hand.hand;
		if (!hh.empty()) {
			if (getRand(3) == 1) _tweetFunc("わしの勝ちじゃ！");
			else if (getRand(4) == 1) _tweetFunc("ほほっ！");
		}
		return hh;
	}

	// 手札の良さで判定
	LOGI(LOG_HEAD "Looking for better deck point. ba_last=%s deck=%s", ba_last.toString().c_str(), ctx.getDeck().toString().c_str());
	result = root.findBestParamChild();
	LOGI(LOG_HEAD "better node = %p %s", result, result ? result->treeToString().c_str() : "-");
	if (!result) result = root.getChildAt(root.numOfChildren()-1);  // nullなら多分バグ。その場合は最後の子とする。
	for (int i=0; i<root.numOfChildren(); i++) {
		auto child = root.getChildAt(i);
		LOGD(LOG_HEAD " node[%2d] %s", i, child->treeToString().c_str());
	}
	auto hh = result->getHeirOfRoot()->hand.hand;
	if (hh.empty() && getRand(12) == 5) {
		_tweetFunc("むむぅ");
	}
	return hh;
}

EvalNode* SagePlayer::_think_recursive(long timelimit, const DaifugoContext& ctx, const CardSet& ba_cards, std::set<EvalNode*>& nodes)
{
	const CardSet dmy;
	std::set<EvalNode*> nextNodes;
	
	for (auto node : nodes) {
		LOGV(LOG_HEAD "Thinking node... %p %s", node, node->treeToString().c_str());

		// 出せる手を全て列挙する
		std::set<CardSet> hands;
		_enumrateAvairableHands(hands, node->hand.tefda, ba_cards, node->isKakumei);

		if (hands.empty()) return node;  // 出せる手がない。

		_rule->setKakumei(node->isKakumei);
	
		// リストを２つ作って分類する
		std::list<EvalHand> betterHands;  // 流せる手
		std::list<EvalHand> poorHands;    // 流せない手
		for (auto& hand : hands) {
			int n = hand.size();
			// その手の評価
			EvalHand eh;
			eh.hand = hand;
			eh.tefda = node->hand.tefda;
			eh.tefda -= hand;
			eh.p1 = _calcPotentialToBeFightback(ctx, hand, node->isKakumei);
			eh.p2 = 0;
			node->hand.p2 += eh.p1 - (n-1);  // このノードの手札評価に反映する

			if (eh.tefda.empty()) {
				// 手札がなくなった！初手または前の手またはこれが流せる手ならあがり確定！
				if (node->isRoot() || IS_STRONG_HAND(node->hand.p1) || (IS_STRONG_HAND(eh.p1) && n==node->hand.hand.size())){
					if (_rule->canAgari(hand) == ChkResult::OK) {
						auto child2 = node->createChild(eh);
						LOGI(LOG_HEAD "あがり確定やで！");
						return child2;
					} else {
						LOGI(LOG_HEAD "あがり禁止札 %s", hand.toString().c_str());
						if (!node->isRoot()) continue;
					}
				}
			}
			
			if (time(NULL) > timelimit) {
				_tweetFunc("ぐぐ、時間がない。。。");
				return NULL;  // タイムリミット
			}

			if (IS_STRONG_HAND(eh.p1)) betterHands.push_back(eh);
			else poorHands.push_back(eh);
		}

		LOGV(LOG_HEAD "better=%lu poor=%lu", betterHands.size(), poorHands.size());

		// 流せる手から一つとって、流せない手を選ぶ。ただし深さ2以降でパスする手の場合は先読みしない
		if (node->isRoot() || node->getParent()->isRoot() || !node->getHeirOfRoot()->hand.hand.empty()){
			for (auto& beh : betterHands) {
				// このノードが流せない手なら、枚数が一致してて、かつ強さが２上でないと出せない。
				if (!node->hand.hand.empty() && !IS_STRONG_HAND(node->hand.p1)) {
					if (node->hand.hand.size() != beh.hand.size()) continue;  // 枚数不一致
					if (node->hand.hand[0].getStrength() +(node->isKakumei?-2:2) > beh.hand[0].getStrength()) continue;  // 強さが２未満
				}
				// 子ノードを追加
				auto child = node->createChild(beh);
				if (poorHands.empty()) {
					nextNodes.insert(child);
					continue;
				}
				for (auto& peh : poorHands) {
					if (peh.hand.communion(beh.hand)) {
						// かぶっているのでダメ
						nextNodes.insert(child);
						continue;
					}
					auto child2 = node->createChild(peh);
					if (child2->hand.tefda.empty()) {
						// 手札がなくなった！あがり禁止札の判定でOKならあがり確定！
						if (_rule->canAgari(child2->hand.hand) == ChkResult::OK) {
							LOGI(LOG_HEAD "あがり確定やで！その２");
							return child2;
						} else {
							LOGI(LOG_HEAD "あがり禁止札 %s その２", child2->hand.hand.toString().c_str());
							continue;
						}
					}
					nextNodes.insert(child2);
				}
			}
		}
		// ルートノードの場合は流せない手も読む(全ての手について手札評価を行わせるため)
		//LOGI(LOG_HEAD "node->getparent() = %p", node->getParent());
		if (node->isRoot()) {
			for (auto& peh : poorHands) {
				auto child2 = node->createChild(peh);
				if (child2->hand.tefda.empty()) {
					// 手札がなくなった！上がり確定！
					LOGI(LOG_HEAD "あがり確定！ %s", peh.hand.toString().c_str());
					return child2;
				}
				nextNodes.insert(child2);
			}
			if (!ba_cards.empty()){
				// 親でなければパスする手も入れる
				EvalHand eh;
				eh.tefda = node->hand.tefda;
				auto child2 = node->createChild(eh);
				nextNodes.insert(child2);
			}
		}
	}

	// 子どものノードについて再帰
	if (!nextNodes.empty()){
		LOGV(LOG_HEAD "次の再帰へ。こどものノード数＝%lu", nextNodes.size());
		return _think_recursive(timelimit, ctx, dmy, nextNodes);
	}
	return NULL;
}

