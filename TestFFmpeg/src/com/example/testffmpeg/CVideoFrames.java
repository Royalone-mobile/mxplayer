package com.example.testffmpeg;

import java.util.concurrent.ConcurrentLinkedQueue;

import android.util.Log;

public class CVideoFrames {
	
	/// �������ݶ���
	public static ConcurrentLinkedQueue<byte[]>  m_FrameDatas = new ConcurrentLinkedQueue<byte[]>();
	
	/***
	 * ��̬���� FFmpegJni �ص��˷���������Ҫ��ʾ������֡������
	 * @param pBufferData
	 */
	public static void e_AddDataToQueue(byte[] pBufferData)
	{
		if(null != pBufferData)
		{			
			m_FrameDatas.add(pBufferData);
			/// ��ӡ��־
			Log.d("e_AddDataToQueue", "Data Input To Queue Success! BufferSize = " + 
					pBufferData.length + " FramQueue Size = " + m_FrameDatas.size());
		}
	}
}
