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
            if ( col == 7 && row != 0)
            {
                QVariant value = cell->dynamicCall("Value()").toInt();
                QTableWidgetItem *item = new QTableWidgetItem(value.toString());
                ui->tableWidget->setItem(row, col, item);
            }
            else
            {
                QVariant value = cell->dynamicCall("Value()");
                QTableWidgetItem *item = new QTableWidgetItem(value.toString());
                ui->tableWidget->setItem(row, col, item);
            }


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
       QLineEdit *line_edit = new QLineEdit();
       QTableWidgetItem *item = ui->tableWidget->item(0, col);
       check_box->setText(item->text());
       line_edit->setPlaceholderText("Вес");
       criteriabox1_layout->addWidget(check_box);
       valuebox1_layout->addWidget(line_edit);
       std::cout<<check_box->getID()<<std::endl;
   }

   //Чтобы id кнопок не перемешивались
   for ( int col = 0; col < int_cols; ++col )
   {
       DynamicCheckBox *check_box_min = new DynamicCheckBox();
       check_box_min->setText("min");
       minmaxbox1_layout->addWidget(check_box_min);
   }

   ui->criteria->setLayout(criteriabox1_layout);
   ui->value->setLayout(valuebox1_layout);
   ui->minmax->setLayout(minmaxbox1_layout);

}


void MainWindow::on_pushButton_clicked()
{
    delete ui->scrollArea->layout();
    int int_rows = ui->tableWidget->rowCount();

    std::vector<int> chosed_criterias;
    QObjectList check_list = ui->criteria->children();
    QObjectList::const_iterator it = check_list.begin();
    QObjectList::const_iterator end = check_list.end();
    while ( it != end )
    {
        DynamicCheckBox *check_box = (DynamicCheckBox *)(*it++);
        if ( check_box->isChecked() )
            chosed_criterias.push_back(check_box->getID());
    }

    int max = 0;
    for ( int criterias: chosed_criterias)
    {
        max = 0;
        for ( int row = 1; row < int_rows; ++row )
        {
            QTableWidgetItem *item = ui->tableWidget->item(row, criterias-1);

            if (item && item->text().toInt() > max)
                max = item->text().toInt();
        }
    }

    std::cout<<max<<std::endl;

}


void MainWindow::on_wallButton_clicked()
{
    qDebug() << "Clicked!";
    if ( ui->wall_answer->layout() != NULL )
    {
        QLayoutItem* item;
        while ( ( item = ui->wall_answer->layout()->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete ui->wall_answer->layout();
    }

    std::vector<int> wall_list;
    std::vector<int> wall_list_values;
    const QList<QLineEdit*> wall_check_list = ui->value_wall->findChildren<QLineEdit*>();
    std::vector<int> answer;
    bool flag = true;

    for ( QLineEdit *it: wall_check_list )
    {
        if ( it->text().size() > 0 )
        {
            wall_list.push_back(it->objectName().back().digitValue());
            wall_list_values.push_back(it->text().toInt());
        }

    }

    for ( int rows = 1; rows < ui->tableWidget->rowCount() - 1; ++rows)
    {
        flag = true;
        for ( size_t it = 0; it < wall_list.size(); ++it )
        {
            qDebug() << ui->tableWidget->item(rows, wall_list.at(it))->text();

            if ( ui->tableWidget->item(rows, wall_list.at(it))->text().toInt() < wall_list_values[it])
            {
                flag = false;
                break;
            }
        }

        if ( flag )
            answer.push_back(rows);

    }

    QVBoxLayout *p_wall_answer_layout = new QVBoxLayout;
    if ( answer.size() == 0 )
    {
        p_wall_answer_layout->addWidget(new QLabel("Нет альтернатив"));
        ui->wall_answer->setLayout(p_wall_answer_layout);
    }
    for ( int it: answer )
    {
        p_wall_answer_layout->addWidget(new QLabel(ui->tableWidget->item(it, 1)->text()));
    }
    ui->wall_answer->setLayout(p_wall_answer_layout);


}


void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    if ( index == 3 )
    {
        QVBoxLayout *criteriabox1_layout = new QVBoxLayout;
        QVBoxLayout *valuebox1_layout = new QVBoxLayout;

        delete ui->criteria_wall->layout();
        delete ui->value_wall->layout();

        for ( int col = 0; col < ui->tableWidget->columnCount() - 1; ++col )
        {
            QLabel *name = new QLabel();
            QLineEdit *line_edit = new QLineEdit();
            QTableWidgetItem *item = ui->tableWidget->item(0, col);
            name->setText(item->text());
            line_edit->setObjectName("wall_criteria_" + std::to_string(col));
            line_edit->setPlaceholderText("Граница");
            criteriabox1_layout->addWidget(name);
            valuebox1_layout->addWidget(line_edit);
        }

        ui->criteria_wall->setLayout(criteriabox1_layout);
        ui->value_wall->setLayout(valuebox1_layout);
    }
}

