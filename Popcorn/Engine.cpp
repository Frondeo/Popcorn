
#include "Engine.h"

char Level_one[ALevel::Level_Height][ALevel::Level_Width] =

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
void ABall::Draw(HDC hdc, RECT &paint_area, AsEngine *engine)
{
   RECT intersection_rect;
   if (! IntersectRect(&intersection_rect, &paint_area, &Ball_Rect))
      return;
      //clear 
   SelectObject(hdc, engine-> BG_Pen);
   SelectObject(hdc, engine-> BG_Brush);

   Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);

   //draw ball
   SelectObject(hdc, Ball_Pen);
   SelectObject(hdc, Ball_Brush);

   Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
}

void ABall::Move(AsEngine *engine, ALevel *level, AsPlatform *platform)
{
   int next_x_pos;
   int next_y_pos;
   int max_x_pos = AsEngine::Max_X_Pos - Ball_Size;
   int platform_y_pos = AsPlatform::Y_Pos - Ball_Size;

   Prev_Ball_Rect = Ball_Rect;

   next_x_pos = Ball_X_Pos + (int)(Ball_Speed * cos(Ball_Direction));
   next_y_pos = Ball_Y_Pos - (int)(Ball_Speed * sin(Ball_Direction));

   //----------------------------------------------------------
   if (next_x_pos < AsBorder::Border_X_Offset)
   {
      next_x_pos = ALevel::X_Offset - (next_x_pos - ALevel::X_Offset);
      Ball_Direction = M_PI - Ball_Direction;
   }
   //----------------------------------------------------------

   if (next_y_pos < AsBorder::Border_Y_Offset)
   {
      next_y_pos = AsBorder::Border_Y_Offset - (next_y_pos - AsBorder::Border_Y_Offset);
      Ball_Direction = -Ball_Direction;
   }
   //----------------------------------------------------------

   if (next_x_pos > max_x_pos)
   {
      next_x_pos = AsEngine::Max_X_Pos - (next_x_pos - max_x_pos);
      Ball_Direction = M_PI - Ball_Direction;
   }
   //----------------------------------------------------------
   if (next_y_pos > AsEngine::Max_Y_Pos)
   {
      next_y_pos = AsEngine::Max_Y_Pos - (next_y_pos - AsEngine::Max_Y_Pos);
      Ball_Direction = M_PI + (M_PI - Ball_Direction);
   }
   //----------------------------------------------------------
   if (next_y_pos > platform_y_pos)
   {
      if (next_x_pos >= platform->X_Pos && next_x_pos <= platform->X_Pos + platform->Width)
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
   Ball_Rect.right = Ball_Rect.left + Ball_Size * AsConfig::Gl_scale;
   Ball_Rect.bottom = Ball_Rect.top + Ball_Size * AsConfig::Gl_scale;

   InvalidateRect(engine->Hwnd, &Prev_Ball_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Ball_Rect, FALSE);
}
//-------------------------------------------------------------------------


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
   
   Level_Rect.left = ALevel::X_Offset * AsConfig::Gl_scale;
   Level_Rect.top = ALevel::Y_Offset * AsConfig::Gl_scale;
   Level_Rect.right = Level_Rect.left + ALevel::Cell_Width * ALevel::Level_Width * AsConfig::Gl_scale;
   Level_Rect.bottom = Level_Rect.top + ALevel::Cell_Height * ALevel::Level_Height * AsConfig::Gl_scale;
}

//-------------------------------------------------------------------------
void ALevel::Check_Level_Brick_Hit(int& next_y_pos, double &ball_direction)
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
            ball_direction= -ball_direction;
         }
      }
      brick_y_pos -= Cell_Height;
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

   if (! IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
      return;

   for (i = 0; i < Level_Height; i++)
      for (j = 0; j < Level_Width; j++)
         Draw_Brick(hdc, X_Offset + j * Cell_Width, Y_Offset + i * Cell_Height, (EBrick_Type)Level_one[i][j]);
}
//-------------------------------------------------------------------------


//APlatform
//-------------------------------------------------------------------------

AsPlatform::AsPlatform()
   : Inner_Width(21), X_Pos(AsBorder::Border_X_Offset), X_Step(AsConfig::Gl_scale * 2), Width(28), Platform_Rect{}, Prev_Platform_Rect{},
   Arc_Pen(0), Circle_Pen(0), Platform_Pen(0), Circle_Brush(0), Platform_Brush(0)
{
}

//-------------------------------------------------------------------------
void AsPlatform::Init()
{
   Arc_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   AsConfig::Create_Pen_Brush(151, 0, 0, Circle_Pen, Circle_Brush);
   AsConfig::Create_Pen_Brush(0, 128, 192, Platform_Pen, Platform_Brush);
}

//-------------------------------------------------------------------------
void  AsPlatform::Redraw_Platform(AsEngine *engine)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Gl_scale;
   Platform_Rect.top = Y_Pos * AsConfig::Gl_scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Gl_scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Gl_scale;

   InvalidateRect(engine->Hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(engine->Hwnd, &Platform_Rect, FALSE);
}

//--------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, AsEngine* engine, RECT& paint_area)
{
   int x = X_Pos;
   int y = Y_Pos;

   RECT intersection_rect;

   if (! IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;

   SelectObject(hdc, engine->BG_Pen);
   SelectObject(hdc, engine->BG_Brush);

   Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

   SelectObject(hdc, Circle_Pen);
   SelectObject(hdc, Circle_Brush);

   Ellipse(hdc, x * AsConfig::Gl_scale, y * AsConfig::Gl_scale, (x + Circle_Size) * AsConfig::Gl_scale, (y + Circle_Size) * AsConfig::Gl_scale);
   Ellipse(hdc, (x + Inner_Width) * AsConfig::Gl_scale, y * AsConfig::Gl_scale, (x + Circle_Size + Inner_Width) * AsConfig::Gl_scale, (y + Circle_Size) * AsConfig::Gl_scale);


   SelectObject(hdc, Arc_Pen);

   Arc(hdc, (x + 1) * AsConfig::Gl_scale, (y + 1) * AsConfig::Gl_scale, (x + Circle_Size - 1) * AsConfig::Gl_scale, (y + Circle_Size - 1) * AsConfig::Gl_scale,
      (x + 1 + 1) * AsConfig::Gl_scale, (y + 1) * AsConfig::Gl_scale, (x + 1) * AsConfig::Gl_scale, (y + 1 + 2) * AsConfig::Gl_scale);

   SelectObject(hdc, Platform_Pen);
   SelectObject(hdc, Platform_Brush);

   RoundRect(hdc, (x + 4) * AsConfig::Gl_scale, (y + 1) * AsConfig::Gl_scale, (x + 4 + Inner_Width - 1) * AsConfig::Gl_scale, (y + 1 + 5) * AsConfig::Gl_scale, 3 * AsConfig::Gl_scale, 3 * AsConfig::Gl_scale);
}
//-------------------------------------------------------------------------


//AsEngine
//-------------------------------------------------------------------------
AsEngine::AsEngine()
   : Hwnd(0), BG_Brush(0), BG_Pen(0)
{
}

void AsEngine::Init_Engine(HWND hwnd) //инициализирует карандаш и кисть
{
   Hwnd = hwnd;

   AsConfig::Create_Pen_Brush(15, 63, 31, BG_Pen, BG_Brush);


   Ball.Init();
   Level.Init();
   Platform.Init();
   Border.Init();

   Platform.Redraw_Platform(this);

   SetTimer(Hwnd, Timer_ID, 50, 0);
}

void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

      Level.Draw(hdc, paint_area);

      Platform.Draw(hdc, this, paint_area);

   /*int i;
   for (i = 0; i < 16; i++)
   {
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Gl_scale, 100, ET_Blue, ELT_O, i);
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Gl_scale, 130, ET_Red, ELT_O, i);
   }*/
 
   Ball.Draw(hdc, paint_area, this);

   Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);
}

int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKT_Left:
      Platform.X_Pos -= Platform.X_Step;

      if (Platform.X_Pos <= AsBorder::Border_X_Offset)
         Platform.X_Pos = AsBorder::Border_X_Offset;

      Platform.Redraw_Platform(this);
      break;

   case EKT_Right:
      Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = Max_X_Pos - Platform.Width;

      Platform.Redraw_Platform(this);
      break;

   case EKT_Space:
      break;
   }

   return 0;
}

int AsEngine::On_Timer()
{
   Ball.Move(this, &Level, &Platform);

   return 0;
}



