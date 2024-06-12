
#include "Engine.h"

char Level_one[AsEngine::Level_Height][AsEngine::Level_Width] =

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

AsEngine::AsEngine()
:  Inner_Width(21), Platform_X_Pos(Border_X_Offset), Platform_X_Step(Gl_scale * 2), Platform_Width(28), Ball_X_Pos(20), Ball_Y_Pos(170),
   Ball_Speed(3.0), Ball_Direction(M_PI - M_PI_4)
{
}

void AsEngine::Init_Engine(HWND hwnd) //инициализирует карандаш и кисть
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
   Create_Pen_Brush(41, 100, 246, Border_Blue_Pen, Border_Blue_Brush);
   Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);

   Level_Rect.left = X_Offset * Gl_scale;
   Level_Rect.top = Y_Offset * Gl_scale;
   Level_Rect.right = Level_Rect.left + Cell_Width * Level_Width * Gl_scale;
   Level_Rect.bottom = Level_Rect.top + Cell_Height * Level_Height * Gl_scale;

   Redraw_Platform();

   SetTimer(Hwnd, Timer_ID, 50, 0);
}

void AsEngine::Draw_Frame(HDC hdc, RECT& paint_area)
{

   RECT intersection_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      Draw_Level(hdc);

   if (IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      Draw_Platform(hdc, Platform_X_Pos, Platform_Y_Pos);

   /*int i;
   for (i = 0; i < 16; i++)
   {
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Gl_scale, 100, ET_Blue, ELT_O, i);
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Gl_scale, 130, ET_Red, ELT_O, i);
   }*/
   if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      Draw_Ball(hdc, paint_area);

   Draw_Bounds(hdc, paint_area);
}

int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform_X_Pos -= Platform_X_Step;

      if (Platform_X_Pos <= Border_X_Offset)
         Platform_X_Pos = Border_X_Offset;

      Redraw_Platform();
      break;

   case EKT_Right:
      Platform_X_Pos += Platform_X_Step;

      if (Platform_X_Pos >= Max_X_Pos - Platform_Width + 1)
         Platform_X_Pos = Max_X_Pos - Platform_Width;

      Redraw_Platform();
      break;

   case EKT_Space:
      break;
   }

   return 0;
}

int AsEngine::On_Timer()
{
   Move_Ball();

   return 0;
}

//-------------------------------------------------------------------------
void AsEngine::Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));

}

void  AsEngine::Redraw_Platform()
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = Platform_X_Pos * Gl_scale;
   Platform_Rect.top = Platform_Y_Pos * Gl_scale;
   Platform_Rect.right = Platform_Rect.left + Platform_Width * Gl_scale;
   Platform_Rect.bottom = Platform_Rect.top + Platform_Height * Gl_scale;

   InvalidateRect(Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(Hwnd, &Platform_Rect, FALSE);
}

//--------------------------------------------------------------------------
void AsEngine::Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
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
void AsEngine::Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
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
void AsEngine::Draw_Brick_Letter(HDC hdc, int x, int y, EBrick_Type brick_type, ELetter_Type letter_type, int rotation_step)
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

//-------------------------------------------------------------------------
void AsEngine::Draw_Level(HDC hdc)
{
   int i, j;
   for (i = 0; i < Level_Height; i++)
      for (j = 0; j < Level_Width; j++)
         Draw_Brick(hdc, X_Offset + j * Cell_Width, Y_Offset + i * Cell_Height, (EBrick_Type)Level_one[i][j]);
}

//--------------------------------------------------------------------------
void AsEngine::Draw_Platform(HDC hdc, int x, int y)
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
void AsEngine::Draw_Ball(HDC hdc, RECT &paint_area)
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

void AsEngine::Draw_Border(HDC hdc, int x, int y, bool top_border)
{
   //Border Blue
   SelectObject(hdc, Border_Blue_Pen);
   SelectObject(hdc, Border_Blue_Brush);

   if (top_border)
      Rectangle(hdc, x * Gl_scale, (y + 1) * Gl_scale, (x + 4) * Gl_scale, (y + 4) * Gl_scale);
   else
      Rectangle(hdc, (x + 1) * Gl_scale, y * Gl_scale, (x + 4) * Gl_scale, (y + 4) * Gl_scale);

   //Border White
   SelectObject(hdc, Border_White_Pen);
   SelectObject(hdc, Border_White_Brush);
   
   if (top_border)
      Rectangle(hdc, x * Gl_scale, y * Gl_scale, (x + 4) * Gl_scale, (y + 1) * Gl_scale);
   else
      Rectangle(hdc, x * Gl_scale, y * Gl_scale, (x + 1) * Gl_scale, (y + 4) * Gl_scale);

   //Black point
   SelectObject(hdc, BG_Pen);
   SelectObject(hdc, BG_Brush);

   if (top_border)
      Rectangle(hdc, (x + 2) * Gl_scale, (y + 1) * Gl_scale, (x + 3) * Gl_scale, (y + 2) * Gl_scale);
   else
      Rectangle(hdc, (x + 2) * Gl_scale, (y + 1) * Gl_scale, (x + 3) * Gl_scale, (y + 2) * Gl_scale);
}

void AsEngine::Draw_Bounds(HDC hdc, RECT &paint_area)
{
   int i;

   for (i = 0; i < 50; i++)
      Draw_Border(hdc, 2, 1 + i * 4, false);

   for (i = 0; i < 50; i++)
      Draw_Border(hdc, 201, 1 + i * 4, false);

   for (i = 0; i < 50; i++)
      Draw_Border(hdc, 3 + i * 4, 0, true);
}

//----------------------------------------------------------
void AsEngine::Check_Level_Brick_Hit(int &next_y_pos)
{
   int i, j;
   int brick_y_pos = Y_Offset + Level_Height * Cell_Height;

   for (i = Level_Height - 1; i >= 0; i--)
   {
      for (j = 0; j < Level_Width; j++)
      {
         if (Level_one[i][j] == 0)
            continue;

         if (next_y_pos < brick_y_pos)
         {
            next_y_pos = brick_y_pos - (next_y_pos - brick_y_pos);
            Ball_Direction = -Ball_Direction;
         }
      }
      brick_y_pos -= Cell_Height;
   }
}

void AsEngine::Move_Ball()
{
   int next_x_pos;
   int next_y_pos;
   int max_x_pos = Max_X_Pos - Ball_Size;
   int platform_y_pos = Platform_Y_Pos - Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

   //----------------------------------------------------------
   if (next_x_pos < Border_X_Offset)
   {
      next_x_pos = X_Offset - (next_x_pos - X_Offset);
      Ball_Direction = M_PI - Ball_Direction;
   }
   //----------------------------------------------------------

   if (next_y_pos < Border_Y_Offset)
   {
      next_y_pos = Y_Offset - (next_y_pos - Y_Offset);
      Ball_Direction = -Ball_Direction;
   }
   //----------------------------------------------------------

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = Max_X_Pos - (next_x_pos - max_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }
   //----------------------------------------------------------
   if (next_y_pos > Max_Y_Pos)
   {
      next_y_pos = Max_Y_Pos - (next_y_pos - Max_Y_Pos);
      Ball_Direction = M_PI + (M_PI - Ball_Direction);
   }
   //----------------------------------------------------------

   if (next_y_pos > platform_y_pos)
   {
      if (next_x_pos >= Platform_X_Pos && next_x_pos <= Platform_X_Pos + Platform_Width)
      {
         next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
         Ball_Direction = M_PI + (M_PI - Ball_Direction);
      }
   }

   Check_Level_Brick_Hit(next_y_pos);

   //Move ball
   Ball_X_Pos = next_x_pos;
   Ball_Y_Pos = next_y_pos;

   Ball_Rect.left = Ball_X_Pos * Gl_scale;
   Ball_Rect.top =  Ball_Y_Pos * Gl_scale;
   Ball_Rect.right = Ball_Rect.left + Ball_Size * Gl_scale;
   Ball_Rect.bottom = Ball_Rect.top + Ball_Size * Gl_scale;

   InvalidateRect(Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(Hwnd, &Ball_Rect, FALSE);
}