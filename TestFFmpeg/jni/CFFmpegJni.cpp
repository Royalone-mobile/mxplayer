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

#define FFMPEG_TAG "FFMPEG_TAG"
#define MYLOG_TAG "TestNDK_DEBUG"

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)

/// ����ȫ�ֱ���
AVFormatContext* m_pFormatCtx = NULL;

AVDictionary* m_pDictOptions = NULL;

bool m_bIsPlaying = false;

int nDecodeNum = 0;

char m_szURLPath[512];
int m_nWith = 0;

static jclass		m_CallBackClass = NULL;
static jmethodID 	m_CallBackQueueMethod = NULL;

/// ������־�ص�����
static void e_PrintFFmpeg(void* ptr, int level, const char* fmt, va_list vl)
{
	switch(level)
	{
	case AV_LOG_PANIC:
		__android_log_print(ANDROID_LOG_ERROR, FFMPEG_TAG, fmt, vl);
		break;

	case AV_LOG_FATAL:
		__android_log_print(ANDROID_LOG_ERROR, FFMPEG_TAG, fmt, vl);
		break;

	case AV_LOG_ERROR:
		__android_log_print(ANDROID_LOG_ERROR, FFMPEG_TAG, fmt, vl);
		break;

	case AV_LOG_WARNING:
		__android_log_print(ANDROID_LOG_ERROR, FFMPEG_TAG, fmt, vl);
		break;

	case AV_LOG_INFO:
		__android_log_print(ANDROID_LOG_INFO, FFMPEG_TAG, fmt, vl);
		break;

	case AV_LOG_DEBUG:
		__android_log_print(ANDROID_LOG_DEBUG, FFMPEG_TAG, fmt, vl);
		break;
	}
}

/// �����ӡ��Ϣ
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, MYLOG_TAG, __VA_ARGS__)


/****
 * �ص����ݷ���
 */
void e_DisplayCallBack(JNIEnv *env, BYTE* pszBuffer, int nSize)
{
	m_CallBackClass = env->FindClass("com/example/testffmpeg/CVideoFrames");
	if(NULL != m_CallBackClass)
	{
		/// �������еľ�̬����
		m_CallBackQueueMethod = env->GetStaticMethodID(m_CallBackClass, "e_AddDataToQueue", "([B)V");
		if(NULL != m_CallBackQueueMethod)
		{
			LOGD("Call BACK Data0..................");
		 	/// ��������
		 	jbyteArray byteArray = env->NewByteArray(nSize);
		    /// ��������
		 	env->SetByteArrayRegion(byteArray, 0, nSize, (jbyte* )pszBuffer);

		 	LOGD("Call BACK Data1..................");

		 	//�ص�java�еķ���
		 	env->CallStaticVoidMethod(m_CallBackClass, m_CallBackQueueMethod , byteArray);
		 	/// �ͷű�����������
		 	env->DeleteLocalRef(byteArray);
		 	LOGD("Call BACK Data2..................");
		}
	}
}

void e_SaveFrame(JNIEnv *env, AVFrame* pFrameRGB, int nWidth, int nHeight)
{
	/// ��ȡ��ȵ�����
	int nWidthBytes = nWidth * 4;/// WIDTHBYTES(nWidth * 24);
	int nBufferLen =  nWidthBytes * nHeight;

	LOGD("%s, DateLen = %d", " SaveFrame 0--------------->", nBufferLen);
	int nBMPHeadLen = sizeof(BmpHead) + sizeof(InfoHead);
	int nBMPBufferLen = nBMPHeadLen + nBufferLen;
	BYTE* pBMPData = new BYTE[nBMPBufferLen];
	memset(pBMPData, 0x00, nBMPBufferLen);
	/// ��ȡ�������ݲ��ֵ�ָ��
	BYTE* pBuffer = pBMPData + nBMPHeadLen;
	/// ��ͼ��תΪbmp�浽�ڴ���, �����ͼ���ǵ�����
	for(int i = 0; i < nHeight; i++)
	{
		memcpy(pBuffer + nWidthBytes * i, pFrameRGB->data[0] + i * pFrameRGB->linesize[0], nWidthBytes);
	}

	/// ��ֵBMPͼƬͷ
    BmpHead m_BMPHeader;
    m_BMPHeader.bfType = ('M' << 8) | 'B';
    /// memcpy(m_BMPHeader.bfType, "BM", sizof(m_BMPHeader.bfType));
    m_BMPHeader.imageSize = nBMPBufferLen;
    m_BMPHeader.blank = 0;
    m_BMPHeader.startPosition = sizeof(BmpHead) + sizeof(InfoHead);
    memcpy(pBMPData, &m_BMPHeader, sizeof(BmpHead));

    /// ��ֵBMPͼƬ��Ϣͷ
    InfoHead  m_BMPInfoHeader;
    m_BMPInfoHeader.Length = sizeof(InfoHead);
    m_BMPInfoHeader.width = nWidth;
    /// = ע�⣬�����bmpinfo.bmiHeader.biHeight ����������
    /// = ����bmp�ļ��Ĵ洢��ʽ�����Ϊ��ֵ����ʾ�����ǵ�������
    m_BMPInfoHeader.height = -nHeight;
    m_BMPInfoHeader.colorPlane = 1;
    m_BMPInfoHeader.bitColor = 32;
    m_BMPInfoHeader.zipFormat = 0;
    m_BMPInfoHeader.realSize = nBufferLen;
    m_BMPInfoHeader.xPels = 0;
    m_BMPInfoHeader.yPels = 0;
    m_BMPInfoHeader.colorUse = 0;
    m_BMPInfoHeader.colorImportant = 0;
    memcpy(pBMPData + sizeof(BmpHead), &m_BMPInfoHeader, sizeof(InfoHead));

    LOGD("SaveFrame 1--------------->");
	/// �ص����ݵ�Java
	/// e_DisplayCallBack(env, pBMPData, nBMPBufferLen);

	FILE* pFile = NULL;
	char szFilename[32];
	int y;
	sprintf(szFilename, "/mnt/sdcard/Frame%d.bmp", nDecodeNum);
	pFile = fopen(szFilename, "wb");
	if(NULL == pFile)
	{
		return;
	}
	fwrite(pBMPData, 1, nBMPBufferLen, pFile);
	fclose(pFile);
	LOGD("SaveFrame 2--------------->");
    /// �ͷ����ݻ���
	delete[] pBMPData;
	pBMPData = NULL;
	LOGD("SaveFrame 3--------------->");
}

/*
 * Class:     com_example_testffmpeg_CFFmpegJni
 * Method:    IGetVersion
 * Signature: ()I
 */
jint Java_com_example_testffmpeg_CFFmpegJni_IGetVersion(JNIEnv *env, jobject thiz)
{
	/// ����FFmpeg��־��Ϣ�ص�
	av_log_set_callback(e_PrintFFmpeg);
	LOGD("Set FFMpeg Log Call Back Success. ");

	/// ��ȡ�汾��Ϣ����ʹ��
	int nRet = 0;
	nRet = avcodec_version();
	/// ���Իص�
	char szTemp[128];
	strcpy(szTemp, "����, ���ǻص���Ϣ");
	e_DisplayCallBack(env, (BYTE* )szTemp, strlen(szTemp));
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
	/// ע�������
	av_register_all();

	const char* pstrRTSPUrl = (env)->GetStringUTFChars(jstrRTSPUrl, 0);
	/// ��ֵRTSP�����ַ
	strcpy(m_szURLPath, pstrRTSPUrl);

	LOGD("%s", m_szURLPath);

	m_nWith = jnWith;
	/// ��ʼ������
	nRet = avformat_network_init();

	/// ����rtspΪTCP��ʽ
	if(1 == jnMeidaType)
	{
		LOGD("%s, %s", m_szURLPath, "1--------------->");
		av_dict_set(&m_pDictOptions, "rtsp_transport", "tcp", 0);
		LOGD("%s, %s", m_szURLPath, "2--------------->");
	}

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
	if(NULL != m_pFormatCtx)
	{
		avformat_close_input(&m_pFormatCtx);
		/// �ͷ�����
		av_free(m_pFormatCtx);
		m_pFormatCtx = NULL;
	}

	if(NULL == m_pFormatCtx && NULL != m_pDictOptions)
	{
		/// ���ļ�
		LOGD("%s, %s", m_szURLPath, "3--------------->");
		if(0 != (nRet = avformat_open_input(&m_pFormatCtx, m_szURLPath, 0, &m_pDictOptions)))
		{
			char szTemp[256];
			memset(szTemp, 0x00, sizeof(szTemp));
			av_strerror(nRet, szTemp, 255);

			LOGD("%s, Error Code = %d, %s, Error = %s", m_szURLPath, nRet, " The Error URL Or Path--------------->", szTemp);
			return nRet;
		}
		LOGD("%s, %s", m_szURLPath, "4--------------->");
	}

	// m_pFormatCtx->max_analyze_duration = 1000;

	// m_pFormatCtx->probesize = 2048;

	/// av_set_int(m_pFormatCtx, "probesize", );

	LOGD("%s", "Test1--------------->");
	if(0 > avformat_find_stream_info(m_pFormatCtx, &m_pDictOptions))
	{
		LOGD("Couldn't find stream information.");
		return -1;
	}
	LOGD("%s", "Test2--------------->");

	LOGD("%s", "Test3--------------->");
	int i = 0, nVideoIndex = -1;
	for(i = 0; i < m_pFormatCtx->nb_streams; i++)
	{
		if(AVMEDIA_TYPE_VIDEO == m_pFormatCtx->streams[i]->codec->codec_type)
		{
			nVideoIndex = i;
			break;
		}
	}
	LOGD("%s", "Test4--------------->");

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

	LOGD("Test8 Width:%d  Height:%d ------------>", pCodecCtx->width,pCodecCtx->height);
	/// if(0 > avcodec_open2(pCodecCtx, pCodec, &m_pDictOptions))
	if(0 > avcodec_open2(pCodecCtx, pCodec, &m_pDictOptions))
	{
		LOGD("Could not open codec.");
		return -1;
	}
	LOGD("Test9 Width:%d  Height:%d ------------>", pCodecCtx->width,pCodecCtx->height);
	/// ��������֡����
	AVFrame	*pFrame = NULL, *pFrameRGB = NULL;
	pFrame = avcodec_alloc_frame();
	pFrameRGB = avcodec_alloc_frame();

	/// ��������֡����
	uint8_t* pPicbuffer = NULL;
	pPicbuffer = new uint8_t[avpicture_get_size(PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height)];
	avpicture_fill((AVPicture *)pFrameRGB, pPicbuffer, PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height);

	/// �����������
	int nCodecRet, nHasGetPicture;
	/// ������ʽת������
	static struct SwsContext* img_convert_ctx = NULL;

	/// ��������֡�������ݰ�
	int nPackgeSize  = pCodecCtx->width * pCodecCtx->height;
	AVPacket* pAVPacket = (AVPacket *)malloc(sizeof(AVPacket));
	av_new_packet(pAVPacket, nPackgeSize);

	/// �г�����ļ����������Ϣ-----------------------------
	av_dump_format(m_pFormatCtx, 0, m_szURLPath, 0);

	/// ���ò���״̬
	m_bIsPlaying = true;

	/// ��ȡ���ݰ�
	while(0 <= av_read_frame(m_pFormatCtx, pAVPacket) && true == m_bIsPlaying)
	{
		/// �ж��Ƿ�����Ƶ������
		if(nVideoIndex == pAVPacket->stream_index)
		{
			/// �������ݰ�
			nCodecRet = avcodec_decode_video2(pCodecCtx, pFrame, &nHasGetPicture, pAVPacket);
			if(0 < nCodecRet)
			{
				LOGD("Num:%d  Width:%d  Height:%d StreamNum:%d ", nDecodeNum++, pCodecCtx->width, pCodecCtx->height,
						pFrame->coded_picture_number);
				/// ��ʽ�����ظ�ʽΪBMP
				img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
						pCodecCtx->width, pCodecCtx->height, PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
				/// ת����ʽΪRGB
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize,
						0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
				/// �ͷŸ���ʽ����Ϣ
				sws_freeContext(img_convert_ctx);
				/// ��ʾ���߱�������
				e_SaveFrame(env, pFrameRGB, pCodecCtx->width, pCodecCtx->height);
			}
		}
		/// �ͷŽ�����������ݰ����� av_read_frame �����Ǳ�
		/// av_free_packet(packet);
	}

	/// �ͷ�ͼƬ���ݻ���
	delete[] pPicbuffer;
	pPicbuffer = NULL;
	/// �ͷ�����֡����ָ��
	av_free(pFrame); pFrame = NULL;
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
	/// ��ֵֹͣ���ű�ʶ
	m_bIsPlaying = false;
	return 0;
}
