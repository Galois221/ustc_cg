# 4_MinSurfMeshPara（学习=模仿＋重复）

### 课堂笔记

采样与重建：采样是真实频率的两倍（否则，欠拟合）

三角网格

设计：概要设计、详细设计、单元测试、重构

vernorli性质（Delaunay）: 圆的性质，最小角最大、凸包性质、内接圆的算术平均最大、低维点是高维点的凸包

Centroidal Voronoi Tessellation(Lloyd algorithm)

Use the Library"Triangle"

Geogram、TETGEN、CGAL

* 网格的数据结构：G=<V,E> Vertex degree、Face

  Duality:点对面，面对点，边对边

  

  三维曲面

  2.5D : Graph Embeding

  medh library:CGAL、Openmesh(matlab)Libigl

  半边结构

  distortion texture(纹理)

  参数化，纹理坐标:把边界固定到凸多边形上
  
---

### 作业要求

- 初步了解 \*.obj 格式的 3D 数据（\*.obj, \*.mtl)  
  - 使用 Win10 自带的 “3D 查看器” 查看 obj 数据文件
  - 安装并使用开源3D几何处理框架 [MeshLab](http://www.meshlab.net) 查看 3D 数据文件 （MeshLab 功能多，可多学习使用）
- 学习三角网格的数据结构及操作
  - 使用 UEngine 网格程序框架
  - 寻找**非封闭**网格曲面的边界
- 实现极小曲面与网格参数化
  - 极小曲面：边界固定，求解稀疏方程组
  - 参数化：边界映射到平面凸多边形，求解稀疏方程组
- 巩固使用 Eigen 库求解稀疏线性方程组

---

### 极小曲面

计算理论：
$$
\begin{cases}
-\delta_i=-v_i+\frac{1}{d_i}\sum_{v\in{N(i)}}v=\frac{1}{d_i}\sum_{v\in N_{i}}(v-v_i)=0&if{~~}v\notin{Boundary}
\\
v_i=v_i^*&if{~~}v\in{Boundary}
\end{cases}
$$
构建稀疏方程组

调用函数：

```C++
NumVertices();     //顶点数量

BoundaryVertice();

pos[0];  //第一个坐标

static_cast<int>(heMesh->Index(v)); //强制转化下标为整数

IsBoudary();

AdjVertices(); //adjacent 相邻的顶点

v->Degree();

pointf3;

vertices()[i]->pos.at(0);

Init();
```

### 实现步骤

- 检测边界
- 固定边界
- 构建稀疏方程组
- 求解稀疏方程组
- 更新顶点坐标

### 具体任务

- `class MinSurf` 为极小曲面 (minimal surface) 的类，在接口 `Minimize()` 中实现极小曲面的算法

### 实现效果

|                             原图                             |                           Minisurf                           |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="C:\Users\Galois\Desktop\picture_4\Minisurf\Balls_raw.png" alt="Balls_raw" style="zoom:50%;" /> | <img src="C:\Users\Galois\Desktop\picture_4\Minisurf\Balls.png" alt="Balls" style="zoom:50%;" /> |
| <img src="C:\Users\Galois\Desktop\picture_4\Minisurf\cat_raw.png" alt="cat_raw" style="zoom:50%;" /> | <img src="C:\Users\Galois\Desktop\picture_4\Minisurf\cat.png" alt="cat" style="zoom:50%;" /> |
| <img src="C:\Users\Galois\Desktop\picture_4\Minisurf\renlian_raw.png" alt="renlian_raw" style="zoom:50%;" /> | <img src="C:\Users\Galois\Desktop\picture_4\Minisurf\renlian2.png" alt="renlian2" style="zoom:50%;" /> |

---

### 网格参数化

计算理论：
$$
\begin{cases}
\delta_i=v_i-\sum_{v_{ij}\in{N(i)}}w_{ij}v_{ij}=\sum_{v_{ij}\in N_{i}}w_{ij}(v_i-v_{ij})=0&if{~~}v\notin{Boundary}
\\
v_i=v_i^*&if{~~}v\in{Boundary}
\end{cases}
$$


将曲面边界上的顶点映射到球面或者凸多边形上：（调用函数）

```c++
v->coor = tmp.cast_to<vecf2>(); //更新v坐标(用于trimesh)
heMesh->Boundaries();

//lambda表达式:lambda 表达式定义了一个匿名函数，并且可以捕获一定范围内的变量
[ capture ] ( params ) opt -> ret { body; };
//其中 capture 是捕获列表，params 是参数表，opt 是函数选项，ret 是返回值类型，body是函数体。

//添加按钮---Qcombobox
ComboBox *shape = new QComboBox();
vector<string> shape_list_ = {"Square", "Circle"};
auto ShapeSlot = [=](const string& item) {
	cout << "Select shape : " << shape->currentIndex() << endl;
};
grid->AddComboBox(shape, "ParamaterizeShape", shape_list_, ShapeSlot);

//指标间的相互转换
static_cast<int>(heMesh->Index(v));
V *v = he->Origin(); // V*v = v->Adjecent()[i];
```



### 实现步骤

- 检测边界
- 将边界映射到**正方形边界**或**圆边界** （只有这一步与上面的求极小曲面不同）
- 构建稀疏方程组
- 求解稀疏方程组
- 更新顶点坐标（bool Trimesh::Update(const std::vector<pointf3>&position)）
- 连接**纹理图像**，更新显示（bool Trimesh::Update(const std::vector<pointf2>&texcoords)）



### 实现效果

Normal Weight(Left)、Cotangent Weight（Right）

|                            Square                            |                            Circle                            |
| :----------------------------------------------------------: | :----------------------------------------------------------: |
| <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214221327036.png" alt="image-20221214221327036" style="zoom: 50%;" /> | <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214221551492.png" alt="image-20221214221551492" style="zoom:50%;" /> |
| <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214221911350.png" alt="image-20221214221911350" style="zoom:50%;" /> | <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214222035646.png" alt="image-20221214222035646" style="zoom:50%;" /> |
| <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214222134373.png" alt="image-20221214222134373" style="zoom:50%;" /> | <img src="C:\Users\Galois\AppData\Roaming\Typora\typora-user-images\image-20221214222317044.png" alt="image-20221214222317044" style="zoom:50%;" /> |

​        从测试图像可以看出，选择不同的边界、不同的权重会产生不同的参数化结果、纹理显示结果。其中，使用了余切权重的效果要比一般权重好一些，主要体现在测试图像与原三角网格比较相似，而一般权重则相对比较粗糙。

### 反思与改进

* 由于参数化是根据边界点的个数把点均匀的“铺”在正方形（圆形）边界。对于正方形边界，存在一些缺角的情况，此时可以考虑对于特殊情况特殊处理；而对于圆的边界，可以尝试边长的关系来获取角度，这样参数化的效果可能更好
* 对于Bunny_head，此算法会出现错误，估计可能是因为存在度为2的点，在参数化时可能会出现翻转