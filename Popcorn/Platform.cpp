﻿#include "Platform.h"

// AsPlatform
//------------------------------------------------------------------------------------------------------------
AsPlatform::AsPlatform()
	: X_Pos(AsConfig::Border_X_Offset), X_Step(AsConfig::Gl_scale * 2), Platform_State(EPS_Normal), Inner_Width(21),
	Rolling_Step(0), Width(Normal_Width), Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Pen(0), Platform_Circle_Pen(0),
	Platform_Inner_Pen(0), Platform_Circle_Brush(0), Platform_Inner_Brush(0)
{
	X_Pos = (AsConfig::Max_X_Pos - Width) / 2;
}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Init()
{
	Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));

	AsConfig::Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);
	AsConfig::Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Act(HWND hwnd)
{
	if (Platform_State == EPS_Meltdown || Platform_State == EPS_Roll_In)
		Redraw_Platform(hwnd);
}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Set_State(EPlatform_State new_state)
{
	int i, len;

	if (Platform_State == new_state)
		return;

	if (new_state == EPS_Meltdown)
	{
		len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

		for (i = 0; i < len; i++)
			Meltdown_Platform_Y_Pos[i] = Platform_Rect.bottom;
	}
	
	Platform_State = new_state;
}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Redraw_Platform(HWND hwnd)
{
	Prev_Platform_Rect = Platform_Rect;

	Platform_Rect.left = X_Pos * AsConfig::Gl_scale;
	Platform_Rect.top = AsConfig::Y_Pos * AsConfig::Gl_scale;
	Platform_Rect.right = Platform_Rect.left + Width * AsConfig::Gl_scale;
	Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Gl_scale;

	if (Platform_State == EPS_Meltdown)
		Prev_Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 1) * AsConfig::Gl_scale;

	InvalidateRect(hwnd, &Prev_Platform_Rect, FALSE);
	InvalidateRect(hwnd, &Platform_Rect, FALSE);
}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw(HDC hdc, RECT& paint_area)
{// Рисуем платформу

	switch (Platform_State)
	{
	case EPS_Normal:
		Draw_Normal_State(hdc, paint_area);
		break;

	case EPS_Meltdown:
		Draw_Meltdown_State(hdc, paint_area);
		break;

	case EPS_Roll_In:
		Draw_Roll_In_State(hdc, paint_area);
		break;
	}
}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Circle_Hightlight(HDC hdc, int x, int y)
{
	// 2. Рисуем блик
	SelectObject(hdc, Highlight_Pen);

	Arc(hdc, x + AsConfig::Gl_scale, y + AsConfig::Gl_scale, x + (Circle_Size - 1) * AsConfig::Gl_scale, y + (Circle_Size - 1) * AsConfig::Gl_scale,
		x + 2 * AsConfig::Gl_scale, y + AsConfig::Gl_scale, x + AsConfig::Gl_scale, y + 3 * AsConfig::Gl_scale);

}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Normal_State(HDC hdc, RECT& paint_area)
{// Рисуем платформу в нормальном состоянии

	int x = X_Pos;
	int y = AsConfig::Y_Pos;
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		return;

	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);

	// 1. Рисуем боковые шарики
	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	Ellipse(hdc, x * AsConfig::Gl_scale, y * AsConfig::Gl_scale, (x + Circle_Size) * AsConfig::Gl_scale, (y + Circle_Size) * AsConfig::Gl_scale);
	Ellipse(hdc, (x + Inner_Width) * AsConfig::Gl_scale, y * AsConfig::Gl_scale, (x + Circle_Size + Inner_Width) * AsConfig::Gl_scale, (y + Circle_Size) * AsConfig::Gl_scale);

	Draw_Circle_Hightlight(hdc, x, y);

	// 3. Рисуем среднюю часть
	SelectObject(hdc, Platform_Inner_Pen);
	SelectObject(hdc, Platform_Inner_Brush);

	RoundRect(hdc, (x + 4) * AsConfig::Gl_scale, (y + 1) * AsConfig::Gl_scale, (x + 4 + Inner_Width - 1) * AsConfig::Gl_scale, (y + 1 + 5) * AsConfig::Gl_scale, 3 * AsConfig::Gl_scale, 3 * AsConfig::Gl_scale);
}

//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT& paint_area)
{// Рисуем платформу в расплавленном состоянии

	int i, j;
	int x, y;
	int y_offset;
	int area_width, area_height;
	COLORREF pixel;
	COLORREF bg_pixel = RGB(AsConfig::BG_Color.R, AsConfig::BG_Color.G, AsConfig::BG_Color.B);
	RECT intersection_rect;

	if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
		return;

	area_width = Width * AsConfig::Gl_scale;
	area_height = Height * AsConfig::Gl_scale + 1;

	for (i = 0; i < area_width; i++)
	{
		y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
		x = Platform_Rect.left + i;

		for (j = 0; j < area_height; j++)
		{
			y = Meltdown_Platform_Y_Pos[i] - j;

			pixel = GetPixel(hdc, x, y);
			SetPixel(hdc, x, y + y_offset, pixel);
		}

		for (j = 0; j < y_offset; j++)
		{
			y = Meltdown_Platform_Y_Pos[i] - area_height + 1 + j;
			SetPixel(hdc, x, y, bg_pixel);
		}

		Meltdown_Platform_Y_Pos[i] += y_offset;
	}
}
//------------------------------------------------------------------------------------------------------------
void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT &paint_area)
{
	int x = X_Pos * AsConfig::Gl_scale;
	int y = AsConfig::Y_Pos * AsConfig::Gl_scale;
	int roller_size = Circle_Size * AsConfig::Gl_scale;
	double alpha;
	XFORM xform, old_xform;

	SelectObject(hdc, Platform_Circle_Pen);
	SelectObject(hdc, Platform_Circle_Brush);

	Ellipse(hdc, x, y, x + roller_size, y + roller_size);

	SetGraphicsMode(hdc, GM_ADVANCED);

	alpha = 2.0 * M_PI / 32.0 * Rolling_Step;

	xform.eM11 = (float)cos(alpha);
	xform.eM12 = (float)sin(alpha);
	xform.eM21 = (float)-sin(alpha);
	xform.eM22 = (float)cos(alpha);
	xform.eDx  = (float)(x + roller_size / 2);
	xform.eDy  = (float)(y + roller_size / 2);
	GetWorldTransform(hdc, &old_xform);
	SetWorldTransform(hdc, &xform);

	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	Rectangle(hdc, - AsConfig::Gl_scale /2 , roller_size / 2, AsConfig::Gl_scale / 2, roller_size / 2);
	
	SetWorldTransform(hdc, &xform);

	Draw_Circle_Hightlight(hdc, x, y);
} 
//------------------------------------------------------------------------------------------------------------
