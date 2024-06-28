#include "Platform.h"


//APlatform
//-------------------------------------------------------------------------
AsPlatform::AsPlatform()
   : Inner_Width(21), X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Gl_scale * 2), Width(28), Platform_Rect{}, Prev_Platform_Rect{},
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
void  AsPlatform::Redraw_Platform(HWND hwnd)
{
   Prev_Platform_Rect = Platform_Rect;

   Platform_Rect.left = X_Pos * AsConfig::Gl_scale;
   Platform_Rect.top = AsConfig::Y_Pos * AsConfig::Gl_scale;
   Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Gl_scale;
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Gl_scale;

   InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
   InvalidateRect(hwnd, &Platform_Rect, FALSE);
}

//--------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT& paint_area)
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
//-------------------------------------------------------------------------
