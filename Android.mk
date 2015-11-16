LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	gao.cc

LOCAL_SHARED_LIBRARIES := \
	libaudioflinger \
	libcameraservice \
	libcutils \
	libmedia \
	libmediaplayerservice \
	libutils \
	liblog \
	libbinder


LOCAL_C_INCLUDES := \
    frameworks/av/media/libmediaplayerservice \
    frameworks/av/services/medialog \
    frameworks/av/services/audioflinger

LOCAL_MODULE:= mediapoc

include $(BUILD_EXECUTABLE)
