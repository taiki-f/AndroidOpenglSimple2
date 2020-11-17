#pragma once

namespace drs { namespace util {

    /// <summary>
    /// べき乗の値を取得する
    /// </summary>
    /// <param name="value">元の値</param>
    template<typename _Type>
    _Type GET_POW2(_Type value) {
        _Type v;
        for (v = 1; v < value; v *= (_Type)2);
        return v;
    }

    /// <summary>
    /// 配列サイズを取得する
    /// </summary>
    /// <param name="&">配列参照</param>
    /// <returns>配列の要素数</returns>
    template<typename _Type, size_t _Size>
    size_t ARRAY_SIZE(const _Type(&)[_Size]) {
        return _Size;
    }

    /// <summary>
    /// カラー共用体
    /// </summary>
    union Color {
        u32 full;
        struct {
            u8 a, b, g, r;
        };
    };
    #define RGBA(r, g, b, a)    (((u32)(a) << 24) | ((u32)(b) << 16) | ((u32)(g) << 8) | ((u32)(r)))

} } // namespace

#if defined(NDEBUG)
    #define LOGI(...)   {}
    #define LOGW(...)   {}
    #define LOGE(...)   {}
    #define ASSERT(v)   {}

#else   // defined(NDEBUG)
    #if defined(ANDROID)
        #define LOGI(...)   ((void)__android_log_print(ANDROID_LOG_INFO, "NativeLibrary", __VA_ARGS__))
        #define LOGW(...)   ((void)__android_log_print(ANDROID_LOG_WARN, "NativeLibrary", __VA_ARGS__))
        #define LOGE(...)   ((void)__android_log_print(ANDROID_LOG_ERROR, "NativeLibrary", __VA_ARGS__))
        #define ASSERT(v)   {if (!(v)) {LOGE(#v); assert(false);}}

    #else
        #define LOGI(...)   {}
        #define LOGW(...)   {}
        #define LOGE(...)   {}
        #define ASSERT(v)   {}

    #endif

#endif  //  defined(NDEBUG)
