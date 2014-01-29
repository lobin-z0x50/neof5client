#ifndef __MYDEF_H__
#define __MYDEF_H__

// MIN/MAX
#define MIN(x,y) ((x)>(y)?(y):(x))
#define MAX(x,y) ((x)<(y)?(y):(x))

// 配列サイズ計算
#define ARRAY_SIZEOF(a) (sizeof (a)/sizeof (a)[0])
#define ARRAY_SIZEOFI(a) (int)(sizeof (a)/sizeof (a)[0])

#ifdef __ANDROID__

// for ANDROID platform

#include <jni.h>
#include <errno.h>

#include <android/log.h>
//#include <android_native_app_glue.h>

#define LOG_TAG "Daif5"

#define LOGV(...) ((void)__android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))
#define LOGFLUSH

#else

// for ANSI C++ console application

#include <cstdio>
#include <ctime>
#define LOGV(...) (printf("VER:[%ld] ",time(NULL)),printf(__VA_ARGS__),puts(""))
#define LOGD(...) (printf("DBG:[%ld] ",time(NULL)),printf(__VA_ARGS__),puts(""))
#define LOGI(...) (printf("INF:[%ld] ",time(NULL)),printf(__VA_ARGS__),puts(""))
#define LOGW(...) (printf("WRN:[%ld] ",time(NULL)),printf(__VA_ARGS__),puts(""))
#define LOGE(...) (printf("ERR:[%ld] ",time(NULL)),printf(__VA_ARGS__),puts(""))
#define LOGFLUSH (fflush(stdout))

#include <stdint.h>

#endif

#endif
