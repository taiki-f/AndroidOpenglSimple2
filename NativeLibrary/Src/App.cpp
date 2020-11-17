#include "App.hpp"
#include "AppDef.h"
#include "FrameBuffer.hpp"
#include "ShaderBase.hpp"
#include "ShaderBlend.hpp"
#include "Sprite.hpp"
#include <glm/ext.hpp>

#define TESTCASE 8

using namespace drs::draw;

#if defined(ANDROID)
// 外部関数のプロトタイプ宣言
void* LoadAssetForAndroid(const char* const fileNameP, u32* outSizeP);
#endif


namespace
{
    const char VERTEX_SHADER[] =            // 頂点シェーダ
        //"#version 100"
        "attribute vec4 position;"
        "attribute vec4 color;"
        "varying vec4 vColor;"
        "uniform mat4 projection;"
        "void main() {"
            "gl_Position = projection * position;"
            "vColor = color;"
        "}";
    const char FRAGMENT_SHADER[] =          // フラグメントシェーダ
        //"#version 100"
        "precision mediump float;"
        "varying vec4 vColor;"
        "void main() {"
            "gl_FragColor = vColor;"
        "}";
    const char BLEND_VERTEX_SHADER[] =      // ブレンド用頂点シェーダ
        //"#version 100"
        "attribute vec4 position;"
        "attribute vec4 color;"
        "attribute vec2 texcoord;"
        "varying vec4 vColor;"
        "varying vec2 vTexcoord;"
        "uniform mat4 projection;"
        "void main() {"
            "gl_Position = projection * position;"
            "vColor = color;"
            "vTexcoord = texcoord;"
        "}";
    const char BLEND_FRAGMENT_SHADER[] =     // ブレンド用フラグメントシェーダプログラム
        //"#version 100"
        "precision mediump float;"
        "varying vec4 vColor;"
        "varying vec2 vTexcoord;"
        "uniform sampler2D textureSrc;"
        "uniform sampler2D textureDst;"
        "uniform vec2 textureSize;"
        "const vec4 minColor = vec4(0.0, 0.0, 0.0, 0.0);"
        "const vec4 maxColor = vec4(1.0, 1.0, 1.0, 1.0);"
        "vec3 GetColor(const in vec3 src, const in vec3 dst, const in float alpha)"
        "{"
            "return clamp(vec3(src * dst) * alpha + dst * (1.0 - alpha), minColor.rgb, maxColor.rgb);"
        "}"
        "void main() {"
            "lowp vec4 src = texture2D(textureSrc, vTexcoord);"
            "lowp vec4 dst = texture2D(textureDst, gl_FragCoord.xy / textureSize);"
//*
            "gl_FragColor.rgb = GetColor(src.rgb * vColor.rgb, dst.rgb, src.a * vColor.a);"
            "gl_FragColor.a = 1.0;"
/*/
            "gl_FragColor = clamp(src + dst, minColor, maxColor) * vColor;"
//*/
        "}";
    const char TEX_FRAGMENT_SHADER[] =      // テクスチャ描画用フラグメントシェーダプログラム
        //"#version 100"
        "precision mediump float;"
        "varying vec4 vColor;"
        "varying vec2 vTexcoord;"
        "uniform sampler2D textureSrc;"
        "void main() {"
            "lowp vec4 src = texture2D(textureSrc, vTexcoord);"
            "gl_FragColor = src * vColor;"
        "}";

    // アセットロード
    std::function<void*(const char*, u32*)> LoadAsset = nullptr;

    // 透視変換行列
    mat4 matProjection;

    // フレームバッファ
    FrameBuffer framebuffer;

    // スプライト
    Sprite sprite;
    Sprite colorSprite;

    // シェーダー
    ShaderBase shaderNormal;
    ShaderBlend shaderBlend;
    ShaderBlend shaderTexture;
}

/// <summary>
/// エラーチェック
/// </summary>
void CheckError()
{
    auto error = glGetError();
    while (error != GL_NO_ERROR)
    {
        LOGE("glGetError = %x", error);
        error = glGetError();
    }
}

/// <summary>
/// コンストラクタ
/// </summary>
App::App(u32 width, u32 height)
{
    m_windowSize.x = width;
    m_windowSize.y = height;

#if defined(ANDROID)
    // アセットのロード関数をセット
    Texture::LoadAssetFunc = LoadAssetForAndroid;

#else
    ASSERT(LoadAsset && "アセットのロード関数が設定されていません");

#endif
}

/// <summary>
/// デストラクタ
/// </summary>
App::~App()
{
}

/// <summary>
/// 初期化
/// </summary>
void App::Initialize()
{
    {
        // シェーダーロード
        GLuint	vertex, fragment;
        vertex = LoadShader(GL_VERTEX_SHADER, VERTEX_SHADER);                // 頂点シェーダ
        fragment = LoadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);          // フラグメントシェーダ

        // シェーダプログラム作成
        shaderNormal.Initialize(vertex, fragment);

        // シェーダー開放
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    {
        // シェーダーロード
        GLuint	vertex, fragment;
        vertex = LoadShader(GL_VERTEX_SHADER, BLEND_VERTEX_SHADER);          // 頂点シェーダ
        fragment = LoadShader(GL_FRAGMENT_SHADER, BLEND_FRAGMENT_SHADER);    // フラグメントシェーダ

        // シェーダプログラム作成
        shaderBlend.Initialize(vertex, fragment);

        // シェーダー開放
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    {
        // シェーダーロード
        GLuint	vertex, fragment;
        vertex = LoadShader(GL_VERTEX_SHADER, BLEND_VERTEX_SHADER);          // 頂点シェーダ
        fragment = LoadShader(GL_FRAGMENT_SHADER, TEX_FRAGMENT_SHADER);      // フラグメントシェーダ

        // シェーダプログラム作成
        shaderTexture.Initialize(vertex, fragment);

        // シェーダー開放
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // 透視変換行列初期化(正射影)
    matProjection = glm::ortho(0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.f);

    if (m_windowSize.x*SCREEN_HEIGHT < m_windowSize.y*SCREEN_WIDTH) // 横長（上下カット）
    {
        m_screenSize.x = m_windowSize.x;
        m_screenSize.y = m_windowSize.x * SCREEN_HEIGHT / SCREEN_WIDTH;
    }
    else                                                            // 縦長（左右カット）
    {
        m_screenSize.x = m_windowSize.y * SCREEN_WIDTH / SCREEN_HEIGHT;
        m_screenSize.y = m_windowSize.y;
    }
    m_screenPos.x = (m_windowSize.x - m_screenSize.x) / 2;      // 中央表示
    m_screenPos.y = (m_windowSize.y - m_screenSize.y) / 2;

    // フレームバッファの初期化
#if TESTCASE == 8
    framebuffer.Initialize(drs::util::GET_POW2(SCREEN_WIDTH), drs::util::GET_POW2(SCREEN_HEIGHT));
#else
    framebuffer.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

    // テクスチャロード
    sprite.LoadPNG("videogame_boy_512.png");
    colorSprite.LoadPNG("rgb808_512.png");

    // ブレンドを有効にする
    glEnable(GL_BLEND);

    // 各種設定を無効にする
    glDisable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
    glDisable(GL_SAMPLE_COVERAGE);

    glDepthMask(GL_FALSE);
    glBlendColor(1.f, 1.f, 1.f, 1.f);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

/// <summary>
/// 終了
/// </summary>
void App::Terminate()
{
    shaderNormal.Terminate();
    shaderBlend.Terminate();
    shaderTexture.Terminate();
}

/// <summary>
/// 描画
/// </summary>
void App::Render()
{
    CheckError();

    // 画面クリア
    FrameBuffer::BindForDefult();
    FrameBuffer::Clear(0.0f, 0.5f, 0.0f);        // 緑
    CheckError();

#if (TESTCASE == 0)
    {
    }
#elif (TESTCASE == 8)
    {
        // フレームバッファのクリア
        framebuffer.Bind();
        FrameBuffer::Clear(0.8f, 0.8f, 0.8f);   // グレー
        CheckError();

        GLuint unit = 0;

        {
            // フレームバッファをバインド
            framebuffer.Bind();

            // シェーダーを有効
            shaderTexture.Use(&framebuffer.m_matProjection[0][0]);
            shaderTexture.SetTexcoord(colorSprite);
            CheckError();

            // 各ユニットにテクスチャを設定
            unit = 0;
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, colorSprite.m_texture);
            glUniform1i(shaderTexture.textureSrcIndex, unit);
            CheckError();

            // ブレンド係数設定
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // スプライトの描画
            colorSprite.m_shaderP = &shaderTexture;
            colorSprite.Draw(0.f, 0.f);
            glFlush();
            CheckError();
        }
        {
            // フレームバッファをバインド
            framebuffer.Bind();

            // シェーダーを有効
            shaderBlend.Use(&framebuffer.m_matProjection[0][0]);
            shaderBlend.SetTexcoord(sprite);
            shaderBlend.SetTextureSize(framebuffer.m_width, framebuffer.m_height);
            CheckError();

            // 各ユニットにテクスチャを設定
            unit = 0;
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, sprite.m_texture);
            glUniform1i(shaderBlend.textureSrcIndex, unit);
            unit++;
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, framebuffer.m_texture);
            glUniform1i(shaderBlend.textureDstIndex, unit);
            CheckError();

            // ブレンド係数設定
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ZERO);

            // スプライトの描画
            sprite.m_shaderP = &shaderBlend;
            sprite.Draw(0.f, 0.f);
            CheckError();
        }
        {
            // フレームバッファをバインド
            FrameBuffer::BindForDefult();
            glViewport(m_screenPos.x, m_screenPos.y, m_screenSize.x, m_screenSize.y);

            // シェーダーを有効
            shaderTexture.Use(&matProjection[0][0]);
            shaderTexture.SetTexcoord(framebuffer);
            CheckError();

            // 各ユニットにテクスチャを設定
            unit = 0;
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, framebuffer.m_texture);
            glUniform1i(shaderTexture.textureSrcIndex, unit);
            CheckError();

            // ブレンド係数設定
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // スプライトの描画
            sprite.m_shaderP = &shaderTexture;
            sprite.Draw(0.f, 0.f, drs::util::GET_POW2(SCREEN_WIDTH), drs::util::GET_POW2(SCREEN_HEIGHT));
            CheckError();
        }
    }
#endif
}
