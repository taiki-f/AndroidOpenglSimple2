#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace drs { namespace draw {

    /// <summary>
    /// シェーダーロード
    /// </summary>
    /// <param name="type">シェーダータイプ</param>
    /// <param name="source">ソースコード</param>
    /// <returns>生成したシェーダーオブジェクト</returns>
    GLuint LoadShader(GLenum type, const char* const source);

    struct ShaderBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        ShaderBase();

        /// <summary>
        /// デストラクタ
        /// </summary>
        virtual ~ShaderBase();

        /// <summary>
        /// 初期化
        /// </summary>
        /// <param name="vShader">バーテックスシェーダー</param>
        /// <param name="fShader">フラグメントシェーダー</param>
        virtual void Initialize(const GLuint& vShader, const GLuint& fShader);

        /// <summary>
        /// 終了
        /// </summary>
        virtual void Terminate();

        /// <summary>
        /// シェーダーを有効にする
        /// </summary>
        /// <param name="matProjection">透視変換行列</param>
        virtual void Use(const GLfloat* const matProjection);

        mat4 matProjection;     // 透視変換行列
        GLuint program;         // シェーダープログラム
        GLint positionIndex;    // 座標
        GLint colorIndex;       // カラー
        GLint projectionIndex;  // 透視変換
    };

} } // namespace
