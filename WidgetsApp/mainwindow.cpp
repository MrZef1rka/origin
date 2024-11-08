#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->rb_one->setText("Скорость 0,1мс");
    ui->rb_two->setText("Скорость 0,5мс");
    ui->rb_three->setText("Скорость 1с");
    ui->rb_four->setText("Скорость 2с");
    ui->rb_five->setText("Скорость 3с");
    ui->rb_six->setText("Скорость 5с");
    ui->pb_button->setText("Запуск");

    ui->pb_progress->setMaximum(100);
    ui->pb_progress->setMinimum(0);
    ui->pb_progress->setFormat("%p%");
    ui->pb_progress->setValue(0);

    ui->pb_button->setEnabled(false);
    ui->pb_button->setCheckable(true);

    ui->cb_list->addItem("Базовый");
    ui->cb_list->addItem("Синий");
    ui->cb_list->addItem("Красный");

    progressTimer = new QTimer(this);
    connect(progressTimer, &QTimer::timeout, this, &MainWindow::updateProgressBar);
    connect(ui->pb_button, &QPushButton::toggled, this, &MainWindow::onButtonToggled);
    connect(ui->cb_list, &QComboBox::currentTextChanged, this, &MainWindow::onColorSelected);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rb_one_clicked()
{
    progressStep = 10;
    progressTimer->setInterval(100);
    ui->pb_button->setEnabled(true);
}

void MainWindow::on_rb_two_clicked()
{
    progressStep = 10;
    progressTimer->setInterval(500);
    ui->pb_button->setEnabled(true);
}

void MainWindow::on_rb_three_clicked()
{
    progressStep = 10;
    progressTimer->setInterval(1000);
    ui->pb_button->setEnabled(true);
}


void MainWindow::on_rb_four_clicked()
{
    progressStep = 10;
    progressTimer->setInterval(2000);
    ui->pb_button->setEnabled(true);
}

void MainWindow::on_rb_five_clicked()
{
    progressStep = 10;
    progressTimer->setInterval(3000);
    ui->pb_button->setEnabled(true);
}

void MainWindow::on_rb_six_clicked()
{
    progressStep = 10;
    progressTimer->setInterval(5000);
    ui->pb_button->setEnabled(true);
}

void MainWindow::onButtonToggled(bool checked)
{
    if (checked) {
        // Когда кнопка включена, таймер запускается в зависимости от режима
        if (!progressTimer->isActive()) {
            progressTimer->start();
            ui->pb_button->setText("Стоп");
        }
    } else {
        // Когда кнопка выключена, останавливаем таймер
        progressTimer->stop();
        ui->pb_progress->setValue(0);
        ui->pb_button->setText("Запуск");
    }
}

void MainWindow::updateProgressBar()
{
    int currentValue = ui->pb_progress->value();
    int newValue = currentValue + progressStep;

    if (newValue > ui->pb_progress->maximum()) {
        ui->pb_progress->setValue(0); // Сбросим прогресс-бар, если он достиг максимума
    } else {
        ui->pb_progress->setValue(newValue); // Обновим прогресс
    }
}

void MainWindow::onColorSelected(const QString &color)
{
    QString colorCode;

    if (color == "Базовый")
    {

        ui->pb_progress->setStyleSheet("");
        return;
    }

    if (color == "Синий")
    {
        colorCode = "#5555ff"; //Синий
    } else if (color == "Красный")
    {
        colorCode = "#f44336"; // Красный
    }

    // Устанавливаем стиль с выбранным цветом
    ui->pb_progress->setStyleSheet(QString(
        "QProgressBar {"
        "background-color: darkgray;"
        "text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "background-color: %1;"
    "}"
 ).arg(colorCode));
}






