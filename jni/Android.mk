LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
OPENCV_CAMERA_MODULES:=off
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=STATIC
include $(OPENCV_PACKAGE_DIR)/share/OpenCV/OpenCV.mk

LOCAL_ARM_NEON := true
LOCAL_NEON_CFLAGS := -mfloat-abi=softfp -mfpu=neon -march=armv7 
#LOCAL_CFLAGS = -pg -g
LOCAL_CFLAGS +=  -Ofast -funroll-loops -fmodulo-sched -fmodulo-sched-allow-regmoves 
LOCAL_ARM_MODE := arm
LOCAL_MODULE    := test
LOCAL_SRC_FILES := main.cpp ./src/gaussianBlur.cpp
LOCAL_PRELINK_MODULE:=false

include $(BUILD_EXECUTABLE)

