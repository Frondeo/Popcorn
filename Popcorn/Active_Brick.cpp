#include "Active_Brick.h"

//AActive_Brick
//-------------------------------------------------------------------------
AActive_Brick::AActive_Brick()
   : Fade_Step(0)
{
}
//-------------------------------------------------------------------------
void AActive_Brick::Act(HWND hwnd)
{
   if (Fade_Step < Max_Fade_Step)
   {
      ++Fade_Step;
      InvalidateRect(hwnd, &Brick_Rect, FALSE);
   }

}

//-------------------------------------------------------------------------

void AActive_Brick::Draw(HDC hdc, RECT& paint_area)
{
   HPEN pen;
   HBRUSH brush;

   AsConfig::Create_Pen_Brush(41 - Fade_Step * (41 / Max_Fade_Step), 100 - Fade_Step * (100 / Max_Fade_Step), 246 - Fade_Step * (246 / Max_Fade_Step), pen, brush);

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);

   Brick_Rect.left = (AsConfig::X_Offset + AsConfig::Cell_Width) * AsConfig::Gl_scale;
   Brick_Rect.top = (AsConfig::Y_Offset + AsConfig::Cell_Height) * AsConfig::Gl_scale;
   Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Gl_scale;
   Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Gl_scale;

   RoundRect(hdc, Brick_Rect.left, Brick_Rect.top, Brick_Rect.right, Brick_Rect.bottom, 2 * AsConfig::Gl_scale, 2 * AsConfig::Gl_scale);
}
