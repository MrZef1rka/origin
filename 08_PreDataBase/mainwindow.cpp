#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , queryModel(nullptr)
    , tableModel(nullptr)
{
    // Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    // Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    // Добавим БД используя стандартный драйвер PSQL и зададим имя
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    // Устанавливаем данные для подключения к БД
    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    // Соединяем сигналы от БД
    connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(this, &MainWindow::sig_RequestToDb, dataBase, &DataBase::RequestToDB);

    // Настраиваем tableView
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow()
{
    delete queryModel;
    delete tableModel;
    delete ui;
}

void MainWindow::on_act_addData_triggered()
{
    dataDb->show();
}

void MainWindow::on_act_connect_triggered()
{
    if(ui->lb_statusConnect->text() == "Отключено"){
        ui->lb_statusConnect->setText("Подключение");
        ui->lb_statusConnect->setStyleSheet("color : black");
        dataBase->ConnectToDataBase(dataForConnect);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);

        // Очищаем модель при отключении
        ui->tableView->setModel(nullptr);
    }
}

void MainWindow::on_pb_request_clicked()
{
    QString request;

    if (ui->cb_category->currentText() == "Все") {
        // Для всех фильмов используем табличную модель
        request = "film"; // Имя таблицы для QSqlTableModel
    } else if (ui->cb_category->currentText() == "Комедия") {
        request = "SELECT title, description FROM film f "
                  "JOIN film_category fc on f.film_id = fc.film_id "
                  "JOIN category c on c.category_id = fc.category_id "
                  "WHERE c.name = 'Comedy'";
    } else if (ui->cb_category->currentText() == "Ужасы") {
        request = "SELECT title, description FROM film f "
                  "JOIN film_category fc on f.film_id = fc.film_id "
                  "JOIN category c on c.category_id = fc.category_id "
                  "WHERE c.name = 'Horror'";
    }

    emit sig_RequestToDb(request);
}

void MainWindow::on_pb_clear_clicked()
{
    // Очищаем таблицу, устанавливая пустую модель
    ui->tableView->setModel(nullptr);

    // Удаляем старые модели
    delete queryModel;
    delete tableModel;
    queryModel = nullptr;
    tableModel = nullptr;
}

void MainWindow::ScreenDataFromDB(QAbstractItemModel *model, int typeRequest)
{
    if (!model) {
        qDebug() << "Received null model";
        return;
    }

    // Устанавливаем модель в tableView
    ui->tableView->setModel(model);

    // Настраиваем отображение
    ui->tableView->resizeColumnsToContents();
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError().text());
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec();
    }
}
