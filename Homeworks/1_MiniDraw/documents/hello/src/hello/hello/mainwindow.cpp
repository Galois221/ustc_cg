#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QToolBar>
#include <QMessageBox> //信号与槽

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)   //new class MainWindow Ui :: ui = MainWindow
{
    ui->setupUi(this);
    CreateButtons();
}

void MainWindow::CreateButtons(){
    hello_world_action_ = new QAction(tr("&Hello world"),this);

    connect(hello_world_action_,&QAction::triggered,this,&MainWindow::Helloworld);

    main_menu_ = menuBar()->addMenu(tr("&Main"));

    main_menu_->addAction(hello_world_action_);

    main_toolbar_=addToolBar(tr("&Main"));

    main_toolbar_->addAction(hello_world_action_);
}

void MainWindow::Helloworld(){
    QMessageBox::about(this,tr("Hello world"),tr("Well done!Go ahead."));
}



MainWindow::~MainWindow()
{
    delete ui;
}

