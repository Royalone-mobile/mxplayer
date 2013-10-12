package com.manzz.mvideo;

import android.app.Activity;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnPreparedListener;
import android.os.Bundle;
import android.os.Environment;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends Activity {

	// / ����ý�岥����
	private MediaPlayer mPlayer = null;

	private EditText EditFileName = null;
	
	private SurfaceView SFView = null;
	
	private String m_strFileName = "";
	
	private int	nCurPos = 0;

	@SuppressWarnings("deprecation")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		/// �������ſؼ�
		mPlayer = new MediaPlayer();
		/// ��ȡ�ļ�����·���༭��
		EditFileName = (EditText) findViewById(R.id.EditFileName);
		/// ��ȡ��ʾ��Ƶ�Ŀؼ�
		SFView = (SurfaceView) findViewById(R.id.SFVideo);
		/// ���ò�����ͼ��
		SFView.getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		SFView.getHolder().setFixedSize(160, 120);
		/// SFView.setKeepScreenOn(true);
		SFView.getHolder().setKeepScreenOn(true);
		SFView.getHolder().addCallback(new SurfaceCallBack());
	}
	
	class SurfaceCallBack implements Callback
	{
		@Override
		public void surfaceChanged(SurfaceHolder holder, int format, int width,
				int height) {
			// TODO Auto-generated method stub
		}

		@Override
		public void surfaceCreated(SurfaceHolder holder) {
			// TODO Auto-generated method stub
			if(0 < nCurPos && false == m_strFileName.equals(""))
			{
				/// �����ļ�·��ָ�����ļ�
				System.out.println("File Name = " + m_strFileName);
				e_MedidaPlay(m_strFileName, nCurPos);				
				nCurPos = 0;
			}
		}

		@Override
		public void surfaceDestroyed(SurfaceHolder holder) {
			// TODO Auto-generated method stub
			if(null != mPlayer && true == mPlayer.isPlaying())
			{
				nCurPos = mPlayer.getCurrentPosition();
				mPlayer.stop();
			}
		}		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public void MediaOption(View viewType) {
		switch (viewType.getId()) {
		case R.id.BtnStart:

			if (null != EditFileName) {
				
				m_strFileName = EditFileName.getText().toString();
				/// ����������粥���ҵ��ļ�·��
				if(false == m_strFileName.startsWith("http://") && false == m_strFileName.startsWith("rtsp://"))
				{
					String strFileName = Environment.getExternalStorageDirectory().toString() + "/" + m_strFileName;					
					m_strFileName = strFileName;
				}
				System.out.println("File Name ---> " + m_strFileName);				
				e_MedidaPlay(m_strFileName, 0);
				
				/*String strFileName = EditFileName.getText().toString();
				System.out.println("File Name = " + strFileName);
				File file = new File(Environment.getExternalStorageDirectory(), strFileName);
			    if (file.exists()) {
					strFileName = file.getAbsolutePath();
					
					System.out.println("File Name ---> " + strFileName);
					/// �����ļ�·��ָ�����ļ�
					e_MedidaPlay(strFileName, 0);
			    }*/
			}

			break;
		case R.id.BtnPause:
			
			if(true == mPlayer.isPlaying())
			{
				mPlayer.pause();
			}
			else
			{
				mPlayer.start();
			}
			break;
		case R.id.BtnRePlay:
			
			if(true == mPlayer.isPlaying())
			{
				mPlayer.seekTo(0);
			}
			else
			{
				if(false == m_strFileName.equals(""))
				{
					System.out.println("File Name = " + m_strFileName);
					e_MedidaPlay(m_strFileName, 0);
				}
			}
			
			break;
		case R.id.BtnStop:
			if(true == mPlayer.isPlaying())
			{
				mPlayer.stop();
			}
			break;
		default:
			System.out.println("δ֪�İ�ť���� on MediaOption...");
			break;
		}
	}

	@Override
	protected void onDestroy() {
		/// �ͷ�ý�岥����
		mPlayer.release();
		/// ���ò���������Ϊnull
		mPlayer = null;
		super.onDestroy();
	}

	public void e_MedidaPlay(String strPath, int nCurPos) {
		try {			
			/// ���ò������ؼ�
			mPlayer.reset();
			/// ���ò���·��������Դ
			mPlayer.setDataSource(strPath);
			/// ������ʾ��ͼ
			mPlayer.setDisplay(SFView.getHolder());
			/// ��������
			mPlayer.prepare();
			/// ���ü�������״̬
			mPlayer.setOnPreparedListener(new PreparedListener(nCurPos));
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	class PreparedListener implements OnPreparedListener{
		
		private int m_nCurPos = 0;
		
		public PreparedListener(int nCurPos)
		{
			m_nCurPos = nCurPos;
		}

		@Override
		public void onPrepared(MediaPlayer mp) {
			// TODO Auto-generated method stub
			if(0 < m_nCurPos)
			{
				mPlayer.seekTo(m_nCurPos);
			}
			/// ����������ɣ�������Ƶ
			mPlayer.start();
		}
	}
}
