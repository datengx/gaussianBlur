LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_NEON := true
LOCAL_NEON_CFLAGS := -mfloat-abi=softfp -mfpu=neon -march=armv7 
#LOCAL_CFLAGS = -pg -g
LOCAL_CFLAGS +=  -Ofast -funroll-loops -fmodulo-sched -fmodulo-sched-allow-regmoves 
LOCAL_ARM_MODE := arm
LOCAL_MODULE    := test
LOCAL_SRC_FILES := main.cpp
LOCAL_PRELINK_MODULE:=false

include $(BUILD_EXECUTABLE)

