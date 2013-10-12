#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ffmpeg/libavcodec/avcodec.h"
#include "ffmpeg/libavformat/avformat.h"
#include "ffmpeg/libswscale/swscale.h"

#include "FFmpegJni.h"

#include <android/log.h>

#define FFMPEG_TAG "FFMpegAndroid"

#define MYLOG_TAG "TestNDK_DEBUG"

/// ����ȫ�ֱ���
AVFormatContext* m_pFormatCtx = NULL;
AVDictionary* m_pDOptions = NULL;

char m_szURLPath[512];

/// ������־�ص�����
static void e_PrintFFmpeg(void* ptr, int level, const char* fmt, va_list vl)
{
	switch(level)
	{
	case AV_LOG_PANIC:
		__android_log_print(ANDROID_LOG_ERROR, FFMPEG_TAG, "AV_LOG_PANIC: %s", fmt);
		break;

	case AV_LOG_FATAL:
		__android_log_print(ANDROID_LOG_ERROR, FFMPEG_TAG, "AV_LOG_FATAL: %s", fmt);
		break;

	case AV_LOG_ERROR:
		__android_log_print(ANDROID_LOG_ERROR, FFMPEG_TAG, "AV_LOG_ERROR: %s", fmt);
		break;

	case AV_LOG_WARNING:
		__android_log_print(ANDROID_LOG_ERROR, FFMPEG_TAG, "AV_LOG_WARNING: %s", fmt);
		break;

	case AV_LOG_INFO:
		__android_log_print(ANDROID_LOG_INFO, FFMPEG_TAG, "AV_LOG_INFO: %s", fmt);
		break;

	case AV_LOG_DEBUG:
		__android_log_print(ANDROID_LOG_DEBUG, FFMPEG_TAG, "AV_LOG_DEBUG: %s", fmt);
		break;
	}
}

/// �����ӡ��Ϣ
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, MYLOG_TAG, __VA_ARGS__)

/*
 * Class:     com_example_testffmpeg_FFmpegJni
 * Method:    IGetVersion
 * Signature: ()I
 */
jint Java_com_example_testffmpeg_FFmpegJni_IGetVersion(JNIEnv *env, jobject thiz)
{
	/// ������־��Ϣ�ص�
	av_log_set_callback(e_PrintFFmpeg);

	/// ��ӡ��־
	LOGD("Set FFMpeg Log Call Back Success. ");

	/// ��ȡ�汾��Ϣ����ʹ��
	int nRet = avcodec_version();

	return nRet;
}

/*
 * Class:     com_example_testffmpeg_FFmpegJni
 * Method:    IInit
 * Signature: (Ljava/lang/String;IIII)I
 */
jint Java_com_example_testffmpeg_FFmpegJni_IInit(JNIEnv *env, jobject thiz, jstring jstrRTSPUrl,
		jint jnMeidaType, jint jnSawle, jint jnWith, jint jnHeight)
{
	/// ע�������
	avcodec_register_all();

	const char* pstrRTSPUrl =(*env)->GetStringUTFChars(env, jstrRTSPUrl, 0);
	/// ��ֵRTSP�����ַ
	strcpy(m_szURLPath, pstrRTSPUrl);

	/// ����rtspΪTCP��ʽ
	if(1 == jnMeidaType)
	{
		av_dict_set(&m_pDOptions, "rtsp_transport", "tcp", 0);
	}
	/// ��ʼ������
	int nRet = avformat_network_init();
	return nRet;
}

/*
 * Class:     com_example_testffmpeg_FFmpegJni
 * Method:    IResize
 * Signature: (II)V
 */
void Java_com_example_testffmpeg_FFmpegJni_IResize(JNIEnv *env, jobject thiz, jint jnWith, jint jnHeight)
{
}

/*
 * Class:     com_example_testffmpeg_FFmpegJni
 * Method:    IPlay
 * Signature: ()I
 */
jint Java_com_example_testffmpeg_FFmpegJni_IPlay(JNIEnv *env, jobject thiz)
{
	/// ���巵��ֵ
	int nRet = -1;
	/// ���ļ�
	if(NULL == m_pFormatCtx)
	{
		m_pFormatCtx = avformat_alloc_context();

		if(NULL != m_pFormatCtx && NULL != m_pDOptions)
		{
			/// ���ļ�
			nRet = avformat_open_input(&m_pFormatCtx, m_szURLPath, 0, &m_pDOptions);
		}
	}
	return nRet;
}

/*
 * Class:     com_example_testffmpeg_FFmpegJni
 * Method:    IStop
 * Signature: ()I
 */
jint Java_com_example_testffmpeg_FFmpegJni_IStop(JNIEnv *env, jobject thiz)
{
	/// �رմ򿪵��ļ�
	avformat_close_input(&m_pFormatCtx);
	return 0;
}
