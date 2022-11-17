#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>
#include <QPainter>
#include <vector>
#include"Figure.h"
#include"Line.h"
#include"Rect.h"
#include"Ellipse.h"
#include"Polygon.h"
#include"Freehand.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ViewWidget; }
QT_END_NAMESPACE

class ViewWidget : public QWidget
{
    Q_OBJECT

public:
    ViewWidget(QWidget *parent = nullptr); //构造函数
    void mousePressEvent(QMouseEvent* event);//鼠标点击发出响应（左右键，单双击）
    void mouseMoveEvent(QMouseEvent* event);//鼠标移动发出响应
    void mouseReleaseEvent(QMouseEvent* event);//鼠标释放发出响应（左右键，单双击）
    void mouseDoubleClickEvent(QMouseEvent* event);
    void set_figure_type_to_line(); //设置图元元素为Line；
    void set_figure_type_to_rect(); //设置图元元素为rect；
    void set_figure_type_to_ellipsee(); //设置图元元素为Ellipsee;
    void set_figure_type_to_polygon(); //设置图元元素为Polygon;
    void set_type_to_freehand();//设置图元元素为Freehand
    void paintEvent(QPaintEvent*);//实现所有的绘制功能
    void undo();
    ~ViewWidget();

    enum FigureType {
        kDefault = 0, //初始值
        kLine = 1,   //画线
        kRectangle = 2, //画正方形
        kEllipsee = 3,  //画椭圆
        kPolygon = 4, //画多边形
        kFreehand = 5, //自由画
    };


public slots:

protected:
    bool draw_status_; //当前绘制状态,true为绘制当前鼠标拖动的图元，false为不绘制
    bool poly;
    QPoint start_p;//当前绘制图元起点
    QPoint end_p; //当前绘制图元终点
    Ui::ViewWidget *ui;
    std::vector<Figure*> figure_array_; 
    
    FigureType figure_type_;
    Figure*figure;  //当前图元
};

#endif // VIEWWIDGET_H
