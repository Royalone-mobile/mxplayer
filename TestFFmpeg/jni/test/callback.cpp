#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <jni.h>
#include <android/log.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))



/**********��������*************

*/
JNIEXPORT void JNICALL Java_com_nan_callback_MyCallbackActivity_callJNIInt( JNIEnv* env, jobject obj , jint i)
{
    //�ҵ�java�е���
    jclass cls = (*env)->FindClass(env, "com/nan/callback/MyCallbackActivity");
    //�������еķ���
    jmethodID mid = (*env)->GetMethodID(env, cls, "callbackInt", "(I)V");
    if (mid == NULL) 
    {
        LOGI("int error");
        return;  
    }
    //��ӡ���յ�������
    LOGI("from java int: %d",i);
    //�ص�java�еķ���
    (*env)->CallVoidMethod(env, obj, mid ,i);
        
}    

/********�����ַ���*************
*/
JNIEXPORT void JNICALL Java_com_nan_callback_MyCallbackActivity_callJNIString( JNIEnv* env, jobject obj , jstring s)
{
    //�ҵ�java�е���
    jclass cls = (*env)->FindClass(env, "com/nan/callback/MyCallbackActivity");
    //�������еķ���
    jmethodID mid = (*env)->GetMethodID(env, cls, "callbackString", "(Ljava/lang/String;)V");
    if (mid == NULL) 
    {
        LOGI("string error");
        return;  
    }
    const char *ch;
    //��ȡ��java���������ַ���
    ch = (*env)->GetStringUTFChars(env, s, NULL);
    //��ӡ
    LOGI("from java string: %s",ch);
    (*env)->ReleaseStringUTFChars(env, s, ch);    
    //�ص�java�еķ���
    (*env)->CallVoidMethod(env, obj, mid ,(*env)->NewStringUTF(env,"���haha"));

}

/********��������(byte[])*************
*/
JNIEXPORT void JNICALL Java_com_nan_callback_MyCallbackActivity_callJNIByte( JNIEnv* env, jobject obj , jbyteArray b)
{
    //�ҵ�java�е���
    jclass cls = (*env)->FindClass(env, "com/nan/callback/MyCallbackActivity");
    //�������еķ���
    jmethodID mid = (*env)->GetMethodID(env, cls, "callbackByte", "([B)V");
    if (mid == NULL) 
    {
        LOGI("byte[] error");
        return;  
    }
    
    //��ȡ���鳤��
    jsize length = (*env)->GetArrayLength(env,b);
    LOGI("length: %d",length);    
    //��ȡ���յ�������
    int i;
    jbyte* p = (*env)->GetByteArrayElements(env,b,NULL);
    //��ӡ
    for(i=0;i<length;i++)
    {
        LOGI("%d",p[i]);
    }

    char c[5];
    c[0] = 1;c[1] = 2;c[2] = 3;c[3] = 4;c[4] = 5;
    //��������
    jbyteArray carr = (*env)->NewByteArray(env,length);
    (*env)->SetByteArrayRegion(env,carr,0,length,c);
    //�ص�java�еķ���
    (*env)->CallVoidMethod(env, obj, mid ,carr);
}
