#pragma once

#include <corecrt_math_defines.h>

struct Color3f
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
};

struct vec4f
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;
};

struct ViewBox
{
    float left = -1.0f;
    float right = 1.0f;
    float bttm = -1.0f;
    float top = 1.0f;
};

struct Point
{
    float x = 0.0f;
    float y = 0.0f;
    Point() {}
    Point(float _x, float _y): x(_x), y(_y)
    {}
};

constexpr Color3f kColorRed = { 1.0f, 0.0f, 0.0f };
constexpr Color3f kColorGreen = { 0.0f, 1.0f, 0.0f };
constexpr Color3f kColorBlue = { 0.0f, 0.0f, 1.0f };
constexpr Color3f kColorWhite = { 1.0f, 1.0f, 1.0f };
constexpr Color3f kColorBlack = { 0.0f, 0.0f, 0.0f };
constexpr Color3f kColorGray = { 0.5f, 0.5f, 0.5f };
constexpr Color3f kColorCyan = { 0.0f, 1.0f, 1.0f };
constexpr Color3f kColorMagenta = { 1.0f, 0.0f, 1.0f };
constexpr Color3f kColorYellow = { 1.0f, 1.0f, 0.0f };
constexpr Color3f kColorBG = { 0.3f, 0.4f, 0.5f };