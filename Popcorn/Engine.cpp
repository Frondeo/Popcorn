#include "Engine.h"

enum EBrick_Type
{
   ET_None,
   ET_Red,
   ET_Blue
};
enum EBall_Color
{
};
HPEN  Brick_Red_Pen, Brick_Blue_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

const int Gl_scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int X_Offset = 8;
const int Y_Offset = 6;

char Level_one[14][12] =
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
void Init()
{
   Brick_Red_Pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 85));
   Brick_Red_Brush = CreateSolidBrush(RGB(255, 85, 85));

   Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 255));
   Brick_Blue_Brush = CreateSolidBrush(RGB(255, 85, 255));
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
void Draw_Level(HDC hdc)
{
   int i, j;
   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Draw_Brick(hdc, X_Offset + j * Cell_Width, Y_Offset + i * Cell_Height, (EBrick_Type)Level_one[i][j]);
}
//--------------------------------------------------------------------------
void Draw_Frame(HDC hdc)
{
   Draw_Level(hdc);
}