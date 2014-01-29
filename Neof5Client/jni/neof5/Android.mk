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

include $(CLEAR_VARS)

LOCAL_MODULE    := neof5client
LOCAL_SRC_FILES := Cards.cpp DaifugoContext.cpp DaifugoRule.cpp EvalNode.cpp Neof5Protocol.cpp PlayerInfo.cpp PoorPlayer.cpp SagePlayer.cpp WsThread.cpp jnimain.cpp
LOCAL_CPPFLAGS  += -std=c++11 -fexceptions
LOCAL_LDLIBS    := -llog -lz -lstdc++ L../libwebsockets/obj/local/armeabi/libwebsockets.a -lwebsockets
LOCAL_STATIC_LIBRARIES := websockets

include $(BUILD_SHARED_LIBRARY)

#$(call import-module,android/native_app_glue)
