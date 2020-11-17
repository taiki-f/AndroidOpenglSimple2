#pragma once

#include "ShaderBase.hpp"
#include "Texture.hpp"

namespace drs { namespace draw {

    /// <summary>
    /// スプライト
    /// </summary>
    class Sprite : public Texture
    {
        enum {
            X = 0,
            Y,
            XY = 2,
        };
        enum {
            R = 0,
            G,
            B,
            A,
            RGB = 3,
            ARGB = 4,
        };

    public:
        /// <summary>
        /// コンストラクタ
        /// </summary>
        Sprite();

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~Sprite();

        /// <summary>
        /// 描画
        /// </summary>
        /// <param name="vertex">頂点座標</param>
        void Draw(const f32* const vertex);

        /// <summary>
        /// 描画
        /// </summary>
        /// <param name="x">X座標</param>
        /// <param name="y">Y座標</param>
        void Draw(f32 x, f32 y);

        /// <summary>
        /// 描画
        /// </summary>
        /// <param name="x">X座標</param>
        /// <param name="y">Y座標</param>
        /// <param name="w">Wサイズ</param>
        /// <param name="h">Hサイズ</param>
        void Draw(f32 x, f32 y, f32 w, f32 h);

        /// <summary>
        /// カラーを指定
        /// </summary>
        /// <param name="abgr">ABGRカラー</param>
        void SetColor(u32 abgr);

        /// <summary>
        /// カラーを指定
        /// </summary>
        /// <param name="abgr">各頂点のARGBカラー</param>
        void SetColor(const u32 (&abgr)[4]);

        /// <summary>
        /// カラーを指定
        /// </summary>
        void SetColor();

    public:
        // 頂点座標
        f32 m_vertex[XY * 4];

        // 頂点カラー
        u8 m_vertexColor[ARGB * 4];

        // シェーダー
        ShaderBase* m_shaderP;
    };

} } // namespace
