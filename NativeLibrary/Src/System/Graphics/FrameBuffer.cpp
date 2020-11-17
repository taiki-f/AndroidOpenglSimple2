#include "AppDef.h"
#include "FrameBuffer.hpp"
#include <glm/ext.hpp>

namespace drs { namespace draw {

    /// <summary>
    /// デフォルトのフレームバッファをバインド
    /// </summary>
    void FrameBuffer::BindForDefult()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    /// <summary>
    /// クリア
    /// </summary>
    void FrameBuffer::Clear(GLfloat r, GLfloat g, GLfloat b)
    {
        glClearColor(r, g, b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    /// <summary>
    /// コンストラクタ
    /// </summary>
    FrameBuffer::FrameBuffer()
        : Texture()
        , m_framebuffer(0)
        , m_renderbuffer(0)
    {}

    /// <summary>
    /// デストラクタ
    /// </summary>
    FrameBuffer::~FrameBuffer()
    {
        if (m_framebuffer)
        {
            glDeleteFramebuffers(1, &m_framebuffer);
            m_framebuffer = 0;
        }
        if (m_renderbuffer)
        {
            glDeleteRenderbuffers(1, &m_renderbuffer);
            m_renderbuffer = 0;
        }
    }

    /// <summary>
    /// 初期化
    /// </summary>
    void FrameBuffer::Initialize(f32 w, f32 h)
    {
        // フレームバッファ生成
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        // レンダーバッファ生成
        glGenRenderbuffers(1, &m_renderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);

        // テクスチャ生成
        MakeTexture(w, h);

        // カラーバッファ用にテクスチャをアタッチ
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);
        BindForDefult();

        // 描画結果を考慮してtopとbottomを反転させておく
        m_matProjection = glm::ortho(0.f, w, 0.f, h);
    }

    /// <summary>
    /// バインド
    /// </summary>
    void FrameBuffer::Bind()
    {
        if (m_framebuffer)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
            glViewport(0, 0, m_width, m_height);
        }
    }

} } // namespace
