#include <com_example_weidongzhang_opencvfirst_FaceRecognizerCollection.h>

#include <opencv2/core/core.hpp>
#include <opencv2/face.hpp>
#include <opencv2/imgcodecs.hpp>

#include <android/log.h>

#include <vector>
#include <list>

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

JNIEXPORT jlong JNICALL Java_com_example_weidongzhang_opencvfirst_FaceRecognizerCollection_nativeCreateObject
(JNIEnv * jenv, jclass, jint)
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

JNIEXPORT void JNICALL Java_com_example_weidongzhang_opencvfirst_FaceRecognizerCollection_nativeTrain
(JNIEnv *, jclass, jlong addr, jlongArray, jintArray){
	std::vector<string> sImg;
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221648.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221650.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221652.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221653.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221656.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221658.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221659.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221700.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_221701.png");



	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232051.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232053.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232054.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232055.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232056.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232058.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232100.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232101.png");
	sImg.push_back("/storage/sdcard0/Pictures/facerec/20160417_232102.png");



	vector<Mat> matImgs;
	vector<int> matLabels;
	for (int i = 0; i < 9; ++i)
	{
		matImgs.push_back(imread(sImg.at(i), 0));
		matLabels.push_back(9);
	}

	for (int i = 9; i < 18; ++i)
	{
		matImgs.push_back(imread(sImg.at(i), 0));
		matLabels.push_back(20);
	}

	((FaceRecognizerAdapter*)(addr))->model->train(matImgs, matLabels);
}

JNIEXPORT void JNICALL Java_com_example_weidongzhang_opencvfirst_FaceRecognizerCollection_nativeUpdate
(JNIEnv *, jclass, jlong, jlongArray, jintArray){

}

JNIEXPORT jint JNICALL Java_com_example_weidongzhang_opencvfirst_FaceRecognizerCollection_nativePredict
(JNIEnv *, jclass, jlong addr){
	try
	{
		Mat img = imread("/storage/sdcard0/Pictures/facerec/20160417_232053.png", 0);
		return ((FaceRecognizerAdapter*)(addr))->model->predict(img);
	}
	catch (...)
	{
		return -100;
	}
}


JNIEXPORT jint JNICALL Java_com_example_weidongzhang_opencvfirst_FaceRecognizerCollection_nativePredict1
(JNIEnv *, jclass, jlong addr, jlong img)
{
	//Mat img1 = imread("/storage/sdcard0/Pictures/facerec/20160418_003313.png", 0);
	//Mat img1 = imread("/storage/sdcard0/Pictures/facerec/20160417_232053.png", 0);

	int iLabel = 0;
	double dConfidence = 0.0;
	((FaceRecognizerAdapter*)(addr))->model->predict(*((Mat*)img), iLabel, dConfidence);

	if (dConfidence < 150)
	{
		return iLabel;
		//return -200;
	}
	else
	{
		return -200;
	}

	//return ((FaceRecognizerAdapter*)(addr))->model->predict(*((Mat*)img));
	//return ((FaceRecognizerAdapter*)(addr))->model->predict(img1);
}

JNIEXPORT void JNICALL Java_com_example_weidongzhang_opencvfirst_FaceRecognizerCollection_nativeDestroyObject
(JNIEnv *, jclass, jlong){

}