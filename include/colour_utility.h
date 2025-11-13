#ifndef COLOUR_UTILITY_H
#define COLOUR_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "math_utility.h"

// COLOURS ARE JUST VECTOR4's

#define Colour_Custom(r, g, b, a) ((Vector4){ r, g, b, a })

// Basic colours
#define Colour_White        ((Vector4){ 1.0f, 1.0f, 1.0f, 1.0f })
#define Colour_Black        ((Vector4){ 0.0f, 0.0f, 0.0f, 1.0f })
#define Colour_Transparent  ((Vector4){ 0.0f, 0.0f, 0.0f, 0.0f })

// Grayscale
#define Colour_Gray         ((Vector4){ 0.5f, 0.5f, 0.5f, 1.0f })
#define Colour_LightGray    ((Vector4){ 0.75f, 0.75f, 0.75f, 1.0f })
#define Colour_DarkGray     ((Vector4){ 0.25f, 0.25f, 0.25f, 1.0f })

// Primary & secondary
#define Colour_Red          ((Vector4){ 1.0f, 0.0f, 0.0f, 1.0f })
#define Colour_Green        ((Vector4){ 0.0f, 1.0f, 0.0f, 1.0f })
#define Colour_Blue         ((Vector4){ 0.0f, 0.0f, 1.0f, 1.0f })
#define Colour_Yellow       ((Vector4){ 1.0f, 1.0f, 0.0f, 1.0f })
#define Colour_Magenta      ((Vector4){ 1.0f, 0.0f, 1.0f, 1.0f })
#define Colour_Cyan         ((Vector4){ 0.0f, 1.0f, 1.0f, 1.0f })

// Extended palette
#define Colour_Orange       ((Vector4){ 1.0f, 0.5f, 0.0f, 1.0f })
#define Colour_Pink         ((Vector4){ 1.0f, 0.41f, 0.71f, 1.0f })
#define Colour_Purple       ((Vector4){ 0.5f, 0.0f, 0.5f, 1.0f })
#define Colour_Violet       ((Vector4){ 0.56f, 0.0f, 1.0f, 1.0f })
#define Colour_Indigo       ((Vector4){ 0.29f, 0.0f, 0.51f, 1.0f })
#define Colour_Lime         ((Vector4){ 0.75f, 1.0f, 0.0f, 1.0f })
#define Colour_Olive        ((Vector4){ 0.5f, 0.5f, 0.0f, 1.0f })
#define Colour_Teal         ((Vector4){ 0.0f, 0.5f, 0.5f, 1.0f })
#define Colour_Aqua         ((Vector4){ 0.0f, 1.0f, 1.0f, 1.0f })
#define Colour_Navy         ((Vector4){ 0.0f, 0.0f, 0.5f, 1.0f })
#define Colour_Maroon       ((Vector4){ 0.5f, 0.0f, 0.0f, 1.0f })
#define Colour_Brown        ((Vector4){ 0.6f, 0.3f, 0.0f, 1.0f })
#define Colour_Tan          ((Vector4){ 0.82f, 0.71f, 0.55f, 1.0f })
#define Colour_Gold         ((Vector4){ 1.0f, 0.84f, 0.0f, 1.0f })
#define Colour_Silver       ((Vector4){ 0.75f, 0.75f, 0.75f, 1.0f })
#define Colour_Bronze       ((Vector4){ 0.8f, 0.5f, 0.2f, 1.0f })

// Nature-inspired
#define Colour_SkyBlue      ((Vector4){ 0.53f, 0.81f, 0.92f, 1.0f })
#define Colour_RoyalBlue    ((Vector4){ 0.25f, 0.41f, 0.88f, 1.0f })
#define Colour_ForestGreen  ((Vector4){ 0.13f, 0.55f, 0.13f, 1.0f })
#define Colour_SeaGreen     ((Vector4){ 0.18f, 0.55f, 0.34f, 1.0f })
#define Colour_DarkGreen    ((Vector4){ 0.0f, 0.39f, 0.0f, 1.0f })
#define Colour_LightGreen   ((Vector4){ 0.56f, 0.93f, 0.56f, 1.0f })
#define Colour_Coral        ((Vector4){ 1.0f, 0.5f, 0.31f, 1.0f })
#define Colour_Salmon       ((Vector4){ 0.98f, 0.5f, 0.45f, 1.0f })
#define Colour_DeepPink     ((Vector4){ 1.0f, 0.08f, 0.58f, 1.0f })
#define Colour_HotPink      ((Vector4){ 1.0f, 0.41f, 0.71f, 1.0f })
#define Colour_Crimson      ((Vector4){ 0.86f, 0.08f, 0.24f, 1.0f })
#define Colour_FireBrick    ((Vector4){ 0.7f, 0.13f, 0.13f, 1.0f })
#define Colour_Chocolate    ((Vector4){ 0.82f, 0.41f, 0.12f, 1.0f })
#define Colour_SandyBrown   ((Vector4){ 0.96f, 0.64f, 0.38f, 1.0f })
#define Colour_Beige        ((Vector4){ 0.96f, 0.96f, 0.86f, 1.0f })
#define Colour_Mint         ((Vector4){ 0.6f, 1.0f, 0.6f, 1.0f })

// Cool tones
#define Colour_MidnightBlue ((Vector4){ 0.1f, 0.1f, 0.44f, 1.0f })
#define Colour_SlateBlue    ((Vector4){ 0.42f, 0.35f, 0.8f, 1.0f })
#define Colour_Turquoise    ((Vector4){ 0.25f, 0.88f, 0.82f, 1.0f })
#define Colour_LightCyan    ((Vector4){ 0.88f, 1.0f, 1.0f, 1.0f })
#define Colour_PaleBlue     ((Vector4){ 0.69f, 0.93f, 0.93f, 1.0f })
#define Colour_SteelBlue    ((Vector4){ 0.27f, 0.51f, 0.71f, 1.0f })

// Warm tones
#define Colour_Peach        ((Vector4){ 1.0f, 0.89f, 0.71f, 1.0f })
#define Colour_Rose         ((Vector4){ 1.0f, 0.0f, 0.5f, 1.0f })
#define Colour_Plum         ((Vector4){ 0.87f, 0.63f, 0.87f, 1.0f })
#define Colour_Lavender     ((Vector4){ 0.9f, 0.9f, 0.98f, 1.0f })
#define Colour_Khaki        ((Vector4){ 0.76f, 0.69f, 0.57f, 1.0f })
#define Colour_Wheat        ((Vector4){ 0.96f, 0.87f, 0.7f, 1.0f })
#define Colour_Amber        ((Vector4){ 1.0f, 0.75f, 0.0f, 1.0f })
#define Colour_Brick        ((Vector4){ 0.8f, 0.25f, 0.33f, 1.0f })

#ifdef __cplusplus
}
#endif

#endif