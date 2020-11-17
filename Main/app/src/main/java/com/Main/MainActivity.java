package com.Main;

// インポート
import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import java.util.Timer;
import java.util.TimerTask;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;
import android.opengl.GLSurfaceView.EGLConfigChooser;
/**
 * @brief メインアクティビティ
 */
public class MainActivity extends Activity
{
    // Nativeライブラリのロード
	static {
        System.loadLibrary("NativeLibrary");
	}

    // OpenGLレンダラー
    GLRenderer glRenderer;
    // OpenGLサーフェイス
    GLSurfaceView glSurfaceView;
    // タイマー
    Timer timer;

    /**
     * @brief アクティビティ生成
     * @param savedInstanceState バンドル
     */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        // GLレンダラーの準備
        glRenderer = new GLRenderer(this);

        // GLサーフェイスの準備
        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setDebugFlags(GLSurfaceView.DEBUG_CHECK_GL_ERROR | GLSurfaceView.DEBUG_LOG_GL_CALLS);
        glSurfaceView.setEGLContextClientVersion(2);
        glSurfaceView.setEGLConfigChooser(new MainChooser());
        glSurfaceView.setRenderer(glRenderer);
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        // OnPause時にEGL資源を保持したままにしておく
        //glSurfaceView.setPreserveEGLContextOnPause(true);

        // ビューの設定
        setContentView(glSurfaceView);

        // 定期実行開始
        startTimer();
    }

    /**
     * @brief 定期実行開始
     */
    void startTimer()
    {
        // 定期実行
        timer = new Timer();
		TimerTask task = new TimerTask() {
			synchronized public void run() {
				glSurfaceView.requestRender();
			}
		};
		timer.schedule(task, 0, 16);
    }

    /**
     * @brief 一時停止
     */
	@Override
	protected void onPause()
	{
		super.onPause();

		synchronized(glRenderer) {
			if (timer != null) {
				timer.cancel();
				timer = null;
			}
		}
		glSurfaceView.onPause();
        glRenderer.clearScreen();
    }

    /**
     * @brief 再開
     */
	@Override
	protected void onResume()
	{
		super.onResume();

		glSurfaceView.onResume();
        if (timer == null) {
			new Handler().post(new Runnable() {
				@Override
				public void	run() {
					startTimer();
				}
			});
		}
	}

    class MainChooser implements EGLConfigChooser
    {
        @Override
        public EGLConfig chooseConfig(EGL10 egl, EGLDisplay display)
        {
            int[] attribList = new int[] {
                EGL10.EGL_RED_SIZE,     8,  // R成分
                EGL10.EGL_GREEN_SIZE,   8,  // G成分
                EGL10.EGL_BLUE_SIZE,    8,  // B成分
                EGL10.EGL_ALPHA_SIZE,   8,  // A成分
                EGL10.EGL_DEPTH_SIZE,   8,  // 深度バッファ
                EGL10.EGL_STENCIL_SIZE, 8,  // ステンシルバッファ
                EGL10.EGL_NONE,
            };

            // 条件を満たす構成の数を取得
            int[] numConfig = new int[1];
            egl.eglChooseConfig(display, attribList, null, 0, numConfig);

            // 条件を満たす構成をすべて取得
            EGLConfig[] configs = new EGLConfig[numConfig[0]];
            egl.eglChooseConfig(display, attribList, configs, configs.length, numConfig);

            return configs[0];
        }
    }
}
