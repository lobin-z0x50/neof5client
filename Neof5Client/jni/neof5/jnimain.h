#ifndef __JNIMAIN_H__
#define __JNIMAIN_H__

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint Java_jp_co_neogenia_neof5client_MainActivity_jnimain(JNIEnv* env, jobject thiz, jint playerLevel, jint ruleAB, jstring host, jint port, jstring path);

#ifdef __cplusplus
}
#endif

#endif
