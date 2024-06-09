#include "Engine.h"
#define _USE_MATH_DEFINES
#include <math.h>

enum EBrick_Type
{
   ET_None,
   ET_Red,
   ET_Blue
};
enum EBall_Color
{
};
HPEN  Arc_Pen, Brick_Red_Pen, Brick_Blue_Pen, Circle_Pen, Platform_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Circle_Brush, Platform_Brush;

const int Gl_scale = 3;
const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int X_Offset = 8;
const int Y_Offset = 6;
const int Circle_Size = 7;

int Inner_Width = 21;


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
void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));

}


void Init() //�������������� �������� � �����
{
 
   Arc_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);
   Create_Pen_Brush(41, 100, 246, Brick_Blue_Pen, Brick_Blue_Brush);
   Create_Pen_Brush(151, 0, 0, Circle_Pen, Circle_Brush);
   Create_Pen_Brush(0, 128, 192, Platform_Pen, Platform_Brush);
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
void Draw_Brick_Letter(HDC hdc, int x, int y, int rotation_step)
{
   //������� �������� �����
   double offset;
   double rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step; // �������������� ���� � ����
   int brick_half_heigth = Brick_Height * Gl_scale / 2;
   int back_part_offset;

   XFORM xform, old_xform;

   if (rotation_step == 4 || rotation_step == 12)
   {
      //������� ��� 
      SelectObject(hdc, Brick_Red_Pen);
      SelectObject(hdc, Brick_Red_Brush);

      Rectangle(hdc, x, y + brick_half_heigth - Gl_scale, x + Brick_Width * Gl_scale, y + brick_half_heigth);

      SelectObject(hdc, Brick_Blue_Pen);
      SelectObject(hdc, Brick_Blue_Brush);

      Rectangle(hdc, x, y + brick_half_heigth, x + Brick_Width * Gl_scale, y + brick_half_heigth + Gl_scale - 1);
   }
   else
   {
      //������� ��� 
      SetGraphicsMode(hdc, GM_ADVANCED);
      //������� ����������
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)brick_half_heigth;
      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      SelectObject(hdc, Brick_Red_Pen);
      SelectObject(hdc, Brick_Red_Brush);

      offset = 3.0f * (1.0f - fabs(xform.eM22)) * (double)Gl_scale;
      back_part_offset = (int)round(offset);
      Rectangle(hdc, 0, -brick_half_heigth - back_part_offset, Brick_Width * Gl_scale, brick_half_heigth - back_part_offset);

      SelectObject(hdc, Brick_Blue_Pen);
      SelectObject(hdc, Brick_Blue_Brush);

      Rectangle(hdc, 0, -brick_half_heigth, Brick_Width * Gl_scale, brick_half_heigth);

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


void Draw_Frame(HDC hdc)
{
   //Draw_Level(hdc);
   //Draw_Platform(hdc, 50, 100);

   int i;
   for (i = 0; i < 16; i++)
      Draw_Brick_Letter(hdc, 20 + i * Cell_Width * Gl_scale, 100, i);
   
}