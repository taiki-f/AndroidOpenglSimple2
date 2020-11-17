#include "Sprite.hpp"

namespace drs { namespace draw {

    /// <summary>
    /// コンストラクタ
    /// </summary>
    Sprite::Sprite()
        : Texture()
    {
        SetColor(0xffffffff);
    }

    /// <summary>
    /// デストラクタ
    /// </summary>
    Sprite::~Sprite()
    {}

    /// <summary>
    /// 描画
    /// </summary>
    void Sprite::Draw(const f32* const vertex)
    {
        SetColor();
        glVertexAttribPointer(m_shaderP->positionIndex, 2, GL_FLOAT, GL_FALSE, 0, vertex);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    void Sprite::Draw(f32 x, f32 y)
    {
        Draw(x, y, m_width, m_height);
    }
    void Sprite::Draw(f32 x, f32 y, f32 w, f32 h)
    {
        m_vertex[XY * 0 + X] = m_vertex[XY * 2 + X] = x;
        m_vertex[XY * 1 + X] = m_vertex[XY * 3 + X] = x + w;
        m_vertex[XY * 0 + Y] = m_vertex[XY * 1 + Y] = y;
        m_vertex[XY * 2 + Y] = m_vertex[XY * 3 + Y] = y + h;

        Draw(m_vertex);
    }

    /// <summary>
    /// カラーを指定
    /// </summary>
    void Sprite::SetColor(u32 abgr)
    {
        u32* p = (u32*)m_vertexColor;
        p[0] = abgr;
        p[1] = abgr;
        p[2] = abgr;
        p[3] = abgr;
    }
    void Sprite::SetColor(const u32 (&abgr)[4])
    {
        u32* p = (u32*)m_vertexColor;
        for (int i = 0; i < 4; ++i)
        {
            p[i] = abgr[i];
        }
    }
    void Sprite::SetColor()
    {
        glVertexAttribPointer(m_shaderP->colorIndex, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (GLubyte*)m_vertexColor);
    }

} } // namespace
