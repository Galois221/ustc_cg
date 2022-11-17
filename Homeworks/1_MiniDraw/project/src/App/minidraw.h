#pragma once

#include <ui_minidraw.h>

#include <QtWidgets/QMainWindow>
#include <qmessagebox.h> //信号与槽
#include "viewwidget.h"

class MiniDraw : public QMainWindow {
	Q_OBJECT  //涉及到信号与槽的标志

public:
	MiniDraw(QWidget* parent = 0);
	~MiniDraw();

	QMenu* pMenu;
	QToolBar* pToolBar;
	QAction* Action_About;
	QAction* Action_Line;
	QAction* Action_Rect;
	QAction* Action_Ellipse;
	QAction* Action_Polygon;
	QAction* Freehand;
	QAction* Undo;

	void Creat_Menu();
	void Creat_ToolBar();
	void Creat_Action();
	void Init();
	void AboutBox();


private:
	Ui::MiniDrawClass ui;
	ViewWidget* view_widget_;
};
