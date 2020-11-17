#pragma once

/// <summary>
/// アプリ
/// </summary>
class App
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="width">アプリの横サイズ</param>
    /// <param name="height">アプリの縦サイズ</param>
    App(u32 width, u32 height);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~App();

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize();

    /// <summary>
    /// 終了
    /// </summary>
    void Terminate();

    /// <summary>
    /// 描画
    /// </summary>
    void Render();

public:
    vec2 m_windowSize;
    vec2 m_screenPos;
    vec2 m_screenSize;
};
