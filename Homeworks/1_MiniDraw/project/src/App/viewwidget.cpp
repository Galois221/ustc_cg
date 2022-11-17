#include "viewwidget.h"
#include "./ui_viewwidget.h"


ViewWidget::ViewWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewWidget)
{
    draw_status_ = false;
    poly = false; 
    figure_type_ = kDefault;
    ui->setupUi(this);
    figure = nullptr;
    start_p = end_p = QPoint(0, 0);
}

void ViewWidget::mousePressEvent(QMouseEvent* event) {
    if (Qt::LeftButton == event->button())//判断是否鼠标左击
    {
        switch (figure_type_)
        {
        case ViewWidget::kDefault:
            break;
        case ViewWidget::kLine:
            draw_status_ = true;
            figure = new Line();
            start_p = end_p = event->pos();
            figure->set_start_point_(start_p);
            figure->set_end_point_(end_p);
            break;
        case ViewWidget::kRectangle:
            draw_status_ = true;
            figure = new Rect();
            start_p = end_p = event->pos();
            figure->set_start_point_(start_p);
            figure->set_end_point_(end_p);
            break;
        case ViewWidget::kEllipsee:
            draw_status_ = true;
            figure = new Ellipsee();
            start_p = end_p = event->pos();
            figure->set_start_point_(start_p);
            figure->set_end_point_(end_p);
            break;
        case ViewWidget::kPolygon:
            draw_status_ = true;
            if (poly == true) {
                figure = new Polygonn();
                poly = false;
                start_p = end_p = event->pos();
                figure->set_start_point_(start_p);
                figure->set_end_point_(end_p);
            }
            else {
                start_p = event->pos();
                end_p = event->pos();
                figure->set_start_point_(start_p);
                figure->set_end_point_(end_p);
            }
            figure->points.push_back(end_p);
            setMouseTracking(true);
            break;
        case ViewWidget::kFreehand:
            draw_status_ = true;
            figure = new Freehand();
            start_p = end_p = event->pos();
            break;
        default:
            break;
        }
    }
}

void ViewWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (Qt::LeftButton == event->button() && figure_type_ == kPolygon) {
        poly = true;
        figure->points.push_back(end_p);
        figure_array_.push_back(figure);
        draw_status_ = false;
    }
}

void ViewWidget::mouseMoveEvent(QMouseEvent *event){
    if (draw_status_)//判断当前绘制状态
    {
        if (figure_type_ == kFreehand) {
            end_p = event->pos();
            figure->points.push_back(end_p);
        }
        else {
            end_p = event->pos();
            figure->set_end_point_(end_p);
        }
    }
}

void ViewWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (draw_status_) {
        switch (figure_type_) {
        case kDefault:
            break;
        case kLine:
            end_p = event->pos();
            figure->set_end_point_(end_p);
            figure_array_.push_back(figure);
            draw_status_ = false;
            break;
        case kRectangle:
            end_p = event->pos();
            figure->set_end_point_(end_p);
            figure_array_.push_back(figure);
            draw_status_ = false;
            break;
        case kEllipsee:
            end_p = event->pos();
            figure->set_end_point_(end_p);
            figure_array_.push_back(figure);
            draw_status_ = false;
            break;
        case kPolygon:
            end_p = event->pos();
            figure->set_end_point_(end_p);
            break;
        case kFreehand:
            end_p = event->pos();
            figure->points.push_back(end_p);
            figure_array_.push_back(figure);
            draw_status_ = false;
            break;
        }
    }
}

void ViewWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this); //定义painter在this指向的控件（此例为ViewWidget）中绘图
    //这一步是预演示
    if (draw_status_) {
        switch (figure_type_) {
        case kDefault:
            break;
        case kLine:
            figure->Draw(painter);
            break;
        case kRectangle:
            figure->Draw(painter);
            break;
        case kEllipsee:
            painter.setPen(QPen(QColor(0, 160, 230),2, Qt::DotLine));
            painter.drawRect(start_p.rx(), start_p.ry(), end_p.rx() - start_p.rx(), end_p.ry() - start_p.ry());
            painter.setPen(2);
            figure->Draw(painter);
            break;
        case kPolygon:
            for (int i = 0; i < figure->points.size() - 1; i++) painter.drawLine(figure->points[i], figure->points[i+1]);
            painter.drawLine(start_p, end_p);
            break;
        case kFreehand:
            figure->Draw(painter);
            break;
        default:
            break;
        }
    }
    for (int i = 0; i < figure_array_.size(); i++)
        figure_array_[i]->Draw(painter);
    update();
}
void ViewWidget::set_figure_type_to_line() {
    figure_type_ = kLine;
    draw_status_ = false;
}
void ViewWidget::set_figure_type_to_rect() {
    figure_type_ = kRectangle;
    draw_status_ = false;
}
void ViewWidget::set_figure_type_to_ellipsee() {
    figure_type_ = kEllipsee;
    draw_status_ = false;
}
void ViewWidget::set_figure_type_to_polygon() {
    figure_type_ = kPolygon;
    draw_status_ = false;
    poly = true;
}
void ViewWidget::set_type_to_freehand() {
    figure_type_ = kFreehand;
    draw_status_ = false;
}
void ViewWidget::undo() {
    figure_array_.pop_back();
    update();
}
ViewWidget::~ViewWidget()
{
    for (size_t i = 0; i < figure_array_.size(); i++) {
        if (figure_array_[i]) {
            delete figure_array_[i];
            figure_array_[i] = nullptr;
        }
    }
    
}

