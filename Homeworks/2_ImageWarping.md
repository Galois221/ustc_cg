# 2_ImageWarping

### 作业要求

* 实现算法：阅读论文

–IDW方法和RBF方法

* 使用QImage类

–四个接口函数width(), height(), pixel(), setPixel()

* 求解线性方程组

–自己写或者使用Eigen库

---

### IDW算法原理

问题：给定 $n$ 对控制点 $(\mathbf{p} _ i,\mathbf{q _ i})$，$\mathbf{p} _ i,\mathbf{q} _ i\in\mathbb{R}^2$，$i=1,\dots,n$ ，得到一个函数 $\mathbf{f}:\mathbb{R}^2\to\mathbb{R}^2$，满足插值条件，即 $\mathbf{f}(\mathbf{p} _ i)=\mathbf{q} _ i,i=1,\dots,n$ 

局部插值函数 $\mathbf{f} _ i(\mathbf{p}):\mathbb{R}^2\to\mathbb{R}^2$ 满足 $f _ i(\mathbf{p} _ i)=\mathbf{q} _ i$，具体为

$$
\mathbf{f} _ i(\mathbf{p})=\mathbf{q} _ i+\mathbf{D} _ i(\mathbf{p}-\mathbf{p} _ i)
$$
其中 $\mathbf{D} _ i:\mathbb{R}^2\to\mathbb{R}^2$，满足 $\mathbf{D} _ i(\mathbf{0})=\mathbf{0}$ 

可选 $\mathbf{D} _ i$ 为线性变换

插值函数为

$$
\mathbf{f}(\mathbf{x})=\sum _ {i=1}^n w _ i(\mathbf{x})\mathbf{f} _ i(\mathbf{x})
$$

其中 $w _ i:\mathbb{R}^2\to\mathbb{R}$，为

$$
w _ i(\mathbf{x})=\frac{\sigma _ i(\mathbf{x})}{\sum _ {j=1}^n \sigma _ j(\mathbf{x})}
$$

$$
\sigma _ i(\mathbf{x})=\frac{1}{\Vert\mathbf{x}-\mathbf{x} _ i\Vert^\mu}
$$

其中 $\mu>1$ ，显然 $0\le w _ i(\pmb{x})\le 1$，且 $\sum _ {i=1}^n w _ i(\mathbf{x})=1$ 

定义能量

$$
\begin{aligned}
E _ i(\mathbf{D} _ i)
=&\sum _ {j=1,j\neq i}^n w _ {i}(\mathbf{p}_j)\left\Vert\mathbf{q} _ i+\left(\begin{array}{c}d _ {i,11} & d _ {i,12}\newline d _ {i,21} & d _ {i,22}\end{array}\right)(\mathbf{p} _ j-\mathbf{p} _ i)-\mathbf{q} _ j\right\Vert^2\newline
=&\sum _ {j=1,j\neq i}^n w _ {i}(\mathbf{p}_j)(
(d _ {i,11}(p _ {j,1}-p _ {i,1})+d _ {i,12}(p _ {j,2}-p _ {i,2})+q _ {i,1}-q _ {j,1})^2+\newline
&(d _ {i,21}(p _ {j,1}-p _ {i,1})+d _ {i,22}(p _ {j,2}-p _ {i,2})+q _ {i,2}-q _ {j,2})^2)
\end{aligned}
\\
令\frac{\partial E_i}{\partial d_{i,11}}=2\sum _ {j=1,j\neq i}^n w _ {i}(\mathbf{p}_j)(
(d _ {i,11}(p _ {j,1}-p _ {i,1})+d _ {i,12}(p _ {j,2}-p _ {i,2})+q _ {i,1}-q _ {j,1})\times(p _ {j,1}-p _ {i,1})=0
\\
则d_{i,11}(\sum_{j=1,i\neq j}^n w_i(\mathbf p_j)\times(p_{j,1}-p_{i,1})^2)+d_{i,12}(\sum_{j=1,i\neq j}^n w_i(\mathbf p_j)\times(p_{j,2}-p_{i,2})\times(p_{j,1}-p_{i,1}))
\\=-\sum_{j=1,i\neq j}^n w_i(\mathbf p_j)(q _ {i,1}-q _ {j,1})\times(p _ {j,1}-p _ {i,1})
$$

最小化该能量可求得 $\mathbf{D} _ i=\left(\begin{array}{c}d _ {i,11} & d _ {i,12}\newline d _ {i,21} & d _ {i,22}\end{array}\right)$ 

---

### RBF算法原理

插值函数

$$
\pmb{f}(\pmb{p})=\sum _ {i=1}^n \boldsymbol{\alpha} _ i R(\Vert\mathbf{p}-\mathbf{p} _ i\Vert)+A\mathbf{p}+\mathbf{b}
$$

其中权重系数 $\boldsymbol{\alpha} _ i\in\mathbb{R}^2$，$A\in\mathbb{R}^{2\times 2}$，$\mathbf{b}\in\mathbb{R}^2$，径向基函数 $R(d)=(d^2+r^2)^{\mu/2}$ 

要求满足插值条件

$$
\mathbf{f}(\mathbf{p} _ j)=\sum _ {i=1}^n\boldsymbol{\alpha} _ i R(\Vert\mathbf{p} _ j-\mathbf{p} _ i\Vert)+A\mathbf{p} _ j+\mathbf{b}=\mathbf{q} _ j,\quad j=1,\dots,n
$$

自由度每维有 $n+3$ 个（n+2+1）

可选的补充约束为

$$
\left[\begin{array}{c}
\mathbf{p} _ 1 & \dots & \mathbf{p} _ n\newline
1            & \dots & 1
\end{array}\right] _ {3\times n}
\left[\begin{array}{c}
\boldsymbol{\alpha^T} _ 1 \newline
\vdots \newline
\boldsymbol{\alpha^T} _ n
\end{array}\right] _ {n\times2}
=\mathbf{0} _ {3\times 2}
$$

这里取$$r=\min_{i\neq j}{~}d(\mathbf{p}_i,\mathbf{p}_j)$$列出具有n+3个未知数的方程组，利用Eigen库分别进行进行求解

### 实验效果

为了能够处理大数据，我们这里首先将顶点分割成若干个gap，然后对其中顶点进行Warping，再由顶点来找原像，便不会出现白缝现象：

|                             IDW                              |                             RBF                              |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| ![image-20221214164101208](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214164101208.png) | ![image-20221214164129299](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214164129299.png) |
| ![image-20221214164809052](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214164809052.png) | ![image-20221214164823541](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214164823541.png) |
| ![image-20221214165245560](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214165245560.png) | ![image-20221214165312898](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214165312898.png) |
| ![image-20221214163944584](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214163944584.png) | ![image-20221214163910596](C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214163910596.png) |






### 思考和总结

* 用户交互体验有待改善，UI接口的设计可进一步完善
* 可以加入更多测试样例