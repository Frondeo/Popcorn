
class Dot
{
   void Draw();

   int X, Y;
};

class Line
{
public:
   void Draw();

   int X1, Y1;
   int X2, Y2;

private:
   int Get_Lenght();
};

class Circle
{
   void Draw();
   int Get_Lenght();
   int Get_Square();

   int X1, Y1;
   int Radius;
};

class Rectangle
{
   void Draw();
   int Get_Perimeter();
   int Get_Square();

   int X1, Y1;
   int X2, Y2;
};


Line line_1, line_2;
void Line::Draw()
{
   int Get_Lenght();
}

void Some_Function()
{
   Line c;

   c.X1 = 2;
   c.Y1 = 3;
   c.Draw();
}

