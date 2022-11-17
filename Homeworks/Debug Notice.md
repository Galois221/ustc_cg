# Debug Notice

* about() has been called:一般是下标超出了
* 借此机会赶快来理解一波const指针
* 为啥用向量pushback就不行，而用矩阵就可以
* 已经生成了内存就不要pushback了
* pushback进去的东西要是一个有内存的东西，而不能是一个简单的数值

- [ ] 引用

  * 引用没有const
  * 引用只能在定义时被初始化一次，之后不可变

- [ ] const指针：指针变量一旦经过初始化，就不能改变指针指向

  * int {*}const p (指向整型常量的指针,需要初始化)、const int{星}p(指向整型的常量指针) =int const {星}p（不需要初始化）、const int{星} const p(指向整型常量的常量指针)    const在星的左边和右边

  * const成员函数 : 不能修改对象的数据

  * 如果声明了一个指向const对象的指针（const int {星}p），则通过该指针只能调用const成员函数

- [ ] const 只能修饰输入参数：
如果输入参数采用“指针传递”，那么加const 修饰可以防止意外地改动该指针，起到保护作用。
* 将“const &”修饰输入参数的用法总结如下：
  (1)对于非内部数据类型的输入参数，应该将“值传递”的方式改为“const 引用传递”，目的是提高效率。例如将void Func(A a) 改为void Func(const A &a)。
  (2)对于内部数据类型的输入参数，不要将“值传递”的方式改为“const 引用传递”。否则既达不到提高效率的目的，又降低了函数的可理解性。例如void Func(int x) 不应该改为void Func(const int &x)。    

- [ ] const 修饰函数的返回值

  * 如果给以“指针传递”方式的函数返回值加const 修饰，那么函数返回值（即指针）的内容不能被修改，该返回值只能被赋给加const 修饰的同类型指针。例如函数
    const char * GetString(void);
    如下语句将出现编译错误：
    char *str = GetString();
    正确的用法是
    const char *str = GetString();

- [ ] 参考资料：

https://www.codersrc.com/archives/9091.html

https://zhuanlan.zhihu.com/p/110159656

函数前面＋&的意思：https://blog.csdn.net/qq_38124695/article/details/78188643

* floor:取整

```c++
setMouseTricking(true);//：鼠标跟踪生效，即使鼠标按键没有被按下，窗口部件也会接收鼠标移动事件。鼠标只要移动就会触发mouseMoveEvent(QMouseEvent *event)
```

