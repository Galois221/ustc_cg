# 你的研0已经寄了

- [ ] MOC(Meta Object Compiler)
- [ ] 一时学不好不要着急，我大不了把时间堆在这上面，重新学一遍，也要拿下它
- [ ] 编程可是你以后工作之后的看家本领了

* tr() 翻译函数
* QPointer
* 类型转化函数：c语言风格强制转换、函数样式转换、类型转换（static_cast、dynamic_cast、const_cast、reintpret_cast）
* Lambda表达式：把函数看作对象
* 当有两个重载函数时，可以利用指针来显示指定
* static_type:
  * **static_cast** < type-id > ( expression )
* Qt 程序需要在`main()`函数创建一个`QCoreApplication`对象，然后调用它的`exec()`函数。这个函数就是开始 Qt 的事件循环。在执行`exec()`函数之后，程序将进入事件循环来监听应用程序的事件。当事件发生时，Qt 将创建一个事件对象。Qt 中所有事件类都继承于`QEvent`。在事件对象创建完毕后，Qt 将这个事件对象传递给`QObject`的`event()`函数。`event()`函数并不直接处理事件，而是按照事件对象的类型分派给特定的事件处理函数（event handler）
* **当重写事件回调函数时，时刻注意是否需要通过调用父类的同名函数来确保原有实现仍能进行！**Qt 中的事件都是 protected 的

- [ ] incline内置函数：为了消除调用的时空开销

* **const常引用**
* `pixel(i,j)`返回坐标（i，j)处的rgb值

- [ ] Qt::setpen()
- [ ] InitPoints为什么要用protected的形式

* 字母不要重复
* about() has been called:用lu分解和inverse函数会得到不同的结果？简直amazing

online judge(oj)

- [ ] QT鼠标事件循环系统
- [ ] QMdiArea:继承于QMainWindow，用于容纳多个子窗口
  * 

# Eigen库的使用：

注意误差的控制，

difine EPSILON 1e-16