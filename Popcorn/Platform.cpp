#include "Platform.h"


//APlatform
//-------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Gl_scale * 2), Platform_State(EPS_Normal), Inner_Width(21), Meltdown_Y_Pos(0),
   Width(28), Platform_Rect{}, Prev_Platform_Rect{}, Arc_Pen(0), Circle_Pen(0), Platform_Pen(0), Circle_Brush(0), Platform_Brush(0)
{
   X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}

//-------------------------------------------------------------------------
void AsPlatform::Init()
{
   Arc_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   AsConfig::Create_Pen_Brush(151, 0, 0, Circle_Pen, Circle_Brush);
   AsConfig::Create_Pen_Brush(0, 128, 192, Platform_Pen, Platform_Brush);
}
void AsPlatform::Act(HWND hwnd)
{
   if (Platform_State != EPS_Meltdown)
   {
      Platform_State = EPS_Meltdown;
      Meltdown_Y_Pos = Platform_Rect.bottom;
   }

   if (Platform_State == EPS_Meltdown)
      Redraw_Platform(hwnd);
}
//-------------------------------------------------------------------------
void  AsPlatform::Redraw_Platform(HWND hwnd)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Gl_scale;
   Platform_Rect.top = AsConfig::Y_Pos * AsConfig::Gl_scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Gl_scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Gl_scale;

   if (Platform_State == EPS_Meltdown)
      Prev_Platform_Rect.bottom = Meltdown_Y_Pos;

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}


//--------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT& paint_area)
{
   
   switch(Platform_State)
   {
   case EPS_Normal:
      Draw_Normas_State(hdc, paint_area);
      break;
   case EPS_Meltdown:
      Draw_Meltdown_State(hdc, paint_area);
      break;
   }
}
//--------------------------------------------------------------------------
void AsPlatform::Draw_Normas_State(HDC hdc, RECT& paint_area)
{
   int x = X_Pos;
   int y = AsConfig::Y_Pos;

   RECT intersection_rect;

   if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;

   SelectObject(hdc, AsConfig::BG_Pen);
   SelectObject(hdc, AsConfig::BG_Brush);

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

//--------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
{
   int i, j;
   int x, y;
   int y_offset;
   int area_width, area_height;

   COLORREF pixel;

   area_width = Width * AsConfig::Gl_scale;
   area_height = Height * AsConfig::Gl_scale;

   for (i = 0; i < area_width; i++)
   {
      y_offset = 1;
      x = Platform_Rect.left + i;

      for (j = 0; j < area_height; j++)
      {
         y = Meltdown_Y_Pos - j;
         
         pixel = GetPixel(hdc, x, y);
         SetPixel(hdc, x, y + y_offset, pixel);
      }
   }
   ++Meltdown_Y_Pos;
}
