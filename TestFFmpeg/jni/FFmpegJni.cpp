#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "CRGB2BMP.h"

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

#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)

/// 定义全局变量
AVFormatContext* m_pFormatCtx = NULL;
AVDictionary* m_pDOptions = NULL;

char m_szURLPath[512];
int m_nWith = 0;

/// 定义日志回调函数
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

/// 定义打印信息
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
		//将图像转为bmp存到内存中, 这里的图像是倒立的
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
    //注意，这里的bmpinfo.bmiHeader.biHeight变量的正负决定bmp文件的存储方式，如果为负值，表示像素是倒过来的*/
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

    /// 释放数据缓冲
	delete [] pBuffer; pBuffer = NULL;
	delete [] pBMPData; pBMPData = NULL;
}

/*
 * Class:     com_example_testffmpeg_FFmpegJni
 * Method:    IGetVersion
 * Signature: ()I
 */
jint Java_com_example_testffmpeg_FFmpegJni_IGetVersion(JNIEnv *env, jobject thiz)
{
	/// 设置日志信息回调
	av_log_set_callback(e_PrintFFmpeg);

	/// 打印日志
	LOGD("Set FFMpeg Log Call Back Success. ");

	/// 获取版本信息测试使用
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
	/// 注册解码器
	avcodec_register_all();

	const char* pstrRTSPUrl = (env)->GetStringUTFChars(jstrRTSPUrl, 0);

	/// 赋值RTSP网络地址
	strcpy(m_szURLPath, pstrRTSPUrl);

	LOGD("%s", m_szURLPath);

	m_nWith = jnWith;

	/// 设置rtsp为TCP方式
	if(1 == jnMeidaType)
	{
		av_dict_set(&m_pDOptions, "rtsp_transport", "tcp", 0);
	}
	/// 初始化网络
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
	/// 定义返回值
	int nRet = -1;
	/// 打开文件
	if(NULL == m_pFormatCtx)
	{
		m_pFormatCtx = avformat_alloc_context();

		if(NULL != m_pFormatCtx && NULL != m_pDOptions)
		{
			/// 打开文件
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

		AVFrame	*pFrame = NULL, *pFrameYUV = NULL, *pFrameRGB = NULL;
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
		/// 输出一下信息-----------------------------
		LOGD("文件信息-----------------------------------------");
		av_dump_format(m_pFormatCtx, 0, m_szURLPath, 0);

		while(av_read_frame(m_pFormatCtx, packet)>=0)
		{
			if(nVideoIndex == packet->stream_index)
			{
				nCodecRet = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
				if(0 < nCodecRet)
				{
					/// 格式化像素为YUV格式
					/*img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width,
							pCodecCtx->height, PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
					sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize,
							0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);*/

					/// 格式化像素格式为BMP
					img_convert_ctx = sws_getContext(	pCodecCtx->width,
							pCodecCtx->height,
							pCodecCtx->pix_fmt,
							pCodecCtx->width,
							pCodecCtx->height,
							PIX_FMT_RGB32,
							SWS_BICUBIC,
							NULL, NULL, NULL);
					if(NULL == img_convert_ctx)
					{
						LOGD("could not initialize conversion context.");
					}

					/// 转换格式为RGB
					sws_scale(img_convert_ctx,
							(const uint8_t* const*)pFrame->data,
							pFrame->linesize,
							0,
							pCodecCtx->height,
							pFrameRGB->data,
							pFrameRGB->linesize);

					/// 释放个格式化信息
					sws_freeContext(img_convert_ctx);


					e_SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height);
				}
				else
				{
					LOGD("解码错误-------------------------->");
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
	/// 关闭打开的文件
	avformat_close_input(&m_pFormatCtx);
	return 0;
}
