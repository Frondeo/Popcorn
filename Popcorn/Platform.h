#pragma once

#include "Config.h"
//--------------------------------------------------------------------------
enum EPlatform_State
{
   EPS_Missing,
   EPS_Normal,
   EPS_Meltdown
};

//--------------------------------------------------------------------------
class AsPlatform
{
public:
   AsPlatform();

   void Init();
   void Act(HWND hwnd);
   void Redraw_Platform(HWND hwnd);
   void Draw(HDC hdc, RECT& paint_area);

   int X_Step;
   int X_Pos;
   int Width;
   int Meltdown_Y_Pos;

private:
   void Draw_Normas_State(HDC hdc, RECT& paint_area);
   void Draw_Meltdown_State(HDC hdc, RECT& paint_area);

   EPlatform_State Platform_State;

   int Inner_Width;

   RECT Platform_Rect, Prev_Platform_Rect;

   HPEN Arc_Pen, Circle_Pen, Platform_Pen;
   HBRUSH Circle_Brush, Platform_Brush;

   static const int Height = 7;
   static const int Circle_Size = 7;
};
//--------------------------------------------------------------------------
