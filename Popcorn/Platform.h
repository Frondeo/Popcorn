#pragma once

#include "Config.h"

//--------------------------------------------------------------------------
class AsPlatform
{
public:
   AsPlatform();

   void Init();
   void Redraw_Platform(HWND hwnd);
   void Draw(HDC hdc, HPEN bg_pen, HBRUSH bg_brush, RECT& paint_area);

   int X_Step;
   int X_Pos;
   int Width;

private:
   int Inner_Width;

   RECT Platform_Rect, Prev_Platform_Rect;

   HPEN Arc_Pen, Circle_Pen, Platform_Pen;
   HBRUSH Circle_Brush, Platform_Brush;

   static const int Height = 7;
   static const int Circle_Size = 7;
};
//--------------------------------------------------------------------------
