#include "ShaderBase.hpp"

namespace drs { namespace draw {

    /// <summary>
    /// シェーダーロード
    /// </summary>
    GLuint LoadShader(GLenum type, const char* const source)
    {
        GLuint	_shader = glCreateShader(type);                     // シェーダオブジェクト作成
        GLint	_compiled = 0;

        assert(_shader != 0);
        glShaderSource(_shader, 1, &source, NULL);                  // プログラムソース設定
        glCompileShader(_shader);                                   // コンパイル
        glGetShaderiv(_shader, GL_COMPILE_STATUS, &_compiled);      // コンパイル結果取得
        if (!_compiled) {                                           // コンパイル失敗
            GLchar*	_buf;
            GLint	_len;

            glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, &_len);
            if ((_len > 0) && (_buf = new GLchar[_len])) {
                glGetShaderInfoLog(_shader, _len, NULL, _buf);      // エラーログ取得
                LOGE("Could not compile shader %d:\n%s\n", type, _buf);
                delete[]	_buf;
            }
            glDeleteShader(_shader);
            return	0;
        }
        return	_shader;
    }

    /// <summary>
    /// コンストラクタ
    /// </summary>
    ShaderBase::ShaderBase()
        : program(0)
    {}

    /// <summary>
    /// デストラクタ
    /// </summary>
    ShaderBase::~ShaderBase()
    {}

    /// <summary>
    /// 初期化
    /// </summary>
    void ShaderBase::Initialize(const GLuint& vShader, const GLuint& fShader)
    {
        // シェーダプログラム作成
        program = glCreateProgram();
        assert(program != 0);
        glAttachShader(program, vShader);
        glAttachShader(program, fShader);

        // リンク
        GLint	linked = GL_FALSE;
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked)
        {
            GLchar*	buf;
            GLint	len;

            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
            if ((len > 0) && (buf = new GLchar[len]))
            {
                // エラーログ取得
                glGetProgramInfoLog(program, len, NULL, buf);
                LOGE("Could not link program:\n%s\n", buf);
                delete[] buf;
            }
            glDeleteProgram(program);
            program = 0;
            return;
        }

        positionIndex = glGetAttribLocation(program, "position");           // 座標
        colorIndex = glGetAttribLocation(program, "color");                 // カラー
        projectionIndex = glGetUniformLocation(program, "projection");      // 透視変換
    }

    /// <summary>
    /// 終了
    /// </summary>
    void ShaderBase::Terminate()
    {
        if (program)
        {
            glDeleteProgram(program);
        }
    }

    /// <summary>
    /// シェーダーを有効にする
    /// </summary>
    void ShaderBase::Use(const GLfloat* const matProjection)
    {
        assert(program != 0);
        glUseProgram(program);
        glUniformMatrix4fv(projectionIndex, 1, GL_FALSE, matProjection);
        glEnableVertexAttribArray(positionIndex);
        glEnableVertexAttribArray(colorIndex);
    }

} } // namespace
