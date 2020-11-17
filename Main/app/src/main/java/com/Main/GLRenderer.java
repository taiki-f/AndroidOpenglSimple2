package com.Main;

// インポート
import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * @brief OpenGLレンダラー
 */
public class GLRenderer implements GLSurfaceView.Renderer
{
    // アプリの初期化
    native void NativeAppInit(AssetManager assetManager, int w, int h);
    // アプリの描画
    native void NativeAppDraw();

    // コンテキスト
    Context context;
    // OpenGLインターフェイス
	GL10 gl = null;
    // OpenGLコンフィグ
    EGLConfig config = null;

    /**
     * コンストラクタ
     */
    public GLRenderer(Context context)
    {
        this.context = context;
    }

    /**
     * サーフェイス生成
     * @param gl OpenGLインターフェイス
     * @param config OpenGLコンフィグ
     */
    @Override
	synchronized public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		this.gl = gl;
        this.config = config;
	}

    /**
     * サーフェイス変更
     * @param gl OpenGLインターフェイス
     * @param w サーフェイスのWサイズ
     * @param h サーフェイスのHサイズ
     */
    @Override
	synchronized public void onSurfaceChanged(GL10 gl, int w, int h)
	{
		this.gl = gl;
        NativeAppInit(context.getAssets(), w, h);
	}

    /**
     * 描画
     * @param gl OpenGLインターフェイス
     */
    @Override
	public void onDrawFrame(GL10 gl)
	{
		this.gl = gl;

		synchronized(this)
		{
            NativeAppDraw();
		}
	}

    /**
     * 画面のクリア
     */
	public void clearScreen()
	{
		if (gl != null)
		{
			gl.glClearColor(0, 0, 0, 1.0f);
			gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		}
	}
}
