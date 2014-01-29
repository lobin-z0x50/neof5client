/*
 * jnimain.cpp
 *
 *  Created on: 2012/03/01
 */

#include <jni.h>

#include "jnimain.h"

#include "mydef.h"
#include <cstdlib>
#include <memory>

#include "WsThread.hpp"
#include "Neof5Protocol.hpp"
#include "PoorPlayer.hpp"
#include "SagePlayer.hpp"

JNIEXPORT jint Java_jp_co_neogenia_neof5client_MainActivity_jnimain(JNIEnv* env, jobject thiz, jint playerLevel, jint ruleAB, jstring host, jint port, jstring path)
{
	Neof5Protocol p;
	WsThread wst(p);

	p.setSendFunc([&](const std::string& dat, bool flush) -> void { wst.sendText(dat, flush); });

	IPlayer* player; 
	if (playerLevel == 1) player = new PoorPlayer();
	else player = new SagePlayer();
	
	std::unique_ptr<IDaifugoRule> rule;
	if (ruleAB == 'B') {
		rule.reset(new DaifugoRuleB());
	} else {
		rule.reset(new DaifugoRuleA());
	}
	player->setRule(rule.get());
	p.setPlayer(player);

	const char* c_host = env->GetStringUTFChars(host, NULL);
	if (!c_host) return -1;

	const char* c_path = env->GetStringUTFChars(path, NULL);
	if (!c_path) { env->ReleaseStringUTFChars(host, c_host); return -2; }

	int ret = wst.run(c_host, port, c_path);

	delete player;

	env->ReleaseStringUTFChars(host, c_host);
	env->ReleaseStringUTFChars(path, c_path);

	return ret;
}
