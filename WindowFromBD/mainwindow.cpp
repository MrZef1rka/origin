#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pb_cancel, &QPushButton::clicked, this, &MainWindow::port_cancel);
    connect(ui->refwindow, &QAction::triggered, this, &MainWindow::show_reference);

    dialRef = new Reference(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::show_reference()
{

    dialRef->show();

};

void MainWindow::port_cancel()
{
    ui->le_port->clear();
    ui->le_port->setValue(0);
}




