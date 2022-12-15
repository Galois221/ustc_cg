# 3_PossionImageEditing

### 作业要求

- 实现 Siggraph 2003 论文 “Poisson Image Editing” 的算法 [下载](https://www.cs.jhu.edu/~misha/Fall07/Papers/Perez03.pdf)
  - 至少须实现文中的Seamless cloning的应用
  - 可执行示范程序可见课程百度云盘的目录 'Homework/Homework3/Demo'（百度云链接见课程主页）
- 实现[**多边形扫描转换算法**](ScanningLine.md)，可参考任一计算机图形学教材。
- 学习使用 Qt 多窗口显示框架：项目 [project/](../project/)  写好了一个多窗口框架，展示了如何得到每个子窗口的指针即可。详见说明文档。
- 学习使用图像库 OpenCV：`QImage` 类只是一个非常简单的图像类，本次作业须学习使用 OpenCV（Open Computer Vision）库。它是一个非常强大的图像处理的开发库，集成了很多算法。是从事图像处理和计算机视觉的研究工作者首选的图像库。我们提供了配置和使用 OpenCV 的示例项目 [opencv_example](opencv_example/)，请务必编译运行成功。

---

### Possion Image算法原理

对于原图像$$f (x, y)$$，新背景$$f ^{*}(x, y)$$ 和嵌入新背景后的新图像$$g(x, y)$$，满足：
$$
\Delta f=\Delta g \quad \text { over } \Omega, \text { with }\left.f\right|_{\partial \Omega}=\left.f^*\right|_{\partial \Omega}
$$
离散化，等价于求解一下方程组：
$$
\left|N_p\right| f_p-\sum_{q \in N_p \cap \Omega} f_q=\sum_{q \in N_p \cap \partial \Omega} f_q^*+\sum_{q \in N_p} v_{p q} \quad \forall p \in \Omega
$$

其中$$\partial{\Omega}=\{p\in S\setminus{\Omega}:N_p \cap\Omega\neq\emptyset\}$$，$$v_{pq}=g_p-g_q$$，$$N_P$$表示与像素$$p$$相邻的四个像素的集合

对于混合梯度，只需将$$v_{p q}$$做如下修改：
$$
v= \begin{cases}f_p^*-f_q^* & i f\left|f_p^*-f_q^*\right|>\left|g_p-g_q\right| \\ g_p-g_q & \text { otherwise }\end{cases}
$$
根据公式$$(2)$$，我们对每个分量，我们可以列出稀疏方程组进行求解



### 扫描线算法步骤

算法的基本思想是：通过维持一个特别的数据结构（结构中保存扫描线与多边形的交点）进行填充。

![https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/03_PoissonImageEditing/scan_line.jpg](https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/03_PoissonImageEditing/scan_line.jpg)

算法步骤：

- 用某个$$h\times w$$矩形框住图像
- 从图像的某个顶点起，依次记录每条线段的最低点的x值(x)、dx/dy(dx)、最高点的y值(ymax)，形成结构体，记录在线段较低点的y值数组```vector<vector<Vertex>>```中，记为V_EG(边表)(对于$$dx/dy=\infin$$的边，在这个过程就直接选中)
- 将V_EG(边表)转化为EG(交点表```vector<vector<Vertex>>```)：对$$i=1,2,...,h$$，对每个以$$i$$为最低y值的Vertex进行循环，从k=i直至k=ymax，在EG[k]中记录该交点的横坐标
- 对$$i=1,2,...,h$$，对EG[i]（表示以纵坐标为i的点的横坐标的构成的向量）的值进行冒泡排序
- 利用奇偶检测法，对矩形的内部进行填充

注：以上方法能够有效的排除奇异情况！

### 类图

![ClassDiagram](D:\Documents\ustc_cg\Homeworks\3_PoissonImageEditing\ClassDiagram.png)

### 测试结果

|                            图片1                             |                            图片2                             |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214204814505.png" alt="image-20221214204814505" style="zoom:80%;" /> | <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214204620787.png" alt="image-20221214204620787" style="zoom: 50%;" />![image-20221214204946370](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214204946370.png) |
|                             合成                             |                        合成（mixed）                         |
| <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214204427955.png" alt="image-20221214204427955" style="zoom:80%;" /> | <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214205217727.png" alt="image-20221214205217727" style="zoom:70%;" /> |

|                            图片1                             |                            图片2                             |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![image-20221214211355517](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214211355517.png) | ![image-20221214211415424](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214211415424.png) |
|                             合成                             |                        合成（mixed）                         |
| ![image-20221214211513058](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214211513058.png) | ![image-20221214211533911](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214211533911.png) |

|                            图片1                             |                            图片2                             |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![image-20221214212050676](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214212050676.png) | ![image-20221214212109980](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214212109980.png) |
|                             合成                             |                         合成(mixed)                          |
| ![image-20221214212032566](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214212032566.png) | ![image-20221214212041333](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214212041333.png) |

从以上测试样例中可以看出，mixed方法可以较好的保存现图像的底纹，更为“透明”；而普通编辑则可以原图像的全部梯度信息，在不同的使用场合各有优劣！

### 思考与总结

* 本次作业总体完成步骤较好：根据二八原则，首先看懂PossionImage算法原理，然后调用作业1的相关代码，先对矩形边界进行测试，再完成扫描线算法！
* OpenCv的行和列是反的！这里debug了好久。