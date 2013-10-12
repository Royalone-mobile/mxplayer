/**
 * ˵��: ͨ��FFmpeg����ý���ļ�����󱣴��Bmpͼ��.
 * FFmpegʹ�õ���LibX264-114.dll
 * FFmpeg SDK �汾0.6.1
 * ����: ��.
 */
#include <string>
#include <cassert>
#include <iostream>
#include <sstream>
#include <tchar.h>
#include <Windows.h>
extern "C"
{
#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
};
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "swscale.lib")
AVFrame *alloc_picture(enum PixelFormat pix_fmt, int width, int height);
BOOL CreateBmp(const char *filename, uint8_t *pRGBBuffer, int width, int height, int bpp) ;
int _tmain(int argc, _TCHAR* argv[])
{
 int iResult = 0;
 //* ע��
 av_register_all();
 //* �ļ���.
 std::string strFile = "e:\\����\\13587���ĸ��Ƭ��__016.mpg";
 //* ���ļ�
 AVFormatContext* pavFmtCxt;
 iResult = av_open_input_file(&pavFmtCxt, strFile.c_str(),
  NULL, 0, NULL);
 assert(iResult == 0);
 iResult = av_find_stream_info(pavFmtCxt);
 assert(iResult >= 0);
 int iVidStrmID = -1;
 for (int i = 0; i < pavFmtCxt->nb_streams; ++i)
 {
  if (CODEC_TYPE_VIDEO == 
   pavFmtCxt->streams[i]->codec->codec_type)
  {
   iVidStrmID = i;
  }
 }
 assert(iVidStrmID != -1);
 //* ����,�򿪽�����.
 AVCodec* pDecodec = avcodec_find_decoder(
  pavFmtCxt->streams[iVidStrmID]->codec->codec_id);
 iResult = avcodec_open(pavFmtCxt
  ->streams[iVidStrmID]->codec, pDecodec);
 assert(iResult >= 0);
 dump_format(pavFmtCxt, iVidStrmID, strFile.c_str(), 0);
 //* ��ȡ�ļ�,����.
 AVFrame* pFrame = avcodec_alloc_frame();
 AVPacket pkt;
 av_init_packet(&pkt);
 //* Seek
 //av_seek_frame(pavFmtCxt, 0, 493, AVSEEK_FLAG_FRAME);
 while (av_read_frame(pavFmtCxt, &pkt)>= 0)
 {
  if (pkt.stream_index == iVidStrmID)
  {
   int iFinished = 0;
   AVCodecContext* pavCCxt = NULL;
   pavCCxt = pavFmtCxt->streams[iVidStrmID]->codec;
   int iDecoded = avcodec_decode_video(pavCCxt, pFrame, 
    &iFinished, pkt.data,pkt.size);
   if (iDecoded > 0 && iFinished)
   {
    std::ostringstream ostrm;
    //* ����ɹ�.���PTS,
    ostrm<<"pts_"
     <<pkt.pts//<<pavFmtCxt->streams[iVidStrmID]->pts_buffer[0]
    <<"\n";
    OutputDebugStringA(ostrm.str().c_str());
    int width, height;
    width = pavFmtCxt->streams[iVidStrmID]->codec->width;
    height = pavFmtCxt->streams[iVidStrmID]->codec->height;
    //* ��YUV420Pת����RGB32.
    SwsContext* pSwsCxt = sws_getContext(width,
     height,
     PIX_FMT_YUV420P,
     width,
     height,
     PIX_FMT_RGB32,
     SWS_BILINEAR, NULL, NULL, NULL);
    uint8_t *rgb_data = static_cast<uint8_t*>(av_malloc(width*height*4));
    uint8_t *rgb_src[3]= {rgb_data, NULL, NULL};
    int rgb_stride[3]={4*width, 0, 0};
    assert(pSwsCxt);
    iResult = sws_scale(pSwsCxt, pFrame->data, pFrame->linesize,
     0, height, rgb_src, rgb_stride);
    assert(iResult == height);
    /* {{ ���Դ���,RGB32,YUV420֮���ת��.
    //* ��RGB32ת��ΪYUV420P
    AVFrame* pYUVFrm = alloc_picture(PIX_FMT_YUV420P, width, height);
    SwsContext* pSwsCxtYUV = sws_getContext(width, height, PIX_FMT_RGB32,
     width, height,
     PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);//* ע��Flag��ֵ.
    iResult = sws_scale(pSwsCxtYUV, rgb_src, rgb_stride,
     0, height, pYUVFrm->data, pYUVFrm->linesize);
    assert(iResult == height);
    
    //* ��ת����RGB32
    ::memset(rgb_data, 0, width*height*4);
    iResult = sws_scale(pSwsCxt, pYUVFrm->data, pYUVFrm->linesize,
     0, height, rgb_src, rgb_stride);
    assert(iResult == height);
    //* }} */
    char sz[100];
    itoa(pkt.pts, sz, 10);
    CreateBmp(sz, rgb_data, width, height, 32);
    ::memset(rgb_data, 0, width*height*4);
    av_freep(&rgb_data);
    //* ע��SwsContext��������������ͷ�.
    sws_freeContext(pSwsCxt);
    /* {{ ���Դ���, ��������������.������ڴ�й©.
    sws_freeContext(pSwsCxtYUV);
    av_free(pYUVFrm->data[0]);
    av_free(pYUVFrm);
    pYUVFrm = NULL;
    //* }} */
   }
   else
   {
    ::OutputDebugStringA("����ʧ��");
   }
  }
 }
 return 0;
}
BOOL CreateBmp(const char *filename, uint8_t *pRGBBuffer, int width, int height, int bpp) 
{ 
 BITMAPFILEHEADER bmpheader; 
 BITMAPINFO bmpinfo; 
 FILE *fp = NULL;
 fp = fopen(filename,"wb"); 
 if( fp == NULL ) 
 { 
  return FALSE; 
 }
 bmpheader.bfType = ('M' <<8)|'B'; 
 bmpheader.bfReserved1 = 0; 
 bmpheader.bfReserved2 = 0; 
 bmpheader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); 
 bmpheader.bfSize = bmpheader.bfOffBits + width*height*bpp/8;
 bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
 bmpinfo.bmiHeader.biWidth = width; 
 bmpinfo.bmiHeader.biHeight = 0 - height; 
 bmpinfo.bmiHeader.biPlanes = 1; 
 bmpinfo.bmiHeader.biBitCount = bpp; 
 bmpinfo.bmiHeader.biCompression = BI_RGB; 
 bmpinfo.bmiHeader.biSizeImage = 0; 
 bmpinfo.bmiHeader.biXPelsPerMeter = 100; 
 bmpinfo.bmiHeader.biYPelsPerMeter = 100; 
 bmpinfo.bmiHeader.biClrUsed = 0; 
 bmpinfo.bmiHeader.biClrImportant = 0;
 fwrite(&bmpheader,sizeof(BITMAPFILEHEADER),1,fp); 
 fwrite(&bmpinfo.bmiHeader,sizeof(BITMAPINFOHEADER),1,fp); 
 fwrite(pRGBBuffer,width*height*bpp/8,1,fp); 
 fclose(fp); 
 fp = NULL;
 return TRUE; 
}
static AVFrame *alloc_picture(enum PixelFormat pix_fmt, int width, int height)
{
 AVFrame *picture;
 uint8_t *picture_buf;
 int size;
 picture = avcodec_alloc_frame();
 if (!picture)
  return NULL;
 size = avpicture_get_size(pix_fmt, width, height);
 picture_buf = (uint8_t*)av_malloc(size);
 if (!picture_buf) {
  av_free(picture);
  return NULL;
 }
 avpicture_fill((AVPicture *)picture, picture_buf,
  pix_fmt, width, height);
 return picture;
}