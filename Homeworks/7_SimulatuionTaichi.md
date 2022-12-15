# 7_SimulationTaichi

### 网课笔记

刚体变换：平移、旋转（位置、朝向（角速度））

弹性体仿真、杨氏模量(Young Modules) E:pasical

应变Strian:$$\epsilon=\Delta L/L$$、应力Stress：$$\sigma=F/A$$、杨氏模量：$$E=\frac{\sigma}{\epsilon}=\frac{F/A}{\Delta L/L}$$

平衡方程：$$\rho \frac{d\boldsymbol v}{dt}=\nabla\cdot\sigma+g$$

弹性形变与塑性形变

本构方程 : 线性弹性模型、超弹性模型(应变梯度)

有限元方法：将模型离散为优先四面体的体积

* 流体仿真

Navier-Stokes Equation：
$$
\rho(\boldsymbol u_t+\boldsymbol u \cdot\nabla\boldsymbol u)=-\nabla p+\mu\Delta \boldsymbol u+\boldsymbol m
\\
s.t.\quad\nabla\cdot\boldsymbol u=0
$$
Houdini、Blender、IK(Inverse Kinematics)、Aninoji

数据驱动、虚拟人

Render：引擎、3D场景->2D图像、GPU:很多小单元可以同时计算一些小东西

物体坐标系、世界（统一）坐标系、相机坐标系、屏幕坐标系

mls-mpm88.cpp、imGUI

## 作业要求

- 构建不同的模拟场景★
  - 使用 `add_objec` 函数设计自己的模拟场景（可以改变模拟物体的形状、粒子数目、初始位置、初始速度等等）
- 探究参数空间：通过调整不同的参数设置体会不同参数对仿真结果的影响★★
  - 比如：探究杨氏模量E对雪模拟的影响，结合其背后的物理原理进行比较分析
- 探究其他算法和模型：通过改变物理模型、改变模拟算法实现新的模拟结果★★★
  - 比如：水冲击弹性物体的模拟
- 渲染与绘制（可选 ★★★★★）
  - 学习使用商业软件（如Houdini、Blender等）对结果进行渲染
  - 【注：3D粒子更合适光照渲染，因此此处不作要求】

---

### MPM方法

1.粒子（Particle）插值到格点(grid)：格点质量计算、格点速度计算

2.更新格点信息：格点受力计算、格点速度更新、碰撞处理

3.格点插值到粒子：粒子速度

4.更新粒子信息：粒子位置更新、形变梯度更新、粒子形变梯度增加约束

### MLS-MPM：example_00_mls_mpm88

MPM 的基本流程是：模拟对象的物理量存储在粒子中，在每一步执行 advance(dt) 函数将模拟向前推进 dt 时间，其中在每一步中都执行以下步骤，用辅助背景网格更新粒子的运动状态：

1．  P2G：将粒子（质量和动量）通过B样条插值到网格（lines 18-36）

2．  Update Grid：更新格点的位置（lines 37-46）

3．  G2P：将网格速度插值到粒子（lines 48-59）

4．  Update F：粒子形变梯度更新（lines 61）

5．  Plasticity：更新弹性和塑性梯度（lines 63-67）

6．  Advection：粒子位置更新（lines 60）

与传统 MPM 相比，MLS-MPM 只需要改变两个步骤：(2) 中格点上力的计算和 (4) 中粒子形变梯度F的更新。

* 参数说明

算法及代码中使用的部分参数如下：

- 仿真参数：网格密度 n，模拟步长 dt，框架时间frame_dt，画布canvas
- 物理参数：杨氏模量 E，泊松比 nu($$\nu$$)，重力加速度，$$F=\frac{\partial x}{\partial X}$$为变形梯度，硬度hardening
- 粒子参数：粒子采样的形状，粒子的初始速度，粒子的质量/密度，plastic(塑胶)、memet(塑胶)

$$
\sigma=C\epsilon \quad \sigma=\lambda tr(\epsilon)I+2\mu\epsilon
\\
\mu=\frac{E}{2(1+\nu)}\quad\lambda=\frac{E\nu}{(1-2\nu)(1+\nu)}
$$

### 杨氏模量E对雪模拟的影响

|                          $$E=10^3$$                          |                         $$E=5*10^3$$                         |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="D:\Documents\ustc_cg\Homeworks\7_SimulationTaichi\data\E=1e3.png" alt="E=1e3" style="zoom:40%;" /> | <img src="D:\Documents\ustc_cg\Homeworks\7_SimulationTaichi\data\E=1e4.png" alt="E=1e4" style="zoom:50%;" /> |
|                          $$E=10^4$$                          |                         $$E=2*10^4$$                         |
| <img src="D:\Documents\ustc_cg\Homeworks\7_SimulationTaichi\data\E=1e4.png" alt="E=1e4" style="zoom:50%;" /> | <img src="D:\Documents\ustc_cg\Homeworks\7_SimulationTaichi\data\E=2e4.png" alt="E=2e4" style="zoom:48%;" /> |

​     从上图可以看出，杨氏模量表示固体抵抗形变的能力，当杨氏模量比较小时，内部弹力很小，当杨氏模量增大时，内部应力增大，雪块将更加容易断裂。

### 相关仿真视频

见附加的视频

### 相关代码

```c++
/***********************************(1)*****************************************/
struct Particle {
    // Position and velocity
    Vec x, v;
    // Deformation gradient
    Mat F;
    // Affine momentum from APIC
    Mat C;
    // Determinant of the deformation gradient (i.e. volume)
    real Jp;
    // Color
    int c;
    //ptype
    int ptype; //0:fluid,1:jelly,2:snow

    Particle(Vec x, int c, Vec v = Vec(0),int ptype = 2) :
        x(x),
        v(v),
        F(1),
        C(0),
        Jp(1),
        c(c),
        ptype(ptype){}
};
////////////////////////////////////////////////////////////////////////////////
/***********************************(2)*****************************************/
    // Compute current Lamé parameters [http://mpm.graphics Eqn. 86]
    auto e = std::exp(hardening * (1.0_f - p.Jp));
    if (p.ptype == 1) e = 0.3;
    auto mu = mu_0 * e, lambda = lambda_0 * e;
    if (p.ptype == 0) mu = 0;
////////////////////////////////////////////////////////////////////////////////
/***********************************(3)*****************************************/
    if (p.ptype == 0) {
        p.F = Mat(1) * sqrt(determinant(F));
    }
    else if (p.ptype == 1) {
        p.F = F;
    }
    else if (p.ptype == 2) {
        Mat svd_u, sig, svd_v;
        svd(F, svd_u, sig, svd_v);

        // Snow Plasticity
        for (int i = 0; i < 2 * int(plastic); i++) {
            sig[i][i] = clamp(sig[i][i], 1.0f - 2.5e-2f, 1.0f + 7.5e-3f);
        }

        real oldJ = determinant(F);
        F = svd_u * sig * transposed(svd_v);

        real Jp_new = clamp(p.Jp * oldJ / determinant(F), 0.6f, 20.0f);

        p.Jp = Jp_new;
        p.F = F;
    }
  }
////////////////////////////////////////////////////////////////////////////////
/***********************************(4)*****************************************/
void add_object(Vec center, int c,Vec velocity = Vec(0.0_f),int ptype=2) {
    // Randomly sample 1000 particles in the square
    for (int i = 0; i < 1000; i++) {
        particles.push_back(Particle((Vec::rand() * 2.0f - Vec(1)) * 0.08f + center, c, velocity, ptype));
    }
}
void add_object_rectangle(Vec v1, Vec v2, int c, int num = 500, Vec velocity=Vec(0.0_f), int ptype = 0) {
    Vec box_min(min(v1.x, v2.x), min(v1.y, v2.y)), box_max(max(v1.x, v2.x), max(v1.y, v2.y));
    int i = 0;
    while (i < num) {
        auto pos = Vec::rand();
        if (pos.x > box_min.x && pos.x<box_max.x && pos.y>box_min.y && pos.y < box_max.y) {
            particles.push_back(Particle(pos, c, velocity, ptype));
            i++;
        }
    }
}
//add fluid
void add_jet() {
    add_object_rectangle(Vec(0.48, 0.05), Vec(0.52, 0.25), 0x87CEFA, 10, Vec(0.0, 18.0), 0);
}
void add_jet2() {
    add_object_rectangle(Vec(0.48, 0.75), Vec(0.52, 0.95), 0xED553B, 10, Vec(0.0, 0.0), 0);
}
////////////////////////////////////////////////////////////////////////////////
/***********************************(5)*****************************************/
  add_object(Vec(0.5, 0.65), 0xFFFAFA, Vec(0, 0), 2);
////////////////////////////////////////////////////////////////////////////////
/***********************************(6)*****************************************/
      if (step < 5e3)
      {
          add_jet();
          add_jet2();
      }
////////////////////////////////////////////////////////////////////////////////
```