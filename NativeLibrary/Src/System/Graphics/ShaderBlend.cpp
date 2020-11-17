#include "ShaderBlend.hpp"

namespace drs { namespace draw {

    /// <summary>
    /// コンストラクタ
    /// </summary>
    ShaderBlend::ShaderBlend()
        : ShaderBase()
        , textureDstIndex(0)
        , textureSrcIndex(0)
    {}

    /// <summary>
    /// 初期化
    /// </summary>
    void ShaderBlend::Initialize(const GLuint& vShader, const GLuint& fShader)
    {
        ShaderBase::Initialize(vShader, fShader);
        textureDstIndex = glGetUniformLocation(program, "textureDst");
        textureSrcIndex = glGetUniformLocation(program, "textureSrc");
        textureSizeIndex = glGetUniformLocation(program, "textureSize");
        texcoordIndex = glGetAttribLocation(program, "texcoord");
    }

    /// <summary>
    /// シェーダーを有効にする
    /// </summary>
    void ShaderBlend::Use(const GLfloat* const matProjection)
    {
        ShaderBase::Use(matProjection);
    }

    /// <summary>
    /// テクスチャサイズを設定
    /// </summary>
    void ShaderBlend::SetTextureSize(const GLfloat* size)
    {
        if (textureSizeIndex != -1)
        {
            glUniform2fv(textureSizeIndex, 1, size);
        }
    }
    void ShaderBlend::SetTextureSize(f32 w, f32 h)
    {
        textureSize[0] = w;
        textureSize[1] = h;
        SetTextureSize(textureSize);
    }
    /// <summary>
    /// テクセルの設定
    /// </summary>
    void ShaderBlend::SetTexcoord(const Texture& texture)
    {
        if (texcoordIndex != -1)
        {
            glEnableVertexAttribArray(texcoordIndex);

            texcoord[0] = texcoord[4] = 0.f;
            texcoord[2] = texcoord[6] = 1.f;
            texcoord[1] = texcoord[3] = 0.f;
            texcoord[5] = texcoord[7] = 1.f;

            glVertexAttribPointer(texcoordIndex, 2, GL_FLOAT, GL_FALSE, 0, texcoord);		// UV座標
        }
        else
        {
            glDisableVertexAttribArray(texcoordIndex);
        }
    }

} } // namespace
