#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QAxObject>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{
    QString file_name = QFileDialog::getOpenFileName(this, "Выберите .xls файл", "C://");

    QAxObject *excel = new QAxObject("Excel.Application", this);
    QAxObject *workbooks = excel->querySubObject("Workbooks");
    QAxObject *workbook = workbooks->querySubObject("Open(const QString&)", file_name);
    excel->dynamicCall("SetVisible(bool)", false);

    QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)", 1);

    QAxObject *used_range = worksheet->querySubObject("UsedRange");
    QAxObject *rows = used_range->querySubObject("Rows");
    QAxObject *columns = used_range->querySubObject("Columns");

    int int_row_start = used_range->property("Row").toInt();
    int int_col_start = used_range->property("Column").toInt();
    int int_cols = columns->property("Count").toInt();
    int int_rows = rows->property("Count").toInt();

    ui->tableWidget->setColumnCount(int_col_start + int_cols);
    ui->tableWidget->setRowCount(int_row_start + int_rows);

    for ( int row = 0; row < int_rows; ++row )
    {
        for ( int col = 0; col < int_cols; ++col )
        {
            QAxObject *cell = worksheet->querySubObject("Cells(int,int)", row + 1, col + 1);
            QVariant value = cell->dynamicCall("Value()");
            QTableWidgetItem *item = new QTableWidgetItem(value.toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    workbook->dynamicCall("Close");
    excel->dynamicCall("Quit()");
}

