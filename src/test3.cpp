#include <iostream>

class A {
private:
    int privateValue;
    A* aPtr;

public:
    A() : privateValue(10), aPtr(nullptr) {}

    // 设置aPtr为指向A的实例
    void setAPtr(A* ptr) { aPtr = ptr; }

    // 友元函数
    friend struct Sb;
};

A a;
struct Sb{
    void accessPrivate() {
        // 友元函数可以访问A的私有成员
        std::cout << "A's private value: " << a.privateValue << std::endl;
    }
};

// 全局对象 a

int main() {
    // 创建另一个A对象，并将其指针赋给a.aPtr

    // 调用友元函数，访问全局对象a的私有成员
    Sb t;
    t.accessPrivate();

    return 0;
}
