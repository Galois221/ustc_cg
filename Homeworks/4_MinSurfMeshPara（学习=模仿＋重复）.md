# 4_MinSurfMeshPara（学习=模仿＋重复）

采样与重建：采样是真实频率的两倍（否则，欠拟合）

三角网格

设计：概要设计、详细设计、单元测试、重构

vernorli性质（Delaunay）:圆的性质，最小角最大、凸包性质、内接圆的算术平均最大、低维点是高维点的凸包

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

## 作业要求

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

### 实现步骤

- 检测边界
- 将边界映射到**正方形边界**或**圆边界** （只有这一步与上面的求极小曲面不同）
- 构建稀疏方程组
- 求解稀疏方程组
- 更新顶点坐标
- 连接**纹理图像**，更新显示