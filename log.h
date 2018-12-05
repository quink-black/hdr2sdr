#ifndef TONEMAP_LOG_H
#define TONEMAP_LOG_H

#ifdef __ANDROID__
#   include <android/log.h>

#   define LOG_TAG "Quink"
#   define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#   define ALOGD(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#else
#   include <stdio.h>
#   include <stdlib.h>
#   define ALOGE(fmt, ...)   printf(fmt "\n", ##__VA_ARGS__)
#   define ALOGD(fmt, ...)   printf(fmt "\n", ##__VA_ARGS__)
#endif

#endif
