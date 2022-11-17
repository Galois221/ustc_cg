#include "minidraw.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QToolBar>

MiniDraw::MiniDraw(QWidget* parent)
	: QMainWindow(parent)//�в���parent��Ϊ�գ���QMainWindow���Ϊ��ָ����ָ���ڵ��Ӵ���
{
	ui.setupUi(this);  //����ui����
	view_widget_ = new ViewWidget();//����ViewWidget������󣬲���ָ�븳ֵ��view_widget_�������Ķ�����������������ͷ�
	Creat_Action();  //��Ӷ���
	Creat_ToolBar();  //��ӹ�����
	Creat_Menu();   //��Ӳ˵�

	setCentralWidget(view_widget_); //�� ViewWidget �ؼ�����Ϊ�����ڵ�����λ��
}
//��������
void MiniDraw::Creat_Action() {
	Action_About = new QAction(tr("&About"), this); //new QAction�����춯��
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
	pToolBar = addToolBar(tr("&Main"));//������
	pToolBar->addAction(Action_About);
	pToolBar->addAction(Action_Line);
	pToolBar->addAction(Action_Rect);
	pToolBar->addAction(Action_Ellipse);
	pToolBar->addAction(Action_Polygon);
	pToolBar->addAction(Freehand);
	pToolBar->addAction(Undo);
}

void MiniDraw::Creat_Menu() {
	pMenu = menuBar()->addMenu(tr("&Tool")); //��Ӳ˵�
	pMenu->addAction(Action_About); //�˵���Ӷ���About��Line
	pMenu->addAction(Action_Line);
	pMenu->addAction(Action_Rect);
	pMenu->addAction(Action_Polygon);
	pMenu->addAction(Freehand);
	pMenu->addAction(Undo);
}

void MiniDraw::AboutBox() {
	QMessageBox::about(this, tr("About"), tr("MiniDraw app"));
	//��ʾһ�����б�����ı��Ŀ�
}

void MiniDraw::Init() {
	view_widget_ = new ViewWidget();
	setCentralWidget(view_widget_);//�� ViewWidget �ؼ�����Ϊ�����ڵ�����λ��
}
MiniDraw::~MiniDraw() {
	delete view_widget_;
}
