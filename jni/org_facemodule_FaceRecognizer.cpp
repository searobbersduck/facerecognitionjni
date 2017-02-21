#include <org_facemodule_FaceRecognizer.h>

#include <opencv2/core/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/imgcodecs.hpp>

#include <android/log.h>

#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include <jni.h>

#define LOG_TAG "FaceDetection/DetectionBasedTracker"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;
using namespace cv::face;


struct FaceRecognizerAdapter
{
	cv::Ptr<LBPHFaceRecognizer> model;
	FaceRecognizerAdapter(){
		model = createLBPHFaceRecognizer(1, 8, 16, 16);
	}
};

JNIEXPORT jlong JNICALL Java_org_facemodule_FaceRecognizer_nativeCreateObject
  (JNIEnv* jenv, jclass, jint)
  {
	LOGD("Hello, welcome to my face recognition system!");

	jlong result = 0;


	try
	{
		//result = (jlong)new FaceRecognizerAdapter();
		result = (jlong)((new FaceRecognizerAdapter()));
	}
	catch (cv::Exception& e)
	{
		LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
		jclass je = jenv->FindClass("org/opencv/core/CvException");
		if (!je)
			je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je, e.what());
	}
	catch (...)
	{
		LOGD("nativeCreateObject caught unknown exception");
		jclass je = jenv->FindClass("java/lang/Exception");
		jenv->ThrowNew(je, "Unknown exception in JNI code of DetectionBasedTracker.nativeCreateObject()");
		return 0;
	}


	return result;  
  }
  
  
  JNIEXPORT jlong JNICALL Java_org_facemodule_FaceRecognizer_nativeStringArray
  (JNIEnv *env, jclass, jobjectArray stringArr)
  {
	  	std::stringstream ss;
	int cnt = env->GetArrayLength(stringArr);
	for (int i = 0; i < cnt; ++i)
	{
		jstring str = (jstring)(env->GetObjectArrayElement(stringArr, i));
		const char* rawString = env->GetStringUTFChars(str, 0);

		ss << std::string(rawString);// << std::endl;

		env->ReleaseStringUTFChars(str, rawString);
	}

	std::string s1 = ss.str();

	std::cout << ss.str() << std::endl;

	std::fstream outfile("/storage/sdcard0/Pictures/test.txt");
	outfile.write(ss.str().c_str(), ss.str().size());
	outfile.close();

	return s1.size();
  }
  
  
  JNIEXPORT void JNICALL Java_org_facemodule_FaceRecognizer_nativeDestroyObject
  (JNIEnv *, jclass, jlong addr)
  {
	  if(addr != 0)
	  {
		  delete (FaceRecognizerAdapter*)(addr);
	  }
  }
  
  JNIEXPORT void JNICALL Java_org_facemodule_FaceRecognizer_nativeTrain
  (JNIEnv * env, jclass, jlong addr, jobjectArray stringArr, jintArray labelArr)
  {
	vector<Mat> matImgs;
	vector<int> matLabels;
	std::vector<string> sImg;
	
	jint* iarr;
	iarr = env->GetIntArrayElements(labelArr, NULL);
	
	int cnt = env->GetArrayLength(stringArr);
	for (int i = 0; i < cnt; ++i)
	{
		jstring str = (jstring)(env->GetObjectArrayElement(stringArr, i));
		const char* rawString = env->GetStringUTFChars(str, 0);

		sImg.push_back(std::string(rawString));// << std::endl;
		matLabels.push_back(iarr[i]);

		env->ReleaseStringUTFChars(str, rawString);
	}
	
	for(int i = 0; i < sImg.size(); ++i)
	{
		matImgs.push_back(imread(sImg.at(i), 0));
	}
	
	env->ReleaseIntArrayElements(labelArr, iarr, 0);
	
	((FaceRecognizerAdapter*)(addr))->model->train(matImgs, matLabels);
  }
  
  JNIEXPORT void JNICALL Java_org_facemodule_FaceRecognizer_nativeUpdate
  (JNIEnv *, jclass, jlong, jobjectArray, jintArray){
	  
  }
  
  
  JNIEXPORT jint JNICALL Java_org_facemodule_FaceRecognizer_nativePredict
  (JNIEnv *, jclass, jlong addr, jlong inputImage)
  {
	  try
	  {
		  //cv::Mat img = imread("/storage/sdcard0/Pictures/facerec/haijun/20160515_221513.png",0);
		  int iLable = 100;
		  double dConfidence = 100000;
		  double dConfidenceStandard = 400;
		  ((FaceRecognizerAdapter*)(addr))->model->predict(*((cv::Mat*)inputImage), iLable, dConfidence);
		  if(dConfidence < dConfidenceStandard)
		  {
			  return iLable;
		  }
		  else{
			  return 10000;
		  }
		  //return ((FaceRecognizerAdapter*)(addr))->model->predict(img);
		  //return 100;
	  }catch(...)
	  {
		  return -100;
	  }
  }