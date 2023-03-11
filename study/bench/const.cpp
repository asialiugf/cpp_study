int main() {
    int          n1 = 0;  // 非 const 对象
    const int    n2 = 0;  // const 对象
    int const    n3 = 0;  // const 对象（同 n2）
    volatile int n4 = 0;  // volatile 对象
    const struct {
        int         n1;
        mutable int n2;
    } x = {0, 0};  // 带 mutable 成员的 const 对象

    n1 = 1;    // OK，可修改对象
               //  n2 = 2; // 错误：不可修改对象
    n4 = 3;    // OK，当做副作用
               //  x.n1 = 4; // 错误：const 对象的成员为 const
    x.n2 = 4;  // OK，const 对象的 mutable 成员不是 const

    const int& r1 = n1;        // 绑定到非 const 对象的 const 引用
                               //  r1 = 2; // 错误：试图通过到 const 的引用修改
    const_cast<int&>(r1) = 2;  // OK，修改非 const 对象 n1

    const int& r2 = n2;  // 绑定到 const 对象的 const 引用
    r2            = 2;   // 错误：试图通过到 const 的引用修改
    //  const_cast<int&>(r2) = 2; // 未定义行为：试图修改 const 对象 n2
}
