#include "Border.h"




//AsBorder
//--------------------------------------------------------------------------
AsBorder::AsBorder()
   :Border_Blue_Pen(0), Border_White_Pen(0), Border_Blue_Brush(0), Border_White_Brush(0)
{
}

//--------------------------------------------------------------------------
void AsBorder::Init()
{
   AsConfig::Create_Pen_Brush(41, 100, 246, Border_Blue_Pen, Border_Blue_Brush);
   AsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}

//-------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN bg_pen, HBRUSH bg_brush)
{
   //Border Blue
   SelectObject(hdc, Border_Blue_Pen);
   SelectObject(hdc, Border_Blue_Brush);

   if (top_border)
      Rectangle(hdc, x * AsConfig::Gl_scale, (y + 1) * AsConfig::Gl_scale, (x + 4) * AsConfig::Gl_scale, (y + 4) * AsConfig::Gl_scale);
   else
      Rectangle(hdc, (x + 1) * AsConfig::Gl_scale, y * AsConfig::Gl_scale, (x + 4) * AsConfig::Gl_scale, (y + 4) * AsConfig::Gl_scale);

   //Border White
   SelectObject(hdc, Border_White_Pen);
   SelectObject(hdc, Border_White_Brush);

   if (top_border)
      Rectangle(hdc, x * AsConfig::Gl_scale, y * AsConfig::Gl_scale, (x + 4) * AsConfig::Gl_scale, (y + 1) * AsConfig::Gl_scale);
   else
      Rectangle(hdc, x * AsConfig::Gl_scale, y * AsConfig::Gl_scale, (x + 1) * AsConfig::Gl_scale, (y + 4) * AsConfig::Gl_scale);

   //Black point
   SelectObject(hdc, bg_pen);
   SelectObject(hdc, bg_brush);

   if (top_border)
      Rectangle(hdc, (x + 2) * AsConfig::Gl_scale, (y + 1) * AsConfig::Gl_scale, (x + 3) * AsConfig::Gl_scale, (y + 2) * AsConfig::Gl_scale);
   else
      Rectangle(hdc, (x + 2) * AsConfig::Gl_scale, (y + 1) * AsConfig::Gl_scale, (x + 3) * AsConfig::Gl_scale, (y + 2) * AsConfig::Gl_scale);
}

void AsBorder::Draw(HDC hdc, RECT& paint_area, HPEN bg_pen, HBRUSH bg_brush)
{
   int i;

   for (i = 0; i < 50; i++)
      Draw_Element(hdc, 2, 1 + i * 4, false, bg_pen, bg_brush);

   for (i = 0; i < 50; i++)
      Draw_Element(hdc, 201, 1 + i * 4, false, bg_pen, bg_brush);

   for (i = 0; i < 50; i++)
      Draw_Element(hdc, 3 + i * 4, 0, true, bg_pen, bg_brush);
}
//-------------------------------------------------------------------------
