#include "mainwindow.h"
#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include "ImageWidget.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setGeometry(300, 150, 800, 450);

	imagewidget_ = new ImageWidget();
	setCentralWidget(imagewidget_);

	CreateActions();
	ConnectAcionsToWidget();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *e)
{

}

void MainWindow::paintEvent(QPaintEvent* paintevent)
{
	
}

void MainWindow::CreateActions()
{
	action_new_ = new QAction(QIcon(":/MainWindow/Resources/images/new.jpg"), tr("&New"), this); //action new : 
	action_new_->setShortcut(QKeySequence::New);  //set a shortcut,maybe:Ctrl+N(help text,dispalyed on Toolbar)
	action_new_->setStatusTip(tr("Create a new file")); //status tips(displayed on parent widget)
	//connect(action_new_, &QAction::triggered, imagewidget_, )

	action_open_ = new QAction(QIcon(":/MainWindow/Resources/images/open.jpg"), tr("&Open..."), this);
	action_open_->setShortcuts(QKeySequence::Open); //set a shortcut,Ctrl+o
	action_open_->setStatusTip(tr("Open an existing file"));

	action_save_ = new QAction(QIcon(":/MainWindow/Resources/images/save.jpg"), tr("&Save"), this);
	action_save_->setShortcuts(QKeySequence::Save);
	action_save_->setStatusTip(tr("Save the document to disk"));

	action_saveas_ = new QAction(tr("Save &As..."), this);
	action_saveas_->setShortcuts(QKeySequence::SaveAs);
	action_saveas_->setStatusTip(tr("Save the document under a new name"));

	action_invert_ = new QAction(tr("Inverse"), this);
	action_invert_->setStatusTip(tr("Invert all pixel value in the image"));

	action_mirror_ = new QAction(tr("Mirror"), this);
	action_mirror_->setStatusTip(tr("Mirror image vertically or horizontally"));
	// The slot requires more arguments than the signal provides.
	//connect(action_mirror_, &QAction::triggered, imagewidget_, &ImageWidget::Mirror);
	connect(action_mirror_, &QAction::triggered, [this]() {
		imagewidget_->Mirror(); // use default arguments
		});

	action_gray_ = new QAction(tr("Grayscale"), this);
	action_gray_->setStatusTip(tr("Gray-scale map"));

	action_restore_ = new QAction(tr("Restore"), this);
	action_restore_->setStatusTip(tr("Show origin image"));

	action_idw_ = new QAction(tr("IDW"), this);
	action_idw_->setStatusTip(tr("warp the image with inverse distance-weighted function"));
	connect(action_idw_, SIGNAL(triggered()), this, SLOT(CallIDW()));

	action_rbf_ = new QAction(tr("RBF"), this);
	action_rbf_->setStatusTip(tr("warp the image with radius basis function"));
	connect(action_rbf_, SIGNAL(triggered()), this, SLOT(CallRBF()));

	action_setting_ = new QAction(tr("SetWarp"), this);
	action_setting_->setStatusTip(tr("Setting Warping data"));
	
	action_clear_ = new QAction(tr("ClearWarp"), this);
	action_clear_->setStatusTip(tr("Clear all input points"));

	Warp_points = new QCheckBox;
	Warp_points->setCheckState(Qt::Unchecked);

	checkboxname = new QLabel;
	checkboxname->setText("Warp points:");

	imagesize = new QLabel;
	imagesize->setText("Image Size:100%");
}

void MainWindow::ConnectAcionsToWidget()
{
	connect(action_open_, &QAction::triggered, imagewidget_, &ImageWidget::Open);  //Let imagewdget_ open a file
	connect(action_save_, SIGNAL(triggered()), imagewidget_, SLOT(Save()));
	connect(action_saveas_, &QAction::triggered, imagewidget_, &ImageWidget::SaveAs);
	connect(action_invert_, &QAction::triggered, imagewidget_, &ImageWidget::Invert);
	connect(action_gray_, &QAction::triggered, imagewidget_, &ImageWidget::TurnGray);
	connect(action_restore_, &QAction::triggered, imagewidget_, &ImageWidget::Restore);
	connect(action_setting_, &QAction::triggered, imagewidget_, &ImageWidget::StartSetting);
	connect(action_clear_, &QAction::triggered, imagewidget_, &ImageWidget::ClearSetting);
	connect(Warp_points, &QCheckBox::stateChanged, imagewidget_, &ImageWidget::change_points_visible);
}

void MainWindow::CreateMenus()
{
	menu_file_ = menuBar()->addMenu(tr("&File"));
	menu_file_->setStatusTip(tr("File menu"));
	menu_file_->addAction(action_new_);
	menu_file_->addAction(action_open_);
	menu_file_->addAction(action_save_);
	menu_file_->addAction(action_saveas_);
	menu_file_->addAction(action_setting_);
	menu_file_->addAction(action_idw_);
	menu_file_->addAction(action_rbf_);
	menu_file_->addAction(action_clear_);


	menu_edit_ = menuBar()->addMenu(tr("&Edit"));
	menu_edit_->setStatusTip(tr("Edit menu"));
	menu_edit_->addAction(action_invert_);
	menu_edit_->addAction(action_mirror_);
	menu_edit_->addAction(action_gray_);
	menu_edit_->addAction(action_restore_);
	menu_edit_->addAction(action_setting_);
	menu_edit_->addAction(action_idw_);
	menu_edit_->addAction(action_rbf_);
	menu_edit_->addAction(action_clear_);
}

void MainWindow::CreateToolBars()
{
	toolbar_file_ = addToolBar(tr("File"));
	toolbar_file_->addAction(action_new_);
	toolbar_file_->addAction(action_open_);
	toolbar_file_->addAction(action_save_);

	// Add separator in toolbar 
	toolbar_file_->addSeparator();
	toolbar_file_->addAction(action_invert_);
	toolbar_file_->addAction(action_mirror_);
	toolbar_file_->addAction(action_gray_);
	toolbar_file_->addAction(action_restore_);
	toolbar_file_->addSeparator();
	toolbar_file_->addAction(action_setting_);
	toolbar_file_->addAction(action_idw_);
	toolbar_file_->addAction(action_rbf_);
	toolbar_file_->addAction(action_clear_);

	this->addToolBarBreak(Qt::TopToolBarArea);
	toolbar_setting_ = addToolBar("Edit");
	toolbar_setting_->addWidget(checkboxname);
	toolbar_setting_->addWidget(Warp_points);
	toolbar_setting_->addSeparator();
	toolbar_setting_->addWidget(imagesize);
	imgsize = new QSlider;
	imgsize->setOrientation(Qt::Horizontal);
	imgsize->setMinimum(1);
	imgsize->setFixedWidth(100);
	imgsize->setMaximum(300);
	imgsize->setSingleStep(1);
	imgsize->setValue(100);
	toolbar_setting_->addWidget(imgsize);
	connect(imgsize, SIGNAL(valueChanged(int)), this, SLOT(set_image_ratio(int)));
}

void MainWindow::CallIDW()
{
	imagewidget_->IDWWarping(-1);
}

void MainWindow::CallRBF()
{
	imagewidget_->RBFWarping(-1);
}

void MainWindow::CreateStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void MainWindow::set_image_ratio(int value)
{
	QString text = "Image Size:";
	if (value < 100)
	{
		text += " ";
		if (value < 10) text += " ";
	}
	text += QString::number(value); //change number to a QString
	text += "% ";
	imagesize->setText(text);
	imagewidget_->setratio(value);
}