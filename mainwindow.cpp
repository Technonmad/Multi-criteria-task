#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QAxObject>
#include <QCheckBox>
#include <QLineEdit>
#include <QRadioButton>
#include <stdio.h>
#include <iostream>
#include <dynamiccheckbox.h>


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

   QVBoxLayout *criteriabox1_layout = new QVBoxLayout;
   QVBoxLayout *valuebox1_layout = new QVBoxLayout;
   QVBoxLayout *minmaxbox1_layout = new QVBoxLayout;

   DynamicCheckBox *check_box = new DynamicCheckBox();
   check_box->cleanID();
   delete check_box;

   delete ui->criteria->layout();
   delete ui->value->layout();
   delete ui->minmax->layout();



   for ( int col = 0; col < int_cols; ++col )
   {
       DynamicCheckBox *check_box = new DynamicCheckBox();
       QTableWidgetItem *item = ui->tableWidget->item(0, col);
       check_box->setText(item->text());
       criteriabox1_layout->addWidget(check_box);
       valuebox1_layout->addWidget(new QLineEdit);
       minmaxbox1_layout->addWidget(new QCheckBox("min"));
       std::cout<<check_box->getID()<<std::endl;
   }

   ui->criteria->setLayout(criteriabox1_layout);
   ui->value->setLayout(valuebox1_layout);
   ui->minmax->setLayout(minmaxbox1_layout);

}

