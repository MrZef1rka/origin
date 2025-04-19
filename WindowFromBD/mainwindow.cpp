#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dialRef = new Reference(this);
    dialRef->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pb_open_clicked()
{
    dialRef->show();
}

