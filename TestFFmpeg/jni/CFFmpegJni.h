/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_example_testffmpeg_CFFmpegJni */

#ifndef _Included_com_example_testffmpeg_CFFmpegJni
#define _Included_com_example_testffmpeg_CFFmpegJni
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IGetVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_testffmpeg_CFFmpegJni_IGetVersion
  (JNIEnv *, jobject);

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IInit
 * Signature: (Ljava/lang/String;IIII)I
 */
JNIEXPORT jint JNICALL Java_com_example_testffmpeg_CFFmpegJni_IInit
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IResize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_example_testffmpeg_CFFmpegJni_IResize
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IPlay
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_testffmpeg_CFFmpegJni_IPlay
  (JNIEnv *, jobject);

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IStop
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_example_testffmpeg_CFFmpegJni_IStop
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
