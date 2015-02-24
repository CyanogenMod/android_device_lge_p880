LOCAL_PATH := $(call my-dir)

L_CFLAGS := -g -Wall

#
# Audio Policy Wrapper
#
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    common.cpp \
    aps_wrapper.cpp \
    audio_policy.cpp

LOCAL_SHARED_LIBRARIES := \
    libhardware liblog libutils
LOCAL_STATIC_LIBRARIES := libmedia_helper

LOCAL_CFLAGS := $(L_CFLAGS)

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE := audio_policy.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_HEAPTRACKED_SHARED_LIBRARY)

#
# Audio HW Wrapper
#
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    common.cpp \
    audio_hw.cpp

LOCAL_SHARED_LIBRARIES := \
    libhardware liblog libutils
LOCAL_STATIC_LIBRARIES := libmedia_helper

LOCAL_CFLAGS := $(L_CFLAGS)

LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE := audio.primary.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_HEAPTRACKED_SHARED_LIBRARY)
