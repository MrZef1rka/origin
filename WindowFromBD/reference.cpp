#include "reference.h"
#include "ui_reference.h"

Reference::Reference(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Reference)
{
    ui->setupUi(this);
}

Reference::~Reference()
{
    delete ui;
}

