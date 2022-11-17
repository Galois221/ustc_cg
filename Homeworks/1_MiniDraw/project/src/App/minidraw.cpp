#include "minidraw.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QToolBar>

MiniDraw::MiniDraw(QWidget* parent)
	: QMainWindow(parent)//有参数parent不为空，则QMainWindow会成为该指针所指窗口的子窗口
{
	ui.setupUi(this);  //设置ui界面
	view_widget_ = new ViewWidget();//创建ViewWidget界面对象，并将指针赋值给view_widget_，创建的对象会在析构函数中释放
	Creat_Action();  //添加动作
	Creat_ToolBar();  //添加工具栏
	Creat_Menu();   //添加菜单

	setCentralWidget(view_widget_); //将 ViewWidget 控件设置为主窗口的中心位置
}
//创建动作
void MiniDraw::Creat_Action() {
	Action_About = new QAction(tr("&About"), this); //new QAction来创造动作
	connect(Action_About, &QAction::triggered, this, &MiniDraw::AboutBox);

	Action_Line = new QAction(tr("&Line"), this); 
	connect(Action_Line, &QAction::triggered, view_widget_,&ViewWidget::set_figure_type_to_line);

	Action_Rect = new QAction(tr("&Rect"), this);
	connect(Action_Rect, &QAction::triggered, view_widget_, &ViewWidget::set_figure_type_to_rect);

	Action_Ellipse = new QAction(tr("&Ellipse"), this);
	connect(Action_Ellipse, &QAction::triggered, view_widget_,&ViewWidget::set_figure_type_to_ellipsee);

	Action_Polygon = new QAction(tr("&Polygon"), this);
	connect(Action_Polygon, &QAction::triggered, view_widget_, &ViewWidget::set_figure_type_to_polygon);

	Freehand = new QAction(tr("&Freehand"), this);
	//connect(Freehand, &QAction::triggered,this, &MiniDraw::Init);
	connect(Freehand, &QAction::triggered, view_widget_, &ViewWidget::set_type_to_freehand);

	Undo = new QAction(tr("&Undo"), this);
	connect(Undo, &QAction::triggered, view_widget_, &ViewWidget::undo);
}

void MiniDraw::Creat_ToolBar() {
	pToolBar = addToolBar(tr("&Main"));//工具栏
	pToolBar->addAction(Action_About);
	pToolBar->addAction(Action_Line);
	pToolBar->addAction(Action_Rect);
	pToolBar->addAction(Action_Ellipse);
	pToolBar->addAction(Action_Polygon);
	pToolBar->addAction(Freehand);
	pToolBar->addAction(Undo);
}

void MiniDraw::Creat_Menu() {
	pMenu = menuBar()->addMenu(tr("&Tool")); //添加菜单
	pMenu->addAction(Action_About); //菜单添加动作About和Line
	pMenu->addAction(Action_Line);
	pMenu->addAction(Action_Rect);
	pMenu->addAction(Action_Polygon);
	pMenu->addAction(Freehand);
	pMenu->addAction(Undo);
}

void MiniDraw::AboutBox() {
	QMessageBox::about(this, tr("About"), tr("MiniDraw app"));
	//显示一个带有标题和文本的框
}

void MiniDraw::Init() {
	view_widget_ = new ViewWidget();
	setCentralWidget(view_widget_);//将 ViewWidget 控件设置为主窗口的中心位置
}
MiniDraw::~MiniDraw() {
	delete view_widget_;
}
