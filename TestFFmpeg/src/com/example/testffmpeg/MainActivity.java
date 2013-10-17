package com.example.testffmpeg;

import android.app.Activity;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Environment;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class MainActivity extends Activity {
	
	private TextView textVersion = null;
	private Button btnGetVersion = null;
	private Button btnStartPlay = null;
	private Button btnStopPlay = null;	
	private CMediaProcess m_MediaProcess = null;
	private String m_strRTSPUrl = "";
	private EditText EdtUrl = null;
	private DisplayMetrics m_DisPlayMetrics;
	
	public CFFmpegJni m_FFmpegJni = null;
	/// ����Ҫ���õĶ�̬��
	static 
	{
		System.loadLibrary("ffmpeg");
		System.loadLibrary("FFmpegJni");
	}
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		/// ��ʾ�������
		m_DisPlayMetrics = new DisplayMetrics();  
		this.getWindowManager().getDefaultDisplay().getMetrics(m_DisPlayMetrics);
	    int nVideoHeight = (m_DisPlayMetrics.widthPixels / 16) * 9;
		/// int nVideoHeight = (m_DisPlayMetrics.widthPixels / 4) * 3;
	    
	    Log.i("DisPlayInfo", "nVideoWidth" + m_DisPlayMetrics.widthPixels + "nVideoHeight" + nVideoHeight);
		
		/// ���ò�����ʾ����
	    CVideoView view = (CVideoView)findViewById(R.id.ConstonVideoView);
	    view.e_SetDisplayWidth(m_DisPlayMetrics.widthPixels);
	    /// ������Ƶ��ͼƬ��ʾ�ĸ߶�
	    view.e_SetDisplayHeight(nVideoHeight);
	    view.e_SetContext(this);
	    /// ���ÿؼ���ʾ�ĸ߶�
	    RelativeLayout.LayoutParams Params = (RelativeLayout.LayoutParams) view.getLayoutParams();
	    Params.height = nVideoHeight;
	    view.setLayoutParams(Params);
	    view.setBackgroundColor(Color.BLACK);
		view.e_Play();
		
		/// ʵ����FFmpegJni������
		m_FFmpegJni = new CFFmpegJni();		
		if(null != m_FFmpegJni)
		{
			/// ʵ����ý�崦�����
			m_MediaProcess = new CMediaProcess(m_FFmpegJni);
		}
		
		/// ��ȡ��ͼ����
		textVersion = (TextView)findViewById(R.id.TestVersion);
		btnGetVersion = (Button)findViewById(R.id.btnGetVersion);
		btnStartPlay = (Button)findViewById(R.id.btnStartPlay);
		btnStopPlay = (Button)findViewById(R.id.btnStopPlay);
		EdtUrl = (EditText)findViewById(R.id.EdtRTSPURL);
		/// ���ð�ť����
		btnGetVersion.setOnClickListener(new onBtnClickListener());
		btnStartPlay.setOnClickListener(new onBtnClickListener());
		btnStopPlay.setOnClickListener(new onBtnClickListener());
	}

	class onBtnClickListener implements OnClickListener
	{
		@Override
		public void onClick(View v) {
			
			int nBtnID = v.getId();
			
			switch (nBtnID) {
			case R.id.btnGetVersion:
				/// ��ȡ�汾��
				int nVersion = m_FFmpegJni.IGetVersion();
				/// ���û�ȡ�汾��
				textVersion.setText("Version:" + nVersion);
				break;
			case R.id.btnStartPlay:
				/// ����߳�δ����				
				if(null != m_MediaProcess)
				{
					/// ��ȡURL��ַ
					m_strRTSPUrl = EdtUrl.getText().toString();
					/// ����������粥���ҵ��ļ�·��
					if(false == m_strRTSPUrl.startsWith("http://") && false == m_strRTSPUrl.startsWith("rtsp://"))
					{
						String strFileName = Environment.getExternalStorageDirectory().toString() + "/" + m_strRTSPUrl;					
						m_strRTSPUrl = strFileName;
					}
					/// ��ʼ��������Ϣ
					if(0 == m_MediaProcess.e_Init(m_strRTSPUrl, CMediaProcess.Net_TCP_Type))
					{
						/// ��ʼ����
						m_MediaProcess.e_Start();
					}
				}
				break;
			case R.id.btnStopPlay:
				if(null != m_MediaProcess)
				{
					/// ֹͣ����
					m_MediaProcess.e_Stop();
				}				
				break;
			default:
				break;
			}
		}
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
}