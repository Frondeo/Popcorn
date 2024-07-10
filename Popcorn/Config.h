#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

//--------------------------------------------------------------------------

class AColor
{
public:
   AColor(unsigned char r, unsigned char g, unsigned char b);

   unsigned char R, G, B;
};

//--------------------------------------------------------------------------
class AsConfig
{
public:
   static void Setup_Color();
   static void Create_Pen_Brush(const AColor& color, HPEN& pen, HBRUSH& brush);
   static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);
   static int Rand(int range);

   static int Current_Timer_Tick;
   static const AColor BG_Color, Red_Brick_Color, Blue_Brick_Color;
   static HBRUSH BG_Brush;
   static HPEN BG_Pen;
   
   static const int Gl_scale = 3;
   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;
   static const int Level_Width = 12;
   static const int Level_Height = 14;
   static const int X_Offset = 8;
   static const int Y_Offset = 6;
   static const int Cell_Width = 16;
   static const int Cell_Height = 8;
   static const int Max_X_Pos = X_Offset + Cell_Width * Level_Width;
   static const int Ball_Size = 4;
   static const int Max_Y_Pos = 199;
   static const int Y_Pos = 185; //Platform
   static const int FPS = 20;
   static const int Brick_Width = 15;
   static const int Brick_Height = 7;
   static const int Red_R = 41;
   static const int Red_G = 246;
   static const int Red_B = 246;
   static const int Blue_R = 255;
   static const int Blue_G = 85;
   static const int Blue_B = 85;
};
//--------------------------------------------------------------------------

