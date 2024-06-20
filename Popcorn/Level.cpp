﻿#include "Level.h"

char Level_one[AsConfig::Level_Height][AsConfig::Level_Width] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


//ALevel
//-------------------------------------------------------------------------
ALevel::ALevel()
   :Brick_Red_Pen(0), Brick_Blue_Pen(0), Letter_Pen(0), Brick_Red_Brush(0), Brick_Blue_Brush(0), Level_Rect{}
{
}

//-------------------------------------------------------------------------
void ALevel::Init()
{
   Letter_Pen = CreatePen(PS_SOLID, AsConfig::Gl_scale, RGB(255, 255, 255));

   AsConfig::Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
   AsConfig::Create_Pen_Brush(41, 100, 246, Brick_Blue_Pen, Brick_Blue_Brush);

   Level_Rect.left = AsConfig::X_Offset * AsConfig::Gl_scale;
   Level_Rect.top = AsConfig::Y_Offset * AsConfig::Gl_scale;
   Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width * AsConfig::Gl_scale;
   Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Gl_scale;
}

//-------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int& next_y_pos, double& ball_direction)
{
   int i, j;
   int brick_y_pos = AsConfig::Y_Offset + AsConfig::Level_Height * AsConfig::Cell_Height;

   for (i = AsConfig::Level_Height - 1; i >= 0; i--)
   {
      for (j = 0; j < AsConfig::Level_Width; j++)
      {
         if (Level_one[i][j] == 0)
            continue;

         if (next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
            ball_direction = -ball_direction;
         }
      }
      brick_y_pos -= AsConfig::Cell_Height;
   }
}

void ALevel::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{
   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case ET_None:
      return;

   case ET_Red:
      pen = Brick_Red_Pen;
      brush = Brick_Red_Brush;
      break;

   case ET_Blue:
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   RoundRect(hdc, x * AsConfig::Gl_scale, y * AsConfig::Gl_scale, (x + Brick_Width) * AsConfig::Gl_scale, (y + Brick_Height) * AsConfig::Gl_scale, 2 * AsConfig::Gl_scale, 2 * AsConfig::Gl_scale);
}

//--------------------------------------------------------------------------
void ALevel::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
   if (is_switch_color)
   {
      front_pen = Brick_Red_Pen;
      front_brush = Brick_Red_Brush;

      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Blue_Brush;
   }
   else
   {
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;

      back_pen = Brick_Red_Pen;
      back_brush = Brick_Red_Brush;
   }
}

//--------------------------------------------------------------------------
void ALevel::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
   //выводим падающую букву

   bool switch_color;
   double offset;
   double rotation_angle; // Преобразование шага в угол
   int brick_half_heigth = Brick_Height * AsConfig::Gl_scale / 2;
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(brick_type == ET_Blue || brick_type == ET_Red))
      return;

   //Корректировка шага вращения кирпича
   rotation_step = rotation_step % 16;

   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8L - (long long)rotation_step); //*


   if (rotation_step > 4 && rotation_step <= 12)
   {
      if (brick_type == ET_Blue)
         switch_color = true;
      else
         switch_color = false;
   }
   else
   {
      if (brick_type == ET_Red)
         switch_color = true;
      else
         switch_color = false;
   }

   Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);


   if (rotation_step == 4 || rotation_step == 12)
   {
      //Выводим фон 
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      Rectangle(hdc, x, y + brick_half_heigth - AsConfig::Gl_scale, x + Brick_Width * AsConfig::Gl_scale, y + brick_half_heigth);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_heigth, x + Brick_Width * AsConfig::Gl_scale, y + brick_half_heigth + AsConfig::Gl_scale - 1);
   }
   else
   {
      //Выводим фон 
      SetGraphicsMode(hdc, GM_ADVANCED);
      //матрица переворота
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)brick_half_heigth;
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0f * (1.0f - fabs(xform.eM22)) * (double)AsConfig::Gl_scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_heigth - back_part_offset, Brick_Width * AsConfig::Gl_scale, brick_half_heigth - back_part_offset);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -brick_half_heigth, Brick_Width * AsConfig::Gl_scale, brick_half_heigth);


      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * AsConfig::Gl_scale, -(5 * AsConfig::Gl_scale) / 2, 0 + 10 * AsConfig::Gl_scale, 5 * AsConfig::Gl_scale / 2);
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }
}

//-------------------------------------------------------------------------
void ALevel::Draw(HDC hdc, RECT& paint_area)
{
   int i, j;
   RECT intersection_rect;

   if (!IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      return;

   for (i = 0; i < AsConfig::Level_Height; i++)
      for (j = 0; j < AsConfig::Level_Width; j++)
         Draw_Brick(hdc, AsConfig::X_Offset + j * AsConfig::Cell_Width, AsConfig::Y_Offset + i * AsConfig::Cell_Height, (EBrick_Type)Level_one[i][j]);
}
//-------------------------------------------------------------------------
