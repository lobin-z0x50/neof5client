# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

#---libwebsockets-------------------------------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE    := libwebsockets
LOCAL_CFLAGS    := -DLWS_BUILTIN_GETIFADDRS
LWS_LIB_PATH	:= ./libwebsockets-1.23/lib
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/$(LWS_LIB_PATH)
LOCAL_C_INCLUDES+= $(LOCAL_PATH)/lwsconfig.android
LOCAL_SRC_FILES := \
	$(LWS_LIB_PATH)/sha-1.c \
	$(LWS_LIB_PATH)/base64-decode.c \
	$(LWS_LIB_PATH)/client.c \
	$(LWS_LIB_PATH)/client-handshake.c \
	$(LWS_LIB_PATH)/client-parser.c \
	$(LWS_LIB_PATH)/server.c \
	$(LWS_LIB_PATH)/server-handshake.c \
	$(LWS_LIB_PATH)/daemonize.c \
	$(LWS_LIB_PATH)/extension.c \
	$(LWS_LIB_PATH)/extension-deflate-frame.c \
	$(LWS_LIB_PATH)/extension-deflate-stream.c \
	$(LWS_LIB_PATH)/getifaddrs.c \
	$(LWS_LIB_PATH)/handshake.c \
	$(LWS_LIB_PATH)/libwebsockets.c \
	$(LWS_LIB_PATH)/output.c \
	$(LWS_LIB_PATH)/parsers.c \
	$(LWS_LIB_PATH)/handshake.c \


include $(BUILD_STATIC_LIBRARY)
#------------------------------------------------------------------------------

#---neof5client----------------------------------------------------------------
include $(CLEAR_VARS)

LOCAL_MODULE    := neof5client
LOCAL_CPPFLAGS  += -std=c++11 -fexceptions
NEOF5_LIB_PATH  := ./neof5
LOCAL_SRC_FILES := \
	$(NEOF5_LIB_PATH)/Cards.cpp \
	$(NEOF5_LIB_PATH)/DaifugoContext.cpp \
	$(NEOF5_LIB_PATH)/DaifugoRule.cpp \
	$(NEOF5_LIB_PATH)/EvalNode.cpp \
	$(NEOF5_LIB_PATH)/Neof5Protocol.cpp \
	$(NEOF5_LIB_PATH)/PlayerInfo.cpp \
	$(NEOF5_LIB_PATH)/PoorPlayer.cpp \
	$(NEOF5_LIB_PATH)/SagePlayer.cpp \
	$(NEOF5_LIB_PATH)/WsThread.cpp \
	$(NEOF5_LIB_PATH)/jnimain.cpp
LOCAL_LDLIBS    += -llog -lz -lstdc++
LOCAL_STATIC_LIBRARIES := websockets

include $(BUILD_SHARED_LIBRARY)
#------------------------------------------------------------------------------
#$(call import-module,android/native_app_glue)
