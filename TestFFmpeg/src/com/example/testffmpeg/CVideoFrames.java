package com.example.testffmpeg;

import java.util.concurrent.ConcurrentLinkedQueue;

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
			/// ����Ҫ���Ƶ����ݵ�Buffer
			m_FrameDatas.add(pBufferData);
		}
	}
}
