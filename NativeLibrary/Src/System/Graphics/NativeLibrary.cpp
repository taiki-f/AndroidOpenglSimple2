#include "App.hpp"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <malloc.h>

namespace
{
    // アプリ
    App* app;

    // アセットマネージャー
    AAssetManager* assetManagerP = nullptr;
}

extern "C"
{
    /// <summary>
    /// アプリの初期化
    /// </summary>
    /// <param name="envP">JNI関数のポインタ構造体</param>
    /// <param name="">未使用</param>
    /// <param name="width">サーフェイスのWサイズ</param>
    /// <param name="height">サーフェイスのHサイズ</param>
    /// <note>GLRendererクラス内の関数</note>
    JNICALL void JNIEXPORT Java_com_Main_GLRenderer_NativeAppInit(JNIEnv* envP, jobject, jobject manager, jint width, jint height);

    /// <summary>
    /// アプリの終了
    /// </summary>
    /// <param name="envP">JNI関数のポインタ構造体</param>
    /// <param name="">未使用</param>
    /// <note>GLRendererクラス内の関数</note>
    JNICALL void JNIEXPORT Java_com_Main_GLRenderer_NativeAppQuit(JNIEnv* envP, jobject);

    /// <summary>
    /// アプリの描画
    /// </summary>
    /// <param name="envP">JNI関数のポインタ構造体</param>
    /// <param name="">未使用</param>
    /// <note>GLRendererクラス内の関数</note>
    JNICALL void JNIEXPORT Java_com_Main_GLRenderer_NativeAppDraw(JNIEnv* eenvPnv, jobject);
}

/// <summary>
/// アプリの初期化
/// </summary>
JNICALL void JNIEXPORT Java_com_Main_GLRenderer_NativeAppInit(JNIEnv* envP, jobject, jobject manager, jint width, jint height)
{
    LOGI("NativeInit call [w:%d][h:%d]", width, height);

    // アセットマネージャーの初期化
    assetManagerP = AAssetManager_fromJava(envP, manager);
    ASSERT(assetManagerP && "アセットマネージャーの初期化に失敗しました")

    // アプリの初期化
    app = new App(width, height);
    app->Initialize();
}

/// <summary>
/// アプリの終了
/// </summary>
JNICALL void JNIEXPORT Java_com_Main_GLRenderer_NativeAppQuit(JNIEnv* envP, jobject)
{
    //LOGI("NativeQuit call");

    // アプリの終了
    app->Terminate();
    delete app;
}

/// <summary>
/// アプリの描画
/// </summary>
JNICALL void JNIEXPORT Java_com_Main_GLRenderer_NativeAppDraw(JNIEnv* envP, jobject)
{
    //LOGI("NativeDraw call");

    // アプリの描画
    app->Render();
}

/// <summary>
/// Android用アセットのロード
/// </summary>
/// <param name="fileNameP">ファイル名</param>
/// <param name="outSizeP">ファイルサイズの出力先</param>
/// <returns>取得したアセットのポインタ</returns>
/// <note>取得したアセットは取得先でfreeしてください。</note>
void* LoadAssetForAndroid(const char* const fileNameP, u32* outSizeP = nullptr)
{
    if (!assetManagerP)
    {
        LOGE("アセットマネージャーの実体がありません");
        return nullptr;
    }

    auto asset = AAssetManager_open(assetManagerP, fileNameP, AASSET_MODE_BUFFER);
    if (!asset)
    {
        LOGE("アセットが実在しません [%s]", fileNameP);
        return nullptr;
    }

    size_t size = AAsset_getLength(asset);
    void* buf = malloc(size);
    AAsset_read(asset, buf, size);
    AAsset_close(asset);

    // 必要ならサイズを返す
    if (outSizeP)
    {
        *outSizeP = (u32)size;
    }
    return buf;
}
