#pragma once

#include "ShaderBase.hpp"
#include "Texture.hpp"

namespace drs { namespace draw {

    struct ShaderBlend : public ShaderBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        ShaderBlend();

        /// <summary>
        /// 初期化
        /// </summary>
        /// <param name="vShader">バーテックスシェーダー</param>
        /// <param name="fShader">フラグメントシェーダー</param>
        virtual void Initialize(const GLuint& vShader, const GLuint& fShader) override;

        /// <summary>
        /// シェーダーを有効にする
        /// </summary>
        /// <param name="matProjection">透視変換行列</param>
        virtual void Use(const GLfloat* const matProjection) override;

        /// <summary>
        /// テクスチャサイズを設定
        /// </summary>
        /// <param name="size">サイズ</param>
        void SetTextureSize(const GLfloat* size);

        /// <summary>
        /// テクスチャサイズを設定
        /// </summary>
        /// <param name="w">Wサイズ</param>
        /// <param name="h">Hサイズ</param>
        void SetTextureSize(f32 w, f32 h);

        /// <summary>
        /// テクセルの設定
        /// </summary>
        void SetTexcoord(const Texture& texture);

        GLint textureDstIndex;      // デスティネーションテクスチャ
        GLint textureSrcIndex;      // ソーステクスチャ
        GLint textureSizeIndex;     // テクスチャサイズ
        GLint texcoordIndex;        // UV座標
        GLfloat textureSize[2];     // テクスチャサイズ
        GLfloat texcoord[8];        // テクスチャ座標
    };

} } // namespace
