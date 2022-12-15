# 6_MassSpring(Curiousity+Passion)

## 计算机动画基础

### 视觉暂留现象 

每秒24桢

* 动画制作：剧本、草图、关键帧（高级工程师设计、创造力）、中间桢（考虑真实性：运动、材料）
* Warping和Morphing的区别:找对应点(Matching)、非线性插值（一个是个x和f，求f（x）;另外一个是给x和f(x)，求f）

动画：时间离散

$$x(t)$$、$$v(t)$$、$$a(t)$$

### 基于物理的仿真

* 运动学：运动方程
* 材料力学、弹性力学、流体力学
* pde数值解、最优化、并行计算

布料仿真、流体仿真、烟雾仿真

法向更新 CAD(designed)\CAE(engineering)\CAM(manufacture)

显示、隐式、半隐士欧拉法damper

* Cloth Simulation
* 常见的优化方法：Gradient/Newton/Quasi-Newton

Renderman、Pixar、VR&AR、Online Educate

* 善于问问题

### 作业要求

- 在给定的网格框架上完成作业，实现
  - 质点弹簧仿真模型的欧拉隐式方法
  - 质点弹簧仿真模型的加速模拟方法
- 学习使用 Tetgen 库生成四面体剖分

### 作业完成步骤

- 本次作业主要完成类 Simulation 中 **Init** 和 **SimulateOnce** 接口函数，前者主要初始化一些模拟用到的矩阵和辅助变量，会在每次更改模拟参数（如劲度系数）时调用从而修改相应的一些辅助变量；后者则是更新下一帧的位置position和速度velocity；
- 模拟只需要顶点的位置和边的索引信息（边索引为点的序号的列表，每两个序号表示一条边由哪两个点连接），均已在Simulation中建立完成

---

### 计算方法

由前 $n$ 帧信息，求得第 $n+1$ 帧信息（位移 $\boldsymbol x$，速度 $\boldsymbol v$）（设时间步长为 $h$）:
$$
\boldsymbol x_{n+1}=\boldsymbol x_n+h\boldsymbol v_{n+1},\\\boldsymbol v_{n+1}=\boldsymbol v_n+h\boldsymbol M^{-1}(\boldsymbol f_{int}(x_{n+1}) +\boldsymbol f_{ext}) =\boldsymbol v_n+h\boldsymbol M^{-1}\boldsymbol f(\boldsymbol x_{n+1}) ,\\\boldsymbol f_{int} = \{\boldsymbol f_{int,1},\boldsymbol f_{int,2},...,\boldsymbol f_{int,m}\}^T\quad \boldsymbol f_{int,j}:\mathcal{R}^{3m}\to\mathcal{R}^{3}表示第j个弹簧收到的内力
$$

其中$$\boldsymbol f_{int}:\mathcal{R}^{3m}\to\mathcal{R}^{3m},M:\mathcal R^{3m*3m}$$为质点的质量(对角阵)，内力：弹簧弹力、阻尼，外力：重力等

### 欧拉隐式方法

记
$$
\boldsymbol y =\boldsymbol x_n + h\boldsymbol v_n + h^2\boldsymbol M^{-1}\boldsymbol f_{ext}, \tag{*}\quad\boldsymbol y\in\mathcal{R}^{3m}
$$
则原问题转化为求解关于$\boldsymbol x$的方程：
$$
\boldsymbol x_{n+1}=\boldsymbol y +h^2\boldsymbol M^{-1}\boldsymbol f_{int}(\boldsymbol x_{n+1}),
\\
\boldsymbol g(\boldsymbol x) = \boldsymbol M(\boldsymbol x-\boldsymbol y) -h^2\boldsymbol f_{int}(\boldsymbol x) = 0,
$$
其中，$$\boldsymbol g:\mathcal{R}^{3m}\to\mathcal{R}^{3m}$$. 利用牛顿法求解该方程，主要迭代步骤：
$$
\boldsymbol x^{(k+1)}=\boldsymbol x^{(k)}-(\nabla \boldsymbol g(\boldsymbol x^{(k)}))^{-1}\boldsymbol g(\boldsymbol x^{(k)}).
$$

迭代初值可选为$\boldsymbol x^{(0)}=y$ .  迭代得到位移$x$后更新速度$\boldsymbol v_{n+1}=(\boldsymbol x_{n+1}-\boldsymbol x_{n})/h$

等价于求解矩阵：
$$
(\nabla\boldsymbol{g(x^{(k)})})(\boldsymbol{x^{(k)}-x^{(k+1)}})=\boldsymbol{g(x^{(k)})}
$$



上式中涉及关于弹力的求导，对于单个弹簧（端点为$\boldsymbol  x_1 \in \mathcal{R}^{3}$，$\boldsymbol  x_2$），劲度系数为$k$，原长为$l$，有：
$$
\boldsymbol x_1所受弹力：     \boldsymbol f_1(\boldsymbol x_1,\boldsymbol x_2)=k(||\boldsymbol x_1-\boldsymbol x_2||-l)\frac{\boldsymbol x_2-\boldsymbol x_1}{||\boldsymbol x_1-\boldsymbol x_2||},\\
\boldsymbol x_2所受弹力：     \boldsymbol f_2(\boldsymbol x_1,\boldsymbol x_2)=-\boldsymbol f_1(\boldsymbol x_1,\boldsymbol x_2),
$$
带入弹力公式得：
$$
\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1} =k(\frac{l}{||\boldsymbol r||}-1)\boldsymbol I-kl||\boldsymbol r||^{-3}\boldsymbol r \boldsymbol r^T,其中\boldsymbol r=\boldsymbol x_1-\boldsymbol x_2, \boldsymbol I为单位阵\\
$$


$$
\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_2}=-\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},
\frac{\partial  \boldsymbol f_2}{\partial \boldsymbol x_1}=-\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},
\frac{\partial  \boldsymbol f_2}{\partial \boldsymbol x_2}=\frac{\partial  \boldsymbol f_1}{\partial \boldsymbol x_1},
$$

对所有弹簧求导并组装即可求得力的导数（组装为稀疏矩阵，矩阵为对称阵）。

### 相关代码

```C++
//目标：更新Simulate中的+positions和velocity
//positions 质点的坐标向量 vector<pointf3>
//velocity 质点的速度向量 vector<pointf3>
//edgelist
//fixed_id 固定点的id地址值、fixed_positions
//stiff 弹簧的劲度系数

//SetFix 设置固定点，输入为索引编号
//SetLeftFix 固定网格x坐标最小的点
//SetVelocity
//Getstiff、Getmass、GettimeStep

//Tetgen 库的使用：d0cument<Tetmesh>
```

### 处理细节

* 完成步骤：g矩阵预分解(矩阵求导)——>固定点的处理——>解方程

* Init:弹簧长度，fixed_pos

### 加速方法

在上述欧拉方法中，对于内力（为保守力）有：
$$
\boldsymbol f_{int}(x)=-\nabla E(\boldsymbol x)
$$
故对方程$(*)$的求解可以转为为一个最小化问题：
$$
\boldsymbol x_{n+1}=\min\limits_{x}\frac{1}{2}(\boldsymbol x-\boldsymbol y)^T\boldsymbol M(\boldsymbol x-\boldsymbol y)+h^2E(\boldsymbol x)
$$
同时对于弹簧的弹性势能可以描述为一个最小化问题：
$$
\frac{1}{2}k(||\boldsymbol p_1-\boldsymbol p_2||-r)^2=\frac{1}{2}k \min\limits_{||\boldsymbol d||=r}||\boldsymbol p_1-\boldsymbol p_2-\boldsymbol d||^2,
$$
从而原问题转化为：
$$
\boldsymbol x_{n+1}=\min\limits_{x,\boldsymbol d\in\boldsymbol U}\frac{1}{2}\boldsymbol x^T(\boldsymbol M+h^2\boldsymbol L)\boldsymbol x-h^2\boldsymbol x^T\boldsymbol J \boldsymbol d-\boldsymbol x^T \boldsymbol M \boldsymbol y
$$
其中
$$
\boldsymbol U= \{ \boldsymbol d=(\boldsymbol d_1,\boldsymbol d_2,...,\boldsymbol d_s),\boldsymbol d_s\in R^3,||\boldsymbol d_i||=l_i \} (l_i为第i个弹簧原长),
$$
<img src="https://cdn.jsdelivr.net/gh/Ubpa/USTC_CG_Data@master/Homeworks/06_MassSpring/remark.PNG" alt="5" style="zoom: 67%;" />

从而可以对 $\boldsymbol x$，$\boldsymbol d$ 迭代优化求得该优化问题的解：
$$
\boldsymbol x 优化：求解方程(\boldsymbol M+h^2\boldsymbol L)\boldsymbol x=h^2\boldsymbol J \boldsymbol d+ \boldsymbol M \boldsymbol y（这里可以预分解矩阵），\\
$$

$$
\boldsymbol d 优化：\boldsymbol d_i=l_i\frac{\boldsymbol p_{i_1}-\boldsymbol p_{i_2}}{||\boldsymbol p_{i_1}-\boldsymbol p_{i_2}||}（这里l_i为第i个弹簧原长，\boldsymbol p_{i_1}，\boldsymbol p_{i_2}为其两端点），
$$

重复迭代过程直到收敛。

---

### 位移约束

将所有n个质点的坐标列为列向量 $x\in R^{3n}$，将所有 m 个自由质点坐标（无约束坐标）列为列向量 $x_f\in R^{3m}$,则两者关系：
$$
\boldsymbol x_f=\boldsymbol K\boldsymbol x,\\  \boldsymbol x=\boldsymbol K^T\boldsymbol x_f+\boldsymbol b,
$$
其中 $K\in R^{3m\times 3n}$ 为单位阵删去约束坐标序号对应行所得的稀疏矩阵，$b$ 为与约束位移有关的向量，计算为 $b=x-K^TKx$, 若约束为固定质点则 $b$ 为常量。由此我们将原本的关于 $x$ 的优化问题转化为对 $x_f$ 的优化问题：欧拉隐式方法中求解方程为：
$$
\boldsymbol g_1(\boldsymbol x_f) = K(\boldsymbol M(\boldsymbol x-\boldsymbol y) -h^2\boldsymbol f_{int}(\boldsymbol x)) = 0,\\
梯度：\nabla_{x_f} \boldsymbol g_1(\boldsymbol x_f) = K\nabla_{x} \boldsymbol g(\boldsymbol x)K^T,\\
$$
加速方法中优化问题中 $x$ 迭代步骤转化为求解关于 $\boldsymbol x_f$ 的方程：
$$
K(\boldsymbol M+h^2\boldsymbol L)K^T\boldsymbol x_f=K(h^2\boldsymbol J \boldsymbol d+ \boldsymbol M \boldsymbol y-(\boldsymbol M+h^2\boldsymbol L)\boldsymbol b)
$$



### 实验结果

见附加的视频