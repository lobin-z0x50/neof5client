
#include "mydef.h"
#include <cstdlib>
#include <memory>

#include "WsThread.hpp"
#include "Neof5Protocol.hpp"
#include "PoorPlayer.hpp"
#include "SagePlayer.hpp"

static void usage(char* myname) {
	LOGI("Usage: %s <A|B> <host> <port> <path>", myname);
}

int main(int argc, char* argv[]) {

	// argument check
	if (argc != 5) {
		usage(argv[0]);
		return 1;
	}

	Neof5Protocol p;
	WsThread wst(p);

	p.setSendFunc([&](const std::string& dat, bool flush) -> void { wst.sendText(dat, flush); });

	SagePlayer player;
	std::unique_ptr<IDaifugoRule> rule;
	if (argv[1][0] == 'B') {
		rule.reset(new DaifugoRuleB());
	} else {
		rule.reset(new DaifugoRuleA());
	}
	player.setRule(rule.get());
	p.setPlayer(&player);

	int ret = wst.run(argv[2], atoi(argv[3]), argv[4]);

	return ret;
}
