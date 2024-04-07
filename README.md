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

### 3. 左值、右值

#### 左值

左值是可以被标识并且在赋值语句中放置值的表达式或对象。简而言之，左值是可以取地址的表达式。在C++中，变量、数据元素、类成员等都是左值。左值可以出现在赋值语句的左边和右边，可以被修改。

```c++
int x = 10; // x是左值，因为他是一个变量
int arr[5]; // arr[5]是左值，因为它是一个数组元素
int* ptr = &x; // &x是左值，因为它是x的地址
```

可以作为左值的类型有：
- 变量名、函数名以及数据成员名
- 返回左值引用的函数调用
- 由赋值运算符和复合赋值运算符连接的表达式，如(a=b，a-=b等)
- 解引用表达式 *ptr
- 前置自增和自减表达式(++a, --b)
- 成员访问运算符的结果
- 由指针访问成员运算符的结果
- 下标运算符的结果
- 字符串字面值

对于一个表达式，凡是对其取地址(&)操作可以成功的都是左值。

#### 右值

右值是不能被标识比并且在赋值语句中放置值的表达式和对象。简而言之，右值是临时的、无法取地址的值。在C++中，字面值、临时对象、函数返回值都是右值。右值只能出现在赋值语句的右边，不能被修改。

```c++
int y = 5; // 5是右值，因为它是一个字面值
int z = x + y; // x + y 是右值，因为它是一个临时值
```

##### 纯右值

自C++11开始，纯右值相当于之前的右值，右值的类型如下：

- 字面值或者函数返回的非引用都是纯右值
- 字面值（字符串字面值除外），例如1,'a',true等
- 返回值为非引用的函数调用或操作符重载。例如:str.substr(1,2)，str1+str2等
- 后置自增或自减表达式
- 算术表达式
- 逻辑表达式
- 比较表达式
- 取地址表达式
- lambda表达式


### 4. 序列化和反序列化

C++中的序列化通常用于将对象的状态转换为**可存储或传输的格式**。反序列化是将存储或传输的格式重新转换回对象的状态。

> 为什么需要序列化和反序列化

1. **数据持久化**：将对象的状态序列化到磁盘或数据库中，以便在程序关闭后能重新加载，这对于要长期存储或分享数据的应用程序非常重要。

2. **数据传输**：在网络上或不同进程之间传输数据时，将对象序列化为一种通用的格式（如JSON、XML、Protocal Buffers等），然后在另一端将其反序列化。这种方式可用于客户端-服务器通信、分布式系统等场景。

3. **跨平台兼容性**：序列化可以帮助解决不同平台或不同版本的应用程序之间的兼容性问题。通过将对象序列化为通用格式，可以保证数据在不同系统上的可移植性。

4. **数据备份和恢复**：通过序列化，可以轻松地对数据进行备份，并在需要时将其恢复到原始状态。

5. **状态传输**：在分布式系统中，序列化和反序列化可以用于将对象状态传输到另一个节点，以实现负载均衡或故障恢复。

> C++如何实现序列化

1. 自定义序列化
这是一种基本的方式，程序员需要手动编写代码来将对象的状态转换为字节流，并在需要时将字节流还原为对象。这种方式需要开发者自己处理对象的每个成员变量，并将其转换为字节流。虽然这种方式灵活性很高，但需要较多的手动工作，并且容易出错。
```C++
#include <iostream>
#include <fstream>

class MyClass {
private:
    int data1;
    double data2;
public:
    // 自定义序列化方法
    void serialize(std::ofstream& ofs) const {
        ofs.write(reinterpret_cast<const char*>(&data1), sizeof(data1));
        ofs.write(reinterpret_cast<const char*>(&data2), sizeof(data2));
    }

    // 自定义反序列化方法
    void deserialize(std::ifstream& ifs) {
        ifs.read(reinterpret_cast<char*>(&data1), sizeof(data1));
        ifs.read(reinterpret_cast<char*>(&data2), sizeof(data2));
    }
};

int main() {
    MyClass obj;
    obj.serialize(std::ofstream("data.bin", std::ios::binary));

    MyClass newObj;
    newObj.deserialize(std::ifstream("data.bin", std::ios::binary));

    return 0;
}
```
2. JSON和XML序列化
JSON（JavaScript Object Notation）和XML（eXtensible Markup Language）是两种通用的文本序列化格式，它们可以表示复杂的数据结构，并且易于阅读和理解。C++中有许多第三方库可以方便地将对象序列化为JSON或XML格式，例如RapidJSON、Boost.PropertyTree等。
```C++
#include <iostream>
#include <fstream>
#include <json/json.h>

class MyClass {
private:
    int data1;
    double data2;
public:
    // JSON序列化方法
    Json::Value toJson() const {
        Json::Value root;
        root["data1"] = data1;
        root["data2"] = data2;
        return root;
    }

    // JSON反序列化方法
    void fromJson(const Json::Value& root) {
        data1 = root["data1"].asInt();
        data2 = root["data2"].asDouble();
    }
};

int main() {
    MyClass obj;
    Json::Value jsonData = obj.toJson();
    std::ofstream ofs("data.json");
    ofs << jsonData;

    Json::Value newJsonData;
    std::ifstream ifs("data.json");
    ifs >> newJsonData;
    MyClass newObj;
    newObj.fromJson(newJsonData);

    return 0;
}
```
3. Protocol Buffers
Protocol Buffers是一种由Google开发的二进制序列化格式，它具有高效、紧凑、跨语言等特点。通过定义.proto文件，可以生成对应的序列化和反序列化代码，从而简化了开发过程。在C++中，可以使用Google提供的protobuf库来实现Protocol Buffers的序列化和反序列化功能。

假设我们有一个名为 message.proto 的Protocol Buffers定义文件，定义了一个包含int和double字段的消息。
```C++
syntax = "proto3";

message MyMessage {
    int32 data1 = 1;
    double data2 = 2;
}
```
然后我们可以使用Protocol Buffers编译器生成相应的C++代码：
```css
protoc --cpp_out=. message.proto
```
然后在C++代码中使用生成的代码来进行序列化和反序列化。

4. MessagePack
MessagePack是一种轻量级的二进制序列化格式，与JSON类似，但更加紧凑和高效。在C++中，有一些第三方库可以实现MessagePack的序列化和反序列化，例如msgpack-c。
```C++
#include <iostream>
#include <msgpack.hpp>

class MyClass {
private:
    int data1;
    double data2;
public:
    MSGPACK_DEFINE(data1, data2); // 定义MessagePack序列化宏

    // 在需要时还可以手动定义序列化和反序列化方法
};

int main() {
    MyClass obj;
    std::stringstream ss;
    msgpack::pack(ss, obj);

    msgpack::object_handle oh = msgpack::unpack(ss.str().data(), ss.str().size());
    msgpack::object obj2 = oh.get();
    MyClass newObj;
    obj2.convert(&newObj);

    return 0;
}
```
5. Boost.Serialization
Boost.Serialization是Boost库中提供的序列化框架，可以将C++对象序列化为二进制格式，也可以序列化为XML格式。它提供了高度的灵活性和可扩展性，但可能会增加代码的复杂性。
```C++
#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

class MyClass {
private:
    int data1;
    double data2;
public:
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & data1;
        ar & data2;
    }
};

int main() {
    MyClass obj;
    {
        std::ofstream ofs("data.txt");
        boost::archive::text_oarchive oa(ofs);
        oa << obj;
    }

    MyClass newObj;
    {
        std::ifstream ifs("data.txt");
        boost::archive::text_iarchive ia(ifs);
        ia >> newObj;
    }

    return 0;
}
```