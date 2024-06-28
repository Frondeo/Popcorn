﻿#pragma once

#include "Config.h"
#include "Level.h"

//ABall
//----------------------------------------------------------

class ABall
{
public:
   ABall();

   void Init();
   void Draw(HDC hdc, RECT& paint_area);
   void Move(HWND hwnd, ALevel* level, int platform_x_pos, int platform_width);


private:

   HPEN Ball_Pen;
   HBRUSH Ball_Brush;

   double Ball_Direction;
   double Ball_Speed;

   int Ball_X_Pos;
   int Ball_Y_Pos;

   RECT Ball_Rect, Prev_Ball_Rect;
};
//----------------------------------------------------------