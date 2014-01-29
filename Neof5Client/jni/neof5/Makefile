SRCS = Cards.cpp DaifugoContext.cpp DaifugoRule.cpp EvalNode.cpp Neof5Protocol.cpp PlayerInfo.cpp PoorPlayer.cpp SagePlayer.cpp WsThread.cpp clientmain.cpp
LIBWEBSOCKET = ../libwebsockets-1.23/build/lib/libwebsockets.a 
CPP_FLAGS = -lz -std=c++11 -lstdc++ -g
CC = g++

clientmain:
	$(CC) -o clientmain $(SRCS) $(LIBWEBSOCKET) $(CPP_FLAGS)

clean:
	rm clientmain

