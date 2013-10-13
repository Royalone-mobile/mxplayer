#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "CRGB2BMP.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/log.h"

#ifdef __cplusplus
}
#endif// end of extern C

#include "CFFmpegJni.h"

#include <android/log.h>

#define FFMPEG_TAG "FFMpegAndroid"

#define MYLOG_TAG "TestNDK_DEBUG"

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)

/// ����ȫ�ֱ���
AVFormatContext* m_pFormatCtx = NULL;

char m_szURLPath[512];
int m_nWith = 0;

jclass		m_CallBackClass = NULL;
jmethodID 	m_CallBackQueueMethod = NULL;

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


void e_SaveFrame(AVFrame* pFrameRGB, int nWidth, int nHeight)
{
	int w = nWidth;
	int h = nHeight;
	int nBufferLen = WIDTHBYTES(w * 24) * h;
	BYTE* pBuffer = new BYTE[nBufferLen];
	memset(pBuffer, 0 , nBufferLen);

	for (int i = 0; i < h; i++)
	{
		//��ͼ��תΪbmp�浽�ڴ���, �����ͼ���ǵ�����
		memcpy(pBuffer + (WIDTHBYTES(w * 24) * i), pFrameRGB->data[0] + i * pFrameRGB->linesize[0], w * 3);
	}

	BYTE* pBMPData = new BYTE[sizeof(BmpHead) + sizeof(InfoHead) + nBufferLen];

    BmpHead m_BMPHeader;
    m_BMPHeader.bfType = 'BM';
    m_BMPHeader.imageSize = 3 * nWidth*nHeight + sizeof(BmpHead) + sizeof(InfoHead);
    m_BMPHeader.blank = 0;
    m_BMPHeader.startPosition=sizeof(BmpHead) + sizeof(InfoHead);
    memcpy(pBMPData, &m_BMPHeader, sizeof(BmpHead));

    InfoHead  m_BMPInfoHeader;
    m_BMPInfoHeader.Length=40;
    m_BMPInfoHeader.width= nWidth;
    //ע�⣬�����bmpinfo.bmiHeader.biHeight��������������bmp�ļ��Ĵ洢��ʽ�����Ϊ��ֵ����ʾ�����ǵ�������
    m_BMPInfoHeader.height= -nHeight;
    m_BMPInfoHeader.colorPlane = 1;
    m_BMPInfoHeader.bitColor=24;
    m_BMPInfoHeader.zipFormat=0;
    m_BMPInfoHeader.realSize=3 * nWidth * nHeight;
    m_BMPInfoHeader.xPels=0;
    m_BMPInfoHeader.yPels=0;
    m_BMPInfoHeader.colorUse=0;
    m_BMPInfoHeader.colorImportant=0;
    memcpy(pBMPData+sizeof(BmpHead),&m_BMPInfoHeader,sizeof(InfoHead));
    memcpy(pBMPData+sizeof(BmpHead)+sizeof(InfoHead), pBuffer, nBufferLen);

    /// �ͷ����ݻ���
	delete [] pBuffer; pBuffer = NULL;
	delete [] pBMPData; pBMPData = NULL;
}

//---------------------------------------------------------------

void e_DisplayCallBack(JNIEnv *env, char* pszBuffer, int nSize)
{
	/*m_CallBackClass = env->FindClass("com/example/testffmpeg/CVideoFrames");
	if(NULL != m_CallBackClass)
	{
		/// �������еľ�̬����
		m_CallBackQueueMethod = env->GetStaticMethodID(m_CallBackClass, "e_AddDataToQueue", "([B)V");
		if(NULL != m_CallBackQueueMethod)
		{
		 	/// ��������
		 	jbyteArray bytecarr = env->NewByteArray(nSize);
		     /// ��������
		 	env->SetByteArrayRegion(bytecarr, 0, nSize, (jbyte*)pszBuffer);
		 	//�ص�java�еķ���
		 	env->CallStaticVoidMethod(m_CallBackClass, m_CallBackQueueMethod , bytecarr);
		}
	}*/

	if(NULL != m_CallBackClass && NULL != m_CallBackQueueMethod)
	{
	 	/// ��������
	 	jbyteArray bytecarr = env->NewByteArray(nSize);
	     /// ��������
	 	env->SetByteArrayRegion(bytecarr, 0, nSize, (jbyte*)pszBuffer);
	 	//�ص�java�еķ���
	 	env->CallStaticVoidMethod(m_CallBackClass, m_CallBackQueueMethod , bytecarr);
	 	/// �ͷű�����������
	 	env->DeleteLocalRef(bytecarr);
	}
}

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IGetVersion
 * Signature: ()I
 */
jint Java_com_example_testffmpeg_CFFmpegJni_IGetVersion(JNIEnv *env, jobject thiz)
{
	/// ������־��Ϣ�ص�
	av_log_set_callback(e_PrintFFmpeg);

	/// ��ӡ��־
	LOGD("Set FFMpeg Log Call Back Success. ");

	/// ��ȡ�汾��Ϣ����ʹ��
	int nRet = 0;
	nRet = avcodec_version();

	/// ����ص�����û�����ã���ô��ȡ�ص�����
	if(NULL == m_CallBackQueueMethod)
	{
	    //�ҵ�java�е���
		m_CallBackClass = env->FindClass("com/example/testffmpeg/CVideoFrames");
		/// JVM�ľֲ����øĳ�ȫ������
		m_CallBackClass = env->NewGlobalRef(m_CallBackClass);

		if(NULL != m_CallBackClass)
		{
			/// �������еľ�̬����
			m_CallBackQueueMethod = env->GetStaticMethodID(m_CallBackClass, "e_AddDataToQueue", "([B)V");
			/// JVM�ľֲ����øĳ�ȫ������
			m_CallBackQueueMethod = env->NewGlobalRef(m_CallBackQueueMethod);
		}
	}

	/// �жϻص������Ƿ��ȡ�ɹ�
    if(NULL == m_CallBackQueueMethod || NULL == m_CallBackClass)
    {
        LOGD("Get CallBack Function Fail.");
        return 0;
    }

	/// ���Իص�
	char szTemp[128];
	strcpy(szTemp, "����, ���ǻص���Ϣ");
	e_DisplayCallBack(env, szTemp, strlen(szTemp));
	return nRet;
}

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IInit
 * Signature: (Ljava/lang/String;IIII)I
 */
jint Java_com_example_testffmpeg_CFFmpegJni_IInit(JNIEnv *env, jobject thiz, jstring jstrRTSPUrl,
		jint jnMeidaType, jint jnSawle, jint jnWith, jint jnHeight)
{
	int nRet = 0;
	/// ע�������
	avcodec_register_all();

	const char* pstrRTSPUrl = (env)->GetStringUTFChars(jstrRTSPUrl, 0);

	/// ��ֵRTSP�����ַ
	strcpy(m_szURLPath, pstrRTSPUrl);

	LOGD("%s", m_szURLPath);

	m_nWith = jnWith;

	/// ����rtspΪTCP��ʽ
	if(1 == jnMeidaType)
	{
		av_dict_set(NULL, "rtsp_transport", "tcp", 0);
	}
	/// ��ʼ������
	nRet = avformat_network_init();
	return nRet;
}

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IResize
 * Signature: (II)V
 */
void Java_com_example_testffmpeg_CFFmpegJni_IResize(JNIEnv *env, jobject thiz, jint jnWith, jint jnHeight)
{

}

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IPlay
 * Signature: ()I
 */
jint Java_com_example_testffmpeg_CFFmpegJni_IPlay(JNIEnv *env, jobject thiz)
{
	/// ���巵��ֵ
	int nRet = -1;
	/// ���ļ�
	if(NULL == m_pFormatCtx)
	{
		m_pFormatCtx = avformat_alloc_context();
	}

	if(NULL != m_pFormatCtx)
	{
		/// ���ļ�
		nRet = avformat_open_input(&m_pFormatCtx, m_szURLPath, 0, NULL);
	}

	if(0 > avformat_find_stream_info(m_pFormatCtx, NULL))
	{
		LOGD("Couldn't find stream information.");
		return -1;
	}

	int i = 0, nVideoIndex = -1;
	for(i = 0; i < m_pFormatCtx->nb_streams; i++)
	{
		if(AVMEDIA_TYPE_VIDEO == m_pFormatCtx->streams[i]->codec->codec_type)
		{
			nVideoIndex = i;
			break;
		}
	}

	if(-1 == nVideoIndex)
	{
		LOGD("Didn't find a video stream.");
		return -1;
	}

	AVCodecContext* pCodecCtx = m_pFormatCtx->streams[nVideoIndex]->codec;
	AVCodec* pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL)
	{
		LOGD("Codec not found.");
		return -1;
	}
	if(0 > avcodec_open2(pCodecCtx, pCodec, NULL))
	{
		LOGD("Could not open codec.");
		return -1;
	}

	AVFrame	*pFrame = NULL, *pFrameYUV = NULL, *pFrameRGB = NULL;
	pFrame = avcodec_alloc_frame();
	pFrameYUV = avcodec_alloc_frame();
	pFrameRGB = avcodec_alloc_frame();
	uint8_t *out_buffer = NULL;
	out_buffer = new uint8_t[avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height)];
	avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

	int nCodecRet, got_picture;
	static struct SwsContext *img_convert_ctx;
	int y_size = pCodecCtx->width * pCodecCtx->height;

	AVPacket* packet = (AVPacket *)malloc(sizeof(AVPacket));
	av_new_packet(packet, y_size);
	/// ���һ����Ϣ-----------------------------
	LOGD("�ļ���Ϣ-----------------------------------------");
	av_dump_format(m_pFormatCtx, 0, m_szURLPath, 0);
	while(0 <= av_read_frame(m_pFormatCtx, packet))
	{
		if(nVideoIndex == packet->stream_index)
		{
			nCodecRet = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if(0 < nCodecRet)
			{
				/// ��ʽ������ΪYUV��ʽ
				/*img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width,
						pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize,
						0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);*/

				/// ��ʽ�����ظ�ʽΪBMP
				img_convert_ctx = sws_getContext(	pCodecCtx->width,
						pCodecCtx->height,
						pCodecCtx->pix_fmt,
						pCodecCtx->width,
						pCodecCtx->height,
						PIX_FMT_RGB32,
						SWS_BICUBIC,
						NULL, NULL, NULL);
				/// ת����ʽΪRGB
				sws_scale(img_convert_ctx,
						(const uint8_t* const*)pFrame->data,
						pFrame->linesize,
						0,
						pCodecCtx->height,
						pFrameRGB->data,
						pFrameRGB->linesize);

				/// �ͷŸ���ʽ����Ϣ
				sws_freeContext(img_convert_ctx);


				e_SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height);
			}
			else
			{
				LOGD("�������-------------------------->");
				return -1;
			}
		}
		/// �ͷŽ����
		av_free_packet(packet);
	}

	delete[] out_buffer; out_buffer = NULL;
	av_free(pFrame); pFrame = NULL;
	av_free(pFrameYUV); pFrameYUV = NULL;
	av_free(pFrameRGB); pFrameRGB = NULL;
	avcodec_close(pCodecCtx); pCodecCtx = NULL;
	avformat_close_input(&m_pFormatCtx);
	/// �ͷ�����
	av_free(m_pFormatCtx);
	m_pFormatCtx = NULL;
	return nRet;
}

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IStop
 * Signature: ()I
 */
jint Java_com_example_testffmpeg_CFFmpegJni_IStop(JNIEnv *env, jobject thiz)
{
	/// �رմ򿪵��ļ�
	avformat_close_input(&m_pFormatCtx);
	/// �ͷ�����
	av_free(m_pFormatCtx);
	m_pFormatCtx = NULL;
	return 0;
}
