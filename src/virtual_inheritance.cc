#include <iostream>

using namespace std;

/*菱形继承*/
class A
{
public:
    int dataA;
    A() : dataA(10)
    {
    }
};

class B : virtual public A
{
public:
    int dataB;
    B() : dataB(20)
    {
    }
};

class C : virtual public A
{
public:
    int dataC;
    C() : dataC(30)
    {
    }
};

class D : public B, public C
{
public:
    int dataD;
    D() : dataD(40) {}
};

int main()
{
    D d;
    // 访问通过 B 继承的成员
    std::cout << "Data from B: " << d.dataA << ", " << d.dataB << std::endl;

    // 访问通过 C 继承的成员
    std::cout << "Data from C: " << d.dataA << ", " << d.dataC << std::endl;

    // 访问通过 D 自身的成员
    std::cout << "Data from D: " << d.dataA << ", " << d.dataB << ", " << d.dataC << ", " << d.dataD << std::endl;

    return 0;
}
