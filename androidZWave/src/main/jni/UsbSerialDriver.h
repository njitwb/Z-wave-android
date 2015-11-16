//
// Created by archermind on 11/11/15.
//

#ifndef Z_WAVE_ANDROID_USBSERIALDRIVER_H
#define Z_WAVE_ANDROID_USBSERIALDRIVER_H

#include "com_azwave_androidzwave_UsbSerialDriver.h"

#include "android/log.h"

#define DEBUG

#define TAG "ZWAVE"
#ifdef DEBUG
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#else
#define LOGI(...) {}
#define LOGD(...) {}
#define LOGE(...) {}
#endif


#endif //Z_WAVE_ANDROID_USBSERIALDRIVER_H
