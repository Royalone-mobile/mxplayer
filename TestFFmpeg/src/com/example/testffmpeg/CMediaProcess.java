package com.example.testffmpeg;

import android.util.Log;

public class CMediaProcess implements Runnable{
	
	/// ���峣����������
	public static final int Net_UDP_Type = 0;
	public static final int Net_TCP_Type = 1;	
	/// ����Ҫ��ȡ�Ļ�ȡFFmpeg��������
	private CFFmpegJni m_FFMpegJni = null;
	/// ���岥��״̬
	private boolean m_bIsPlaying = false;
	/// ��������̶߳���
	private Thread m_DecoderTread = null;
	
	public CMediaProcess(CFFmpegJni FFmpegJni)
	{
		/// ��ֵFFmepg������
		m_FFMpegJni = FFmpegJni;
	}
	
	public int e_Init(String strRTSPUrl, int nMediaType)
	{
		/// ��ʼ��ý�����
		return m_FFMpegJni.IInit(strRTSPUrl, nMediaType);
	}
	
	public void e_IResize(int nDecodeWidth, int nDecodeHeight)
	{
		/// ������ʾ����
		m_FFMpegJni.IResize(nDecodeWidth, nDecodeHeight);
	}

	public void e_Start()
	{
		/// �������״̬Ϊֹͣ����״̬������ý���ļ�
		if(false == m_bIsPlaying)
		{
			/// ��ֵ���ڲ���״̬
			this.m_bIsPlaying = true;
			/// �����߳�����
			m_DecoderTread = new Thread(this);
			m_DecoderTread.start();
		}
	}
	
	public void e_Stop()
	{
		/// ������ڲ���
		if(true == m_bIsPlaying)
		{
			/// ����Jniֹͣ������Ƶ
			m_FFMpegJni.IStop();
			/// ��ֵ����״̬Ϊ��δ����״̬
			m_bIsPlaying = false;
			/// �����߳�����ڲ��Ź����У�ֹͣ�߳�����
			if(null != m_DecoderTread && m_DecoderTread.isAlive())
			{
				Log.d("Thread Test", "T Alive --------------------");
				m_DecoderTread.interrupt();
			}			
			/// �������֡����
			CVideoFrames.m_FrameDatas.clear();
		}
	}

	@Override
	public void run()
	{
		/// ��ʼ����
		m_FFMpegJni.IPlay();
		m_bIsPlaying = false;
	}
}