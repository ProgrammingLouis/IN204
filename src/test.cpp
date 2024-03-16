

class A
{
  private:
    int a;
  
  public:
    A() : a(0) {}
    A(int a) : a(a) {}

    int getA() { return a; }
};

class B
{
  private:
    float b;
  
  public:
    B() : b(0) {}
    B(float b) : b(b) {}

    float getB() { return b; }
};

template <typename T>
class Vect2
{
  Vect2() = delete;
};

template <>
class Vect2<A>
{
public:
    Vect2() {}
    Vect2(A x, A y) : x(x), y(y) {}
    A x, y;
};

template <>
class Vect2<B>
{
public:
    Vect2() {}
    Vect2(B x, B y) : x(x), y(y) {}
    B x, y;

    void methodForB() {}
};


int main()
{
    Vect2<A> v1(1, 2);
    Vect2<B> v2(1.0, 2.0);
    return 0;
}