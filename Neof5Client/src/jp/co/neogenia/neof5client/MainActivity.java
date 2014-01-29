package jp.co.neogenia.neof5client;

import java.net.URLEncoder;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.View;
import android.widget.*;


public class MainActivity extends Activity implements View.OnClickListener {

	private Button btnConnect;
	private Handler handler = new Handler();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		btnConnect = (Button)findViewById(R.id.btnConnect);
		btnConnect.setOnClickListener(this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

    //-------------------------------------------------------------------------
    // JNI
    //-------------------------------------------------------------------------
	static {
		System.loadLibrary("gnustl_shared");
		System.loadLibrary("neof5client");
	}
	private native int jnimain(int level, int ruleAB, String host, int port, String path);

	@Override
	public void onClick(View arg0) {
		try{
			final String host = ((EditText)findViewById(R.id.editTextHost)).getText().toString();
			final String port = ((EditText)findViewById(R.id.editTextPort)).getText().toString();
			final String path = ((EditText)findViewById(R.id.editTextPath)).getText().toString();
			final String level = ((EditText)findViewById(R.id.editTextLevel)).getText().toString();
			
			final int intPort = Integer.parseInt(port);
			int a = path.indexOf("/",1);
			final int rule = path.charAt(a+1) == 'B' ? 1 : 0;

			final int intLevel= Integer.parseInt(level);
		
			final String name = URLEncoder.encode("ネオ爺"+level,"utf-8");
			
			// ボタンを無効化
			btnConnect.setEnabled(false);

			// 別スレッドで起動
			Thread t = new Thread(new Runnable() {
				
				@Override
				public void run() {
					jnimain(intLevel, rule, host, intPort, path+"?name="+name);
					
					// ボタンを有効化
					handler.post(new Runnable() {						
						@Override
						public void run() {
							btnConnect.setEnabled(true);
						}
					});
				}
			});
			t.start();
		}catch(Exception ex){
			Toast.makeText(this, ex.getLocalizedMessage(), Toast.LENGTH_LONG).show();
		}
	}
	
	
}
