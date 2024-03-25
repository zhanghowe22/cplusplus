# cplusplus
This repository is used to record C++ study notes

## 基础知识点
### 1. 菱形继承 （解决方式：虚继承）
菱形继承是指一个派生类同时继承自两个间接基类，而这两个基类又直接或间接的继承自同一个基类的情况。这种继承关系形成了一个菱形结构，因此得名。
具体结构体如下：

      A
     / \
    B   C
     \ /
      D
在这个结构中，类 D 继承自类 B 和类 C，而类 B 和类 C 都直接或间接地继承自类 A。这样就形成了一个菱形继承的结构。

菱形继承可能会引发一些**问题**，主要包括以下几点：

(1) **二义性**：由于类‘D’继承了两个具有相同成员函数或成员变量的基类，当在派生类中使用这些成员时，可会出现二义性，编译器无法确定到底使用了哪一个基类成员。

(2) **资源浪费**：如果基类 B 和基类 C 都有各自的资源（如内存），而这些资源在类 A 中被重复继承，可能会导致资源的浪费。

为了解决菱形继承可能引发的问题，C++ 中提供了**虚继承**（Virtual Inheritance）的机制。通过在基类之间使用虚继承，可以确保只有一个基类的实例被共享，从而避免了二义性和资源浪费的问题。

**虚继承的实现原理**：
    
虚继承的底层实现涉及到虚函数表（vtable）和虚基类表（vtable）两个概念。

虚函数表（vtable）：在 C++ 中，每个包含虚函数的类都会生成一个虚函数表，其中存储了类的虚函数指针。虚函数表使得在运行时能够动态地根据对象的实际类型来调用正确的虚函数。对于每个含有虚函数的类，其对象中都包含一个指向其对应的虚函数表的指针。

虚基类表（vtable）：对于虚继承的类，编译器会在对象中生成一个虚基类表，其中存储了指向虚基类的偏移量。这个表使得派生类能够在运行时正确地访问虚基类的成员，即使多个路径继承了同一个虚基类，也能够保证只有一个实例被共享。

在虚继承的情况下，派生类需要通过虚基类表来访问虚基类的成员。当创建派生类的对象时，会调用虚基类的构造函数来初始化虚基类的成员，并在派生类对象中存储指向虚基类表的指针。这样，无论派生类通过哪条路径访问虚基类，都能够正确地访问到虚基类的成员。

总的来说，虚继承的底层实现涉及到虚函数表和虚基类表的管理，以及在派生类对象中存储这些表的指针。这样可以确保在派生类中正确地访问虚基类的成员，并且避免了由于多次继承而引发的二义性和资源浪费。


### 2. override、final的作用

C++11引入了这两个关键字，它们用于改进和加强面向对象编程中的虚函数的使用。

为什么要引入这两个关键字？

`override`关键字

`override`关键字用于明确指示一个子类的虚函数重写基类中的虚函数，它的作用是：

(1) 增强代码可读性，通过在派生类中使用`override`关键字，可以清晰的表明这个函数是重写了基类中的虚函数，而不是一个新函数。

(2) 防止意外的错误。如果在派生类中使用override关键字声明了一个函数，但这个函数与基类中的虚函数名称不匹配，编译器会产生错误，从而避免了因为拼写错误或函数签名不一致二导致的意外错误。

``` c++
#include <iostream>

class Base {
public:
    virtual void foo() {
        std::cout << "Base::foo()" << std::endl;
    }

    virtual void bar() const {
        std::cout << "Base::bar()" << std::endl;
    }
};

class Derived : public Base {
public:
    // 使用 override 关键字重写基类中的虚函数
    void foo() override {
        std::cout << "Derived::foo()" << std::endl;
    }

    // 使用 override 关键字重写基类中的虚函数，并指定常量成员函数
    void bar() const override {
        std::cout << "Derived::bar()" << std::endl;
    }

    // 使用 final 关键字阻止派生类进一步派生
    // virtual void baz() final {
    //     std::cout << "Derived::baz()" << std::endl;
    // }
};

int main() {
    Base* basePtr = new Derived();
    basePtr->foo(); // 输出 "Derived::foo()"
    basePtr->bar(); // 输出 "Derived::bar()"

    delete basePtr;

    return 0;
}
```

`final`关键字

`final`关键字用于禁止派生类对基类中的虚函数进行重写，或者禁止派生类继续派生。它的作用是：

(1) 明确禁止重写。在基类中使用`final`关键字可以明确的表示该虚函数不允许在派生类中进行重写。这对于一些核心的、不希望被修改的虚函数非常有用。

(2) 防止派生类进一步派生。在类的定义中使用`final`关键字可以防止该类被其他类继续派生。这对于一些设计上不希望被继承的类非常有用。

综上所述，override 和 final 关键字在 C++ 中的引入主要是为了增强代码的可读性、降低错误的发生概率，并在一定程度上提高了代码的安全性。

```c++
#include <iostream>

class Base {
public:
    // 声明为 final，阻止派生类对该函数进行重写
    virtual void foo() final {
        std::cout << "Base::foo()" << std::endl;
    }
};

// 试图继承自 Base 并重写 foo() 函数，会导致编译错误
// class Derived : public Base {
// public:
//     void foo() override {
//         std::cout << "Derived::foo()" << std::endl;
//     }
// };

int main() {
    Base baseObj;
    baseObj.foo(); // 输出 "Base::foo()"

    return 0;
}
```