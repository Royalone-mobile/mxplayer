package com.example.testffmpeg;

public class CFFmpegJni {
	
	/***
	 * ��ȡ�汾��
	 * @return
	 */
	public native int IGetVersion();
	
	/***
	 * ��ʼ��������������������ý�������
	 * @param strRTSPUrl
	 * @param nMediaType
	 * @param nScale
	 * @param nwidth
	 * @param nHeight
	 * @return
	 */
	public native int IInit(String strRTSPUrl, int nMediaType);

	/***
	 * ��������С
	 * @param nWidth
	 * @param nHeight
	 */
	public native void IResize(int nWidth, int nHeight);
	
	/***
	 * ��ʼ����
	 */
	public native int IPlay();
	
	/***
	 * ֹͣ����
	 */
	public native int IStop();
}
