#pragma once

#include "Texture.hpp"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace drs { namespace draw {

    struct FrameBuffer : public Texture
    {
        /// <summary>
        /// デフォルトのフレームバッファをバインド
        /// </summary>
        static void BindForDefult();

        /// <summary>
        /// クリア
        /// </summary>
        /// <param name="r">Rカラー値</param>
        /// <param name="g">Gカラー値</param>
        /// <param name="b">Bカラー値</param>
        static void Clear(GLfloat r, GLfloat g, GLfloat b);

        /// <summary>
        /// コンストラクタ
        /// </summary>
        FrameBuffer();

        /// <summary>
        /// デストラクタ
        /// </summary>
        virtual ~FrameBuffer();

        /// <summary>
        /// 初期化
        /// </summary>
        /// <param name="w">テクスチャのWサイズ</param>
        /// <param name="h">テクスチャのHサイズ</param>
        virtual void Initialize(f32 w, f32 h);

        /// <summary>
        /// バインド
        /// </summary>
        virtual void Bind();

        GLuint m_framebuffer;       // フレームバッファ
        GLuint m_renderbuffer;      // レンダーバッファ
        mat4 m_matProjection;       // 透視変換行列
    };

} } // namespace
