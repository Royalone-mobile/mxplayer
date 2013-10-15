package com.example.testffmpeg;

import android.util.Log;

public class CMediaProcess extends Thread{
	
	/// ���峣����������
	public static final int Net_UDP_Type = 0;
	public static final int Net_TCP_Type = 1;
	
	/// ����Ҫ��ȡ�Ļ�ȡFFmpeg��������
	private CFFmpegJni m_FFMpegJni = null;
	/// ���岥��״̬
	private boolean m_bIsPlaying = false;
	
	public CMediaProcess(CFFmpegJni FFmpegJni)
	{
		/// ��ֵFFmepg������
		m_FFMpegJni = FFmpegJni;
	}
	
	public int e_Init(String strRTSPUrl, int nMediaType, int nScale, int nWidth, int nHeight)
	{
		/// ��ʼ��ý�����
		return m_FFMpegJni.IInit(strRTSPUrl, nMediaType, nScale, nWidth, nHeight);
	}

	public void e_Start()
	{
		/// �������״̬Ϊֹͣ����״̬������ý���ļ�
		if(false == m_bIsPlaying)
		{
			/// ��ֵ���ڲ���״̬
			this.m_bIsPlaying = true;
			/// �����߳�����
			this.start();
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
			if(this.isAlive())
			{
//				try {
//					Thread.sleep(100);
//				} catch (InterruptedException e) {
//					// TODO Auto-generated catch block
//					e.printStackTrace();
//				}
				Log.d("Thread Test", "T Alive --------------------");
				this.interrupt();
			}			
			/// �������֡����
			CVideoFrames.m_FrameDatas.clear();
		}
	}

	@Override
	public void run()
	{
		Log.d("Thread Test", "T Start--------------------");
		/// ��ʼ����
		m_FFMpegJni.IPlay();
		
		/*while(true == m_bIsPlaying)
		{
			try
			{
				Thread.sleep(1000);
			}
			catch (InterruptedException e) 
			{
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			Log.d("Thread Test", "T Runing --------------------");
		}*/
		
		Log.d("Thread Test", "T Stop--------------------");
		
		/// m_bIsPlaying = false;
	}
	
	
}
