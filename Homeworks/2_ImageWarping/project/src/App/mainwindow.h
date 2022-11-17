#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
	class QAction;
	class QMenu;
	class ViewWidget;
	class QImage;
	class QPainter;
	class QRect;
	class ImageWidget;
	class QCheckBox;
	class QLabel;
	class QSlider;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *e);
	void paintEvent(QPaintEvent *paintevent);

private slots:
	
private:
	void CreateActions();
	void ConnectAcionsToWidget();
	void CreateMenus();
	void CreateToolBars();
	void CreateStatusBar();

private:
	Ui::MainWindowClass ui;

	QMenu		*menu_file_;
	QMenu		*menu_edit_;
	QMenu		*menu_help_;
	QToolBar	*toolbar_file_;
	QToolBar	*toolbar_edit_;
	QToolBar* toolbar_setting_;
	QAction		*action_new_;
	QAction		*action_open_;
	QAction		*action_save_;
	QAction		*action_saveas_;
	QAction		*action_invert_;
	QAction		*action_mirror_;
	QAction		*action_gray_;
	QAction		*action_restore_;
	QAction* action_idw_;
	QAction* action_rbf_;
	QAction* action_setting_;  
	QAction* action_clear_;
	QCheckBox* Warp_points;    //display warp_points or not
	QSlider* imgsize;          //adjust the image ratio
	QLabel* checkboxname;      //set the checkboxname
	QLabel* imagesize;         //set the table of imagesize

public:
	ImageWidget		*imagewidget_;
private slots:
	void CallIDW();
	void CallRBF();
	void set_image_ratio(int value);
};

#endif // MAINWINDOW_H
