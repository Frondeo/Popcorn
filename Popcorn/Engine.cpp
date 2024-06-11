#include "Engine.h"
#define _USE_MATH_DEFINES
#include <math.h>

enum EBrick_Type
{
   ET_None,
   ET_Red,
   ET_Blue
};

enum ELetter_Type
{
   ELT_None,
   
   ELT_O
};

HWND Hwnd;
HPEN Arc_Pen, Letter_Pen, BG_Pen, Brick_Red_Pen, Brick_Blue_Pen, Circle_Pen, Platform_Pen, Ball_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, BG_Brush, Circle_Brush, Platform_Brush, Ball_Brush;

const int Gl_scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int X_Offset = 8;
const int Y_Offset = 6;
const int Level_Width = 14;
const int Level_Height = 12;
const int Circle_Size = 7;
const int Platform_Y_Pos = 185;
const int Platform_Height = 7;
const int Ball_Size = 4;
const int Max_X_Pos = X_Offset + Cell_Width * Level_Width - Ball_Size;
const int Max_Y_Pos = 199 - Ball_Size;


int Inner_Width = 21;
int Platform_X_Pos = 0;
int Platform_X_Step = Gl_scale * 2;
int Platform_Width = 28;

int Ball_X_Pos = 20;
int Ball_Y_Pos = 170;
//int Ball_X_Offset = 2;
//int Ball_Y_Offset = -2;
double Ball_Speed = 3.0;
double Ball_Direction = M_PI - M_PI_4;


RECT Platform_Rect, Prev_Platform_Rect;
RECT Level_Rect;
RECT Ball_Rect, Prev_Ball_Rect;


char Level_one[Level_Width][Level_Height]=

{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
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

//-------------------------------------------------------------------------
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));

}

void  Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = (X_Offset + Platform_X_Pos) * Gl_scale;
   Platform_Rect.top = Platform_Y_Pos * Gl_scale;
   Platform_Rect.right = Platform_Rect.left + Platform_Width * Gl_scale;
   Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Gl_scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}

void Init_Engine(HWND hwnd) //инициализирует карандаш и кисть
{
   Hwnd = hwnd;
   
   Arc_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   Letter_Pen = CreatePen(PS_SOLID, Gl_scale, RGB(255, 255, 255));

   Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);

   Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
   Create_Pen_Brush(41, 100, 246, Brick_Blue_Pen, Brick_Blue_Brush);
   Create_Pen_Brush(151, 0, 0, Circle_Pen, Circle_Brush);
   Create_Pen_Brush(0, 128, 192, Platform_Pen, Platform_Brush);
   Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);

   Level_Rect.left = X_Offset * Gl_scale;
   Level_Rect.top = Y_Offset * Gl_scale;
   Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Gl_scale;
   Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * Gl_scale;

   Redraw_Platform();

   SetTimer(Hwnd, Timer_ID, 50, 0);
}  

//--------------------------------------------------------------------------
void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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

   RoundRect(hdc, x * Gl_scale, y * Gl_scale, (x + Brick_Width) * Gl_scale, (y + Brick_Height) * Gl_scale, 2 * Gl_scale, 2 * Gl_scale);
}

//--------------------------------------------------------------------------

void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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

void Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
{
   //выводим падающую букву

   bool switch_color;
   double offset;
   double rotation_angle; // Преобразование шага в угол
   int brick_half_heigth = Brick_Height * Gl_scale / 2;
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

      Rectangle(hdc, x, y + brick_half_heigth - Gl_scale, x + Brick_Width * Gl_scale, y + brick_half_heigth);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, x, y + brick_half_heigth, x + Brick_Width * Gl_scale, y + brick_half_heigth + Gl_scale - 1);
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

      offset = 3.0f * (1.0f - fabs(xform.eM22)) * (double)Gl_scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_heigth - back_part_offset, Brick_Width * Gl_scale, brick_half_heigth - back_part_offset);

      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      Rectangle(hdc, 0, -brick_half_heigth, Brick_Width * Gl_scale, brick_half_heigth);


      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * Gl_scale, -(5 * Gl_scale) / 2, 0 + 10 * Gl_scale, 5 * Gl_scale / 2);
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }
}

//--------------------------------------------------------------------------

void Draw_Level(HDC hdc)
{
   int i, j;
   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Draw_Brick(hdc, X_Offset + j * Cell_Width, Y_Offset + i * Cell_Height, (EBrick_Type)Level_one[i][j]);
}

//--------------------------------------------------------------------------

void Draw_Platform(HDC hdc, int x, int y)
{ 

   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   SelectObject(hdc, Circle_Pen);
   SelectObject(hdc, Circle_Brush);

   Ellipse(hdc, x * Gl_scale, y * Gl_scale, (x + Circle_Size) * Gl_scale, (y + Circle_Size) * Gl_scale);
   Ellipse(hdc, (x + Inner_Width) * Gl_scale, y * Gl_scale, (x + Circle_Size + Inner_Width) * Gl_scale, (y + Circle_Size) * Gl_scale);


   SelectObject(hdc, Arc_Pen);

   Arc(hdc, (x + 1) * Gl_scale, (y + 1) * Gl_scale, (x + Circle_Size - 1) * Gl_scale, (y + Circle_Size - 1) * Gl_scale,
         (x + 1 + 1)* Gl_scale, (y + 1) * Gl_scale, (x + 1)* Gl_scale, (y + 1 + 2) * Gl_scale);

   SelectObject(hdc, Platform_Pen);
   SelectObject(hdc, Platform_Brush);

   RoundRect(hdc, (x + 4) * Gl_scale, (y + 1) * Gl_scale, (x + 4 + Inner_Width - 1) * Gl_scale, (y + 1 + 5) * Gl_scale, 3 * Gl_scale, 3 * Gl_scale);
}
void Draw_Ball(HDC hdc, RECT &paint_area)
{
   //clear 
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
   
   //draw ball
   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}



void Draw_Frame(HDC hdc, RECT &paint_area)
{
   
   RECT intersection_rect;
  
   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      Draw_Level(hdc);

   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      Draw_Platform(hdc, X_Offset + Platform_X_Pos, Platform_Y_Pos);

   /*int i;
   for (i = 0; i < 16; i++)
   {
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Gl_scale, 100, ET_Blue, ELT_O, i);
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Gl_scale, 130, ET_Red, ELT_O, i);
   }*/
   if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      Draw_Ball(hdc, paint_area);
}

int On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;
      Redraw_Platform();
      break;

   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;
      Redraw_Platform();
      break;

   case EKT_Space:
      break;
   }
   
   return 0;
}

void Move_Ball()
{
   int next_x_pos;
   int next_y_pos;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

   //---------------------------------------------------------------------
   if (next_x_pos < 0)
   {
      next_x_pos = -next_x_pos;
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos < Y_Offset)
   {
      next_y_pos = Y_Offset - (next_y_pos - Y_Offset);
      Ball_Direction = -Ball_Direction;
   }

   if (next_x_pos > Max_X_Pos)
   {
      next_x_pos = Max_X_Pos - (next_x_pos - Max_X_Pos);
      Ball_Direction = M_PI - Ball_Direction;
   }

   if (next_y_pos > Max_Y_Pos)
   {
      next_y_pos = Max_Y_Pos - (next_y_pos - Max_Y_Pos);
      Ball_Direction = M_PI + (M_PI - Ball_Direction);
   }
   //----------------------------------------------------------

   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = (X_Offset + Ball_X_Pos) * Gl_scale;
   Ball_Rect.top = (Y_Offset + Ball_Y_Pos) * Gl_scale;
   Ball_Rect.right = Ball_Rect.left + Ball_Size * Gl_scale;
   Ball_Rect.bottom = Ball_Rect.top + Ball_Size * Gl_scale;

   InvalidateRect(Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(Hwnd, &Ball_Rect, FALSE);
}


int On_Timer()
{
   Move_Ball();

   return 0;
}