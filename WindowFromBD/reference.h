#ifndef REFERENCE_H
#define REFERENCE_H

#include <QMainWindow>
#include <QDialog>

namespace Ui {
class Reference;
}

class Reference : public QDialog
{
    Q_OBJECT

public:
    explicit Reference(QWidget *parent = nullptr);
    ~Reference();

private:
    Ui::Reference *ui;
};

#endif // REFERENCE_H
