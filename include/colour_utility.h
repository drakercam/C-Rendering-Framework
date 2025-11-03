#ifndef COLOUR_UTILITY_H
#define COLOUR_UTILITY_H

#include "math_utility.h"

// COLOURS ARE JUST VECTOR4's

namespace Colour
{
    static inline const Vector4 Custom(float r, float g, float b, float a) { return { r, g, b, a }; }

    // Basic colours
    static constexpr Vector4 White        = { 1.0f, 1.0f, 1.0f, 1.0f };
    static constexpr Vector4 Black        = { 0.0f, 0.0f, 0.0f, 1.0f };
    static constexpr Vector4 Transparent  = { 0.0f, 0.0f, 0.0f, 0.0f };

    // Grayscale
    static constexpr Vector4 Gray         = { 0.5f, 0.5f, 0.5f, 1.0f };
    static constexpr Vector4 LightGray    = { 0.75f, 0.75f, 0.75f, 1.0f };
    static constexpr Vector4 DarkGray     = { 0.25f, 0.25f, 0.25f, 1.0f };

    // Primary & secondary
    static constexpr Vector4 Red          = { 1.0f, 0.0f, 0.0f, 1.0f };
    static constexpr Vector4 Green        = { 0.0f, 1.0f, 0.0f, 1.0f };
    static constexpr Vector4 Blue         = { 0.0f, 0.0f, 1.0f, 1.0f };
    static constexpr Vector4 Yellow       = { 1.0f, 1.0f, 0.0f, 1.0f };
    static constexpr Vector4 Magenta      = { 1.0f, 0.0f, 1.0f, 1.0f };
    static constexpr Vector4 Cyan         = { 0.0f, 1.0f, 1.0f, 1.0f };

    // Extended palette
    static constexpr Vector4 Orange       = { 1.0f, 0.5f, 0.0f, 1.0f };
    static constexpr Vector4 Pink         = { 1.0f, 0.41f, 0.71f, 1.0f };
    static constexpr Vector4 Purple       = { 0.5f, 0.0f, 0.5f, 1.0f };
    static constexpr Vector4 Violet       = { 0.56f, 0.0f, 1.0f, 1.0f };
    static constexpr Vector4 Indigo       = { 0.29f, 0.0f, 0.51f, 1.0f };
    static constexpr Vector4 Lime         = { 0.75f, 1.0f, 0.0f, 1.0f };
    static constexpr Vector4 Olive        = { 0.5f, 0.5f, 0.0f, 1.0f };
    static constexpr Vector4 Teal         = { 0.0f, 0.5f, 0.5f, 1.0f };
    static constexpr Vector4 Aqua         = { 0.0f, 1.0f, 1.0f, 1.0f };
    static constexpr Vector4 Navy         = { 0.0f, 0.0f, 0.5f, 1.0f };
    static constexpr Vector4 Maroon       = { 0.5f, 0.0f, 0.0f, 1.0f };
    static constexpr Vector4 Brown        = { 0.6f, 0.3f, 0.0f, 1.0f };
    static constexpr Vector4 Tan          = { 0.82f, 0.71f, 0.55f, 1.0f };
    static constexpr Vector4 Gold         = { 1.0f, 0.84f, 0.0f, 1.0f };
    static constexpr Vector4 Silver       = { 0.75f, 0.75f, 0.75f, 1.0f };
    static constexpr Vector4 Bronze       = { 0.8f, 0.5f, 0.2f, 1.0f };

    // Nature-inspired
    static constexpr Vector4 SkyBlue      = { 0.53f, 0.81f, 0.92f, 1.0f };
    static constexpr Vector4 RoyalBlue    = { 0.25f, 0.41f, 0.88f, 1.0f };
    static constexpr Vector4 ForestGreen  = { 0.13f, 0.55f, 0.13f, 1.0f };
    static constexpr Vector4 SeaGreen     = { 0.18f, 0.55f, 0.34f, 1.0f };
    static constexpr Vector4 DarkGreen    = { 0.0f, 0.39f, 0.0f, 1.0f };
    static constexpr Vector4 LightGreen   = { 0.56f, 0.93f, 0.56f, 1.0f };
    static constexpr Vector4 Coral        = { 1.0f, 0.5f, 0.31f, 1.0f };
    static constexpr Vector4 Salmon       = { 0.98f, 0.5f, 0.45f, 1.0f };
    static constexpr Vector4 DeepPink     = { 1.0f, 0.08f, 0.58f, 1.0f };
    static constexpr Vector4 HotPink      = { 1.0f, 0.41f, 0.71f, 1.0f };
    static constexpr Vector4 Crimson      = { 0.86f, 0.08f, 0.24f, 1.0f };
    static constexpr Vector4 FireBrick    = { 0.7f, 0.13f, 0.13f, 1.0f };
    static constexpr Vector4 Chocolate    = { 0.82f, 0.41f, 0.12f, 1.0f };
    static constexpr Vector4 SandyBrown   = { 0.96f, 0.64f, 0.38f, 1.0f };
    static constexpr Vector4 Beige        = { 0.96f, 0.96f, 0.86f, 1.0f };
    static constexpr Vector4 Mint         = { 0.6f, 1.0f, 0.6f, 1.0f };

    // Cool tones
    static constexpr Vector4 MidnightBlue = { 0.1f, 0.1f, 0.44f, 1.0f };
    static constexpr Vector4 SlateBlue    = { 0.42f, 0.35f, 0.8f, 1.0f };
    static constexpr Vector4 Turquoise    = { 0.25f, 0.88f, 0.82f, 1.0f };
    static constexpr Vector4 LightCyan    = { 0.88f, 1.0f, 1.0f, 1.0f };
    static constexpr Vector4 PaleBlue     = { 0.69f, 0.93f, 0.93f, 1.0f };
    static constexpr Vector4 SteelBlue    = { 0.27f, 0.51f, 0.71f, 1.0f };

    // Warm tones
    static constexpr Vector4 Peach        = { 1.0f, 0.89f, 0.71f, 1.0f };
    static constexpr Vector4 Rose         = { 1.0f, 0.0f, 0.5f, 1.0f };
    static constexpr Vector4 Plum         = { 0.87f, 0.63f, 0.87f, 1.0f };
    static constexpr Vector4 Lavender     = { 0.9f, 0.9f, 0.98f, 1.0f };
    static constexpr Vector4 Khaki        = { 0.76f, 0.69f, 0.57f, 1.0f };
    static constexpr Vector4 Wheat        = { 0.96f, 0.87f, 0.7f, 1.0f };
    static constexpr Vector4 Amber        = { 1.0f, 0.75f, 0.0f, 1.0f };
    static constexpr Vector4 Brick        = { 0.8f, 0.25f, 0.33f, 1.0f };
}

#endif