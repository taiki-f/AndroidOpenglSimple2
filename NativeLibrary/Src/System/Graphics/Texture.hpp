#pragma once

#include <functional>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace drs { namespace draw {

    struct Texture
    {
        // アセット関数
        using AssetFunc = std::function<void*(const char*, u32*)>;;
        static AssetFunc LoadAssetFunc;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        Texture();

        /// <summary>
        /// デストラクタ
        /// </summary>
        virtual ~Texture();

        /// <summary>
        /// テクスチャ生成
        /// </summary>
        /// <param name="w">テクスチャのWサイズ</param>
        /// <param name="h">テクスチャのHサイズ</param>
        /// <param name="dataP">テクスチャデータ</param>
        virtual void MakeTexture(u32 w, u32 h, const u8* dataP = nullptr);

        /// <summary>
        /// PNGファイルのロード
        /// </summary>
        /// <param name="fileNameP">ファイル名</param>
        /// <returns>成否</returns>
        bool LoadPNG(const char* fileNameP);


        GLuint m_texture;               // テクスチャ
        f32 m_width, m_height;          // テクスチャのサイズ
    };

} } // namespace
