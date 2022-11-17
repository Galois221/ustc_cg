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
    ViewWidget(QWidget *parent = nullptr); //���캯��
    void mousePressEvent(QMouseEvent* event);//�����������Ӧ�����Ҽ�����˫����
    void mouseMoveEvent(QMouseEvent* event);//����ƶ�������Ӧ
    void mouseReleaseEvent(QMouseEvent* event);//����ͷŷ�����Ӧ�����Ҽ�����˫����
    void mouseDoubleClickEvent(QMouseEvent* event);
    void set_figure_type_to_line(); //����ͼԪԪ��ΪLine��
    void set_figure_type_to_rect(); //����ͼԪԪ��Ϊrect��
    void set_figure_type_to_ellipsee(); //����ͼԪԪ��ΪEllipsee;
    void set_figure_type_to_polygon(); //����ͼԪԪ��ΪPolygon;
    void set_type_to_freehand();//����ͼԪԪ��ΪFreehand
    void paintEvent(QPaintEvent*);//ʵ�����еĻ��ƹ���
    void undo();
    ~ViewWidget();

    enum FigureType {
        kDefault = 0, //��ʼֵ
        kLine = 1,   //����
        kRectangle = 2, //��������
        kEllipsee = 3,  //����Բ
        kPolygon = 4, //�������
        kFreehand = 5, //���ɻ�
    };


public slots:

protected:
    bool draw_status_; //��ǰ����״̬,trueΪ���Ƶ�ǰ����϶���ͼԪ��falseΪ������
    bool poly;
    QPoint start_p;//��ǰ����ͼԪ���
    QPoint end_p; //��ǰ����ͼԪ�յ�
    Ui::ViewWidget *ui;
    std::vector<Figure*> figure_array_; 
    
    FigureType figure_type_;
    Figure*figure;  //��ǰͼԪ
};

#endif // VIEWWIDGET_H
