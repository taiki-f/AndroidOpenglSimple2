#include "png.h"
#include "Texture.hpp"

namespace drs { namespace draw {

    namespace
    {
        /// <summary>
        /// メモリ読み込みコールバック
        /// </summary>
        /// <param name="pngP">PNGコンテキスト</param>
        /// <param name="dataP">データの先頭アドレス</param>
        /// <param name="length">データサイズ</param>
        void PNGReadCallback(png_structp pngP, png_bytep dataP, png_size_t length)
        {
            u8** p = (u8**)png_get_io_ptr(pngP);

            memcpy(dataP, *p, length);
            *p += length;
        }
    }

    // アセット関数
    Texture::AssetFunc Texture::LoadAssetFunc = nullptr;

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Texture::Texture()
        : m_texture(0)
    {}

    /// <summary>
    /// デストラクタ
    /// </summary>
    Texture::~Texture()
    {
        if (m_texture)
        {
            glDeleteTextures(1, &m_texture);
            m_texture = 0;
        }
    }

    /// <summary>
    /// テクスチャ生成
    /// </summary>
    void Texture::MakeTexture(u32 w, u32 h, const u8* dataP)
    {
        m_width = w;
        m_height = h;

        // テクスチャオブジェクト作成
        glGenTextures(1, &m_texture);
        glBindTexture(GL_TEXTURE_2D, m_texture);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    /// <summary>
    /// PNGファイルのロード
    /// </summary>
    bool Texture::LoadPNG(const char* fileNameP)
    {
        if (!LoadAssetFunc)
        {
            ASSERT(LoadAssetFunc && "アセットロード関数が設定されていません");
            return false;
        }
        
        void* data = LoadAssetFunc(fileNameP, nullptr);
        if (!data)
        {
            LOGE("ファイルが読み込めません [%s]", fileNameP);
            return false;
        }

        auto png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        ASSERT(png);

        auto info = png_create_info_struct(png);
        ASSERT(info);

        png_set_read_fn(png, nullptr, PNGReadCallback);
        png_init_io(png, (png_FILE_p)&data);
        png_read_info(png, info);

        enum {
            FORMAT_RGB = 3,
            FORMAT_RGBA,
        };
        png_uint_32 width, height;
        int bitDepth, colorType, format;

        png_get_IHDR(png, info, &width, &height, &bitDepth, &colorType, nullptr, nullptr, nullptr);
        switch (colorType)
        {
        case PNG_COLOR_TYPE_RGB:
            format = FORMAT_RGB;
            break;

        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(png);
            png_read_update_info(png, info);
            png_get_IHDR(png, info, &width, &height, &bitDepth, &colorType, nullptr, nullptr, nullptr);
            format = (colorType == PNG_COLOR_TYPE_RGB) ? FORMAT_RGB : FORMAT_RGBA;
            break;

        case PNG_COLOR_TYPE_GRAY:
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            png_set_gray_to_rgb(png);
            png_read_update_info(png, info);
            format = (colorType == PNG_COLOR_TYPE_GRAY) ? FORMAT_RGB : FORMAT_RGBA;
            break;

        default:
            format = FORMAT_RGBA;
            break;
        }

        u32 rowBytes = width * format;
        u8* buf = (u8*)memalign(4, rowBytes * height);
        png_bytep rows[height];
        ASSERT(buf);

        for (u32 i = 0; i < height; ++i)
        {
            rows[i] = buf + (i * rowBytes);
        }
        png_read_image(png, rows);

        // テクスチャ生成
        MakeTexture((f32)width, (f32)height, buf);

        // 後始末
        png_destroy_read_struct(&png, &info, nullptr);
        free(buf);

        return true;
    }

} } // namespace
