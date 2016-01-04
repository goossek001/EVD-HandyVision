#include<jni.h>
#include<string.h>


/*
 * use Java_ to
 * Call the packageName from mainActivity
 * Change the dots to underscores
 * And create the function
 */
jstring Java_kayentim_handyvision_MainActivity_helloWorld(JNIEnv* env, jobject obj){

    return(*env)->NewStringUTF(env, "hello world");

}