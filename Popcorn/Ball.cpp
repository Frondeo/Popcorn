﻿#include "Ball.h"


//ABall
//----------------------------------------------------------
ABall::ABall()
   : Ball_Pen(0), Ball_Brush(0), Ball_X_Pos(20), Ball_Y_Pos(170), Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4), Ball_Rect{}, Prev_Ball_Rect{}
{
}
//----------------------------------------------------------
void ABall::Init()
{
   AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//----------------------------------------------------------
void ABall::Draw(HDC hdc, RECT& paint_area)
{
   RECT intersection_rect;
   if (!IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      return;
   //clear 
   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

   //draw ball
   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}

void ABall::Move(HWND hwnd, ALevel* level, int platform_x_pos, int platform_width)
{
   int next_x_pos;
   int next_y_pos;
   int max_x_pos = AsConfig::Max_X_Pos - AsConfig::Ball_Size;
   int platform_y_pos = AsConfig::Y_Pos - AsConfig::Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

   //----------------------------------------------------------
   if (next_x_pos < AsConfig::Border_X_Offset)
   {
      next_x_pos = AsConfig::X_Offset - (next_x_pos - AsConfig::X_Offset);
      Ball_Direction = M_PI - Ball_Direction;
   }
   //----------------------------------------------------------

   if (next_y_pos < AsConfig::Border_Y_Offset)
   {
      next_y_pos = AsConfig::Border_Y_Offset - (next_y_pos - AsConfig::Border_Y_Offset);
      Ball_Direction = -Ball_Direction;
   }
   //----------------------------------------------------------

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = AsConfig::Max_X_Pos - (next_x_pos - max_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }
   //----------------------------------------------------------
   if (next_y_pos > AsConfig::Max_Y_Pos)
   {
      next_y_pos = AsConfig::Max_Y_Pos - (next_y_pos - AsConfig::Max_Y_Pos);
      Ball_Direction = M_PI + (M_PI - Ball_Direction);
   }
   //----------------------------------------------------------
   if (next_y_pos > platform_y_pos)
   {
      if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         Ball_Direction = M_PI + (M_PI - Ball_Direction);
      }
   }

   level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

   //Move ball
   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = Ball_X_Pos * AsConfig::Gl_scale;
   Ball_Rect.top = Ball_Y_Pos * AsConfig::Gl_scale;
   Ball_Rect.right = Ball_Rect.left + AsConfig::Ball_Size * AsConfig::Gl_scale;
   Ball_Rect.bottom = Ball_Rect.top + AsConfig::Ball_Size * AsConfig::Gl_scale;

   InvalidateRect(hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(hwnd, &Ball_Rect, FALSE);
}
//-------------------------------------------------------------------------