#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_rb_one_clicked();

    void on_rb_two_clicked();

    void onButtonToggled(bool checked);
    void updateProgressBar();

    void onColorSelected(const QString &color);

    void on_rb_three_clicked();

    void on_rb_four_clicked();

    void on_rb_five_clicked();

    void on_rb_six_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *progressTimer;
    int progressStep;
};
#endif // MAINWINDOW_H
