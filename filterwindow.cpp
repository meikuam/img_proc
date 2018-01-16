#include "filterwindow.h"
#include "ui_filterwindow.h"

#include <iostream>
#include <QDebug>
#include <QVector>

////---------
///**
// * Removes all layout items on the given @a row from the given grid
// * @a layout. If @a deleteWidgets is true, all concerned child widgets
// * become not only removed from the layout, but also deleted. Note that
// * this function doesn't actually remove the row itself from the grid
// * layout, as this isn't possible (i.e. the rowCount() and row indices
// * will stay the same after this function has been called).
// */
//void removeRow(QGridLayout *layout, int row, bool deleteWidgets) {
//    remove(layout, row, -1, deleteWidgets);
//    layout->setRowMinimumHeight(row, 0);
//    layout->setRowStretch(row, 0);
//}

///**
// * Removes all layout items on the given @a column from the given grid
// * @a layout. If @a deleteWidgets is true, all concerned child widgets
// * become not only removed from the layout, but also deleted. Note that
// * this function doesn't actually remove the column itself from the grid
// * layout, as this isn't possible (i.e. the columnCount() and column
// * indices will stay the same after this function has been called).
// */
//void removeColumn(QGridLayout *layout, int column, bool deleteWidgets) {
//    remove(layout, -1, column, deleteWidgets);
//    layout->setColumnMinimumWidth(column, 0);
//    layout->setColumnStretch(column, 0);
//}

///**
// * Helper function. Removes all layout items within the given @a layout
// * which either span the given @a row or @a column. If @a deleteWidgets
// * is true, all concerned child widgets become not only removed from the
// * layout, but also deleted.
// */
//void remove(QGridLayout *layout, int row, int column, bool deleteWidgets) {
//    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
//    for (int i = layout->count() - 1; i >= 0; i--) {
//        int r, c, rs, cs;
//        layout->getItemPosition(i, &r, &c, &rs, &cs);
//        if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column)) {
//            // This layout item is subject to deletion.
//            QLayoutItem *item = layout->takeAt(i);
//            if (deleteWidgets) {
//                deleteChildWidgets(item);
//            }
//            delete item;
//        }
//    }
//}

///**
// * Helper function. Deletes all child widgets of the given layout @a item.
// */
//void deleteChildWidgets(QLayoutItem *item) {
//    if (item->layout()) {
//        // Process all child items recursively.
//        for (int i = 0; i < item->layout()->count(); i++) {
//            deleteChildWidgets(item->layout()->itemAt(i));
//        }
//    }
//    delete item->widget();
//}
//-----------

void FilterWindow::addTable(Method f) {
    switch (f) {
    case Sobel:
    {

        tables.clear();
        labels.clear();
//        removeRow(ui->gridLayout, 0, true);
//        removeRow(ui->gridLayout, 1, true);
        Mask m(Sobel);
        tables.push_back(new QTableWidget(this));
        tables.push_back(new QTableWidget(this));

        tables[0]->horizontalHeader()->setDefaultSectionSize(30);
        tables[1]->horizontalHeader()->setDefaultSectionSize(30);
        tables[0]->setRowCount(m.height());
        tables[0]->setColumnCount(m.width());
        tables[1]->setRowCount(m.height());
        tables[1]->setColumnCount(m.width());
        for(int x = 0; x < m.width(); x++) {
            for(int y = 0; y < m.height(); y++) {
                tables[0]->setItem(y, x, new QTableWidgetItem(QString::number(m(x,y,0))));
                tables[1]->setItem(y, x, new QTableWidgetItem(QString::number(m(x,y,1))));
            }
        }

        labels.push_back(new QLabel(this));
        labels.push_back(new QLabel(this));
        labels.back()->setText("Gx");
        labels.back()->setText("Gy");
        ui->gridLayout->addWidget(labels[0], 0, 0);
        ui->gridLayout->addWidget(labels[1], 0, 1);


        ui->gridLayout->addWidget(tables[0], 1, 0);
        ui->gridLayout->addWidget(tables[1], 1, 1);
        break;
    }
    case Morphology:
    {
        tables.clear();
        labels.clear();

//        removeRow(ui->gridLayout, 0, true);
//        removeRow(ui->gridLayout, 1, true);
        Mask m(Morphology);
        tables.push_back(new QTableWidget(this));

        tables[0]->horizontalHeader()->setDefaultSectionSize(30);
        tables[0]->setRowCount(m.height());
        tables[0]->setColumnCount(m.width());
        for(int x = 0; x < m.width(); x++) {
            for(int y = 0; y < m.height(); y++) {
                tables[0]->setItem(y, x, new QTableWidgetItem(QString::number(m(x,y,0))));
            }
        }

        labels.push_back(new QLabel(this));
        labels.back()->setText("Примитив эрозии");
        labels.push_back(new QLabel(this));
        labels.back()->setText("");
        ui->gridLayout->addWidget(labels[0], 0, 0);
        ui->gridLayout->addWidget(tables[0], 1, 0);

        ui->gridLayout->addWidget(labels[1], 1, 0);
        ui->gridLayout->addWidget(labels[1], 1, 1);
        break;
    }
    case Prewitt:
    {
        tables.clear();
        labels.clear();

//        removeRow(ui->gridLayout, 0, true);
//        removeRow(ui->gridLayout, 1, true);
        Mask m(Prewitt);
        tables.push_back(new QTableWidget(this));
        tables.push_back(new QTableWidget(this));

        tables[0]->horizontalHeader()->setDefaultSectionSize(30);
        tables[1]->horizontalHeader()->setDefaultSectionSize(30);
        tables[0]->setRowCount(m.height());
        tables[0]->setColumnCount(m.width());
        tables[1]->setRowCount(m.height());
        tables[1]->setColumnCount(m.width());
        for(int x = 0; x < m.width(); x++) {
            for(int y = 0; y < m.height(); y++) {
                tables[0]->setItem(y, x, new QTableWidgetItem(QString::number(m(x,y,0))));
                tables[1]->setItem(y, x, new QTableWidgetItem(QString::number(m(x,y,1))));
            }
        }

        labels.push_back(new QLabel(this));
        labels.push_back(new QLabel(this));
        labels.back()->setText("Gx");
        labels.back()->setText("Gy");
        ui->gridLayout->addWidget(labels[0], 0, 0);
        ui->gridLayout->addWidget(labels[1], 0, 1);


        ui->gridLayout->addWidget(tables[0], 1, 0);
        ui->gridLayout->addWidget(tables[1], 1, 1);
        break;
    }
    case Canny:
    {
        break;
    }
    case Laplas:
    {

        break;
    }
    case Roberts:
    {
        break;
    }
    }
}

FilterWindow::FilterWindow(ImgData *inp, ImgData *out, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilterWindow)
{
    ui->setupUi(this);
    addTable(Sobel);
    on_filterBox_currentIndexChanged(0);
    input = inp;
    output = out;
}

FilterWindow::~FilterWindow()
{
    delete ui;
}

void FilterWindow::on_applyButton_clicked()
{
    ImgData* buf = new ImgData(*input);

    Filter::filter(input, buf, Binary);
    Filter::filter(buf, output, method);
//    Filter::filter(input, output, method);
    delete buf;
    setImage(output->img());
    repaint();
    this->close();
}

void FilterWindow::on_cancelButton_clicked()
{
    this->close();
}

void FilterWindow::on_filterBox_currentIndexChanged(int index)
{
    switch(index) {
    case 0:
        method = Sobel;
        break;
    case 1:
        method = Morphology;
        break;
    case 2:
        method = Prewitt;
        break;
    case 3:
        method = Canny;
        break;
    case 4:
        method = Laplas;
        break;
    case 5:
        method = Roberts;
        break;
    }
    addTable(method);
}
