//***************************************************************************************
// Load a image and find all visible fingers in this image
// Autors:	Kay Goossen
// Date:	29 September 2015
// Version: 1.00
//***************************************************************************************

#include<jni.h>
#include<string.h>
#include "OpenCVExtentions.h"
#include "HandReconisionTools.h"
#include "Math.h"


extern "C" {
	/**
     * getPicture receives the camera data from MainActivity
     * Returns the string with hand sign
     */
	jstring Java_kayentim_handyvision_MainActivity_getPicture(JNIEnv* env, jobject obj){

		vision::Mat srcHSV;
		vision::Mat obj;
		vision::Mat srcHSV  = vision::Mat(obj);
		vision::bgrtohsv(obj, srcHSV);

		return env->NewStringUTF("HelloWorld_2");

	}
}
