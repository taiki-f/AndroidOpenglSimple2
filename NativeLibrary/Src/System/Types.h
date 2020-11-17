#pragma once

#include <glm/glm.hpp>

// 独自型
using s8 = glm::i8;
using u8 = glm::u8;
using s16 = glm::i16;
using u16 = glm::u16;
using s32 = glm::i32;
using u32 = glm::u32;
using s64 = glm::i64;
using u64 = glm::u64;
using f32 = glm::f32;
using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;
using mat4 = glm::mat4;

// 型ごとの最大値
constexpr u8 MAX_TYPE8 = 0xff;
constexpr u16 MAX_TYPE16 = 0xffff;
constexpr u32 MAX_TYPE32 = 0xffffffff;
