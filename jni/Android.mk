LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#OPENCV_CAMERA_MODULES:=off
#OPENCV_INSTALL_MODULES:=off
OPENCV_LIB_TYPE:=STATIC

OPENCV_ANDROID_SDK := F:/libsource/opencv/OpenCV-3.1.0-android-sdk/OpenCV-android-sdk

ifdef OPENCV_ANDROID_SDK
  ifneq ("","$(wildcard $(OPENCV_ANDROID_SDK)/OpenCV.mk)")
    include ${OPENCV_ANDROID_SDK}/OpenCV.mk
  else
    include ${OPENCV_ANDROID_SDK}/sdk/native/jni/OpenCV.mk
  endif
else
  include ../../sdk/native/jni/OpenCV.mk
endif

LOCAL_SRC_FILES  := eigen_faces.cpp facerec.cpp fisher_faces.cpp lbph_faces.cpp map_predict_collector.cpp mindist_predict_collector.cpp predict_collector.cpp stat_predict_collector.cpp std_predict_collector.cpp topn_predict_collector.cpp vector_predict_collector.cpp org_facemodule_FaceRecognizer.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE     := FaceRecognizer

include $(BUILD_SHARED_LIBRARY)
