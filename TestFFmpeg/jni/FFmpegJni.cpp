#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/log.h"

#ifdef __cplusplus
}
#endif// end of extern C

#include "FFmpegJni.h"

#include <android/log.h>

#define FFMPEG_TAG "FFMpegAndroid"

#define MYLOG_TAG "TestNDK_DEBUG"

/// ����ȫ�ֱ���
AVFormatContext* m_pFormatCtx = NULL;
AVDictionary* m_pDOptions = NULL;

char m_szURLPath[512];
int m_nWith = 0;

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


void SaveFrame(AVFrame* pFrame, int width, int height, int iFrame)
{
    FILE *pFile;
    char szFilename[32];
    int  y;

	/////////////////////
	BITMAPFILEHEADER m_fileHeader;
	BITMAPINFOHEADER m_infoHeader;
	unsigned int uiTmp, uiTmp2;
	unsigned char *ucTmp = NULL;
	unsigned char ucRGB;
	int i;

	uiTmp = (width*3+3)/4*4*height;
	uiTmp2 = width*height*3;


	//�ļ���ʶ"BM"����0x4D42����ʾλͼ
	m_fileHeader.bfType = 0x4D42;
	//�����ļ��Ĵ�С����λ���ֽڣ�
	m_fileHeader.bfSize = sizeof(m_fileHeader) + sizeof(m_infoHeader) + uiTmp;
	//����������Ϊ0
	m_fileHeader.bfReserved1 = 0;
	//����������Ϊ0
	m_fileHeader.bfReserved2 = 0;
	//���ļ���ʼ��λͼ���ݵ�ƫ��������λ���ֽڣ�
	m_fileHeader.bfOffBits = sizeof(m_fileHeader) + sizeof(m_infoHeader);

	//��Ϣͷ���ȣ���λ���ֽڣ�������ֵΪ28
	m_infoHeader.biSize = 0x28;
	//λͼ��ȣ���λ�����أ�
	m_infoHeader.biWidth = width;
	//λͼ�߶ȣ���λ�����أ�������Ϊ������ʾ�����λͼ����Ϊ������ʾ�����λͼ
	m_infoHeader.biHeight = height;
	//λͼ��������Ϊ1��
	m_infoHeader.biPlanes = 1;
	//ÿ�����ص�λ��
	m_infoHeader.biBitCount = 24;
	//ѹ��˵����0(BI_RGB)��ʾ��ѹ��
	m_infoHeader.biCompression = 0;
	//���ֽ�����ʾ��λͼ���ݵĴ�С��Ϊ4��λ����
	m_infoHeader.biSizeImage = uiTmp;
	//ˮƽ�ֱ��ʣ���λ������/�ף�
	m_infoHeader.biXPelsPerMeter = 0;
	//��ֱ�ֱ��ʣ���λ������/�ף�
	m_infoHeader.biYPelsPerMeter = 0;
	//λͼʹ�õ���ɫ��
	m_infoHeader.biClrUsed = 0;
	//��Ҫ����ɫ��
	m_infoHeader.biClrImportant = 0;
	/////////////////////

    // Open file
	sprintf(szFilename, "frame%d.bmp", iFrame);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
        return;
    // Write header
	fprintf(pFile, "P6\n%d %d\n255\n", width, height);
    // Write pixel data
	fwrite(&m_fileHeader, sizeof(m_fileHeader), 1, pFile);
	fwrite(&m_infoHeader, sizeof(m_infoHeader), 1, pFile);
    for(y=height-1; y>=0; y--)
    {
		if(ucTmp != NULL) {
			delete []ucTmp;
			ucTmp = NULL;
		}
		ucTmp = new unsigned char[width*3];
		memcpy(ucTmp, pFrame->data[0]+y*pFrame->linesize[0], width*3);
		for(i = 0; i < width; i++) {
			ucRGB = ucTmp[3*i];
			ucTmp[3*i] = ucTmp[3*i+2];
			ucTmp[3*i+2] = ucRGB;
		}
		ucRGB = 0;
        fwrite(ucTmp, 1, width*3, pFile);
		fwrite(&ucRGB, 1, (uiTmp-uiTmp2)/height , pFile);
	}
    // Close file
    // fclose(pFile);
}

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

	const char* pstrRTSPUrl = (env)->GetStringUTFChars(jstrRTSPUrl, 0);

	/// ��ֵRTSP�����ַ
	strcpy(m_szURLPath, pstrRTSPUrl);

	LOGD("%s", m_szURLPath);

	m_nWith = jnWith;

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

		if(0 > avformat_find_stream_info(m_pFormatCtx, &m_pDOptions))
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
		if(0 > avcodec_open2(pCodecCtx, pCodec, &m_pDOptions))
		{
			LOGD("Could not open codec.");
			return -1;
		}

		AVFrame	*pFrame,*pFrameYUV;
		pFrame = avcodec_alloc_frame();
		pFrameYUV = avcodec_alloc_frame();
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

		while(av_read_frame(m_pFormatCtx, packet)>=0)
		{
			if(nVideoIndex == packet->stream_index)
			{
				nCodecRet = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
				if(0 < nCodecRet)
				{
					/// ��ʽ������ΪYUV��ʽ
					img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width,
							pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
					sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize,
							0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);


					/*img_convert_ctx = sws_getContext(	pAVCodecCtx->width,
														pAVCodecCtx->height,
														pAVCodecCtx->pix_fmt,
														pAVCodecCtx->width,
														pAVCodecCtx->height,
														PIX_FMT_RGB24,
														SWS_BICUBIC,
														NULL, NULL, NULL);
					if(img_convert_ctx == NULL)
						LOGE("could not initialize conversion context\n");

					sws_scale(	img_convert_ctx,
								(const uint8_t* const*)pAVFrame->data,
								pAVFrame->linesize,
								0,
								pAVCodecCtx->height,
								pAVFrameRGB->data,
								pAVFrameRGB->linesize);*/
				}
				else
				{
					LOGD("�������-------------------------->");
					return -1;
				}
			}
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
