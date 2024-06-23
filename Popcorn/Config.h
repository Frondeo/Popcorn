#pragma once

#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

//--------------------------------------------------------------------------
class AsConfig
{
public:
   static void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN& pen, HBRUSH& brush);

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
   static const int Max_Y_Pos = 199 - Ball_Size;
   static const int Y_Pos = 185; //Platform
   static const int FPS = 20;
   static const int Brick_Width = 15;
   static const int Brick_Height = 7;
};
//--------------------------------------------------------------------------

