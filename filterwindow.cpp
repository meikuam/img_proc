#include "filterwindow.h"
#include "ui_filterwindow.h"

#include <iostream>

//------------------<stackoverflow code>------------------
//https://stackoverflow.com/questions/5395266/removing-widgets-from-qgridlayout

/**
 * Helper function. Deletes all child widgets of the given layout @a item.
 */
void deleteChildWidgets(QLayoutItem *item) {
    if (item->layout()) {
        // Process all child items recursively.
        for (int i = 0; i < item->layout()->count(); i++) {
            deleteChildWidgets(item->layout()->itemAt(i));
        }
    }
    delete item->widget();
}

/**
 * Helper function. Removes all layout items within the given @a layout
 * which either span the given @a row or @a column. If @a deleteWidgets
 * is true, all concerned child widgets become not only removed from the
 * layout, but also deleted.
 */
void remove(QGridLayout *layout, int row, int column, bool deleteWidgets) {
    // We avoid usage of QGridLayout::itemAtPosition() here to improve performance.
    for (int i = layout->count() - 1; i >= 0; i--) {
        int r, c, rs, cs;
        layout->getItemPosition(i, &r, &c, &rs, &cs);
        if ((r <= row && r + rs - 1 >= row) || (c <= column && c + cs - 1 >= column)) {
            // This layout item is subject to deletion.
            QLayoutItem *item = layout->takeAt(i);
            if (deleteWidgets) {
                deleteChildWidgets(item);
            }
            delete item;
        }
    }
}

/**
 * Removes all layout items on the given @a row from the given grid
 * @a layout. If @a deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the row itself from the grid
 * layout, as this isn't possible (i.e. the rowCount() and row indices
 * will stay the same after this function has been called).
 */
void removeRow(QGridLayout *layout, int row, bool deleteWidgets) {
    remove(layout, row, -1, deleteWidgets);
    layout->setRowMinimumHeight(row, 0);
    layout->setRowStretch(row, 0);
}

/**
 * Removes all layout items on the given @a column from the given grid
 * @a layout. If @a deleteWidgets is true, all concerned child widgets
 * become not only removed from the layout, but also deleted. Note that
 * this function doesn't actually remove the column itself from the grid
 * layout, as this isn't possible (i.e. the columnCount() and column
 * indices will stay the same after this function has been called).
 */
void removeColumn(QGridLayout *layout, int column, bool deleteWidgets) {
    remove(layout, -1, column, deleteWidgets);
    layout->setColumnMinimumWidth(column, 0);
    layout->setColumnStretch(column, 0);
}
//-----------------</stackoverflow code>------------------


FilterWindow::FilterWindow(ImgData *inp, ImgData *out, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilterWindow)
{
    ui->setupUi(this);

    input = inp;
    output = out;

    ui->filterBox->addItem("Собеля");                       // 0
    ui->filterBox->addItem("Математической морфологии");    // 1
    ui->filterBox->addItem("Превитта");                     // 2
    ui->filterBox->addItem("Канни");                        // 3
    ui->filterBox->addItem("Лапласа");                      // 4
    ui->filterBox->addItem("Робертса");                     // 5
    ui->filterBox->addItem("Бинаризации");                  // 6
    ui->filterBox->addItem("Фильтр Гаусса");                // 7

//    on_filterBox_currentIndexChanged(0);
}

FilterWindow::~FilterWindow()
{
    delete ui;
}

void FilterWindow::on_applyButton_clicked()
{
    Filter::filter(input, output, method);
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
    {
        method = Sobel;
        tables.clear();
        labels.clear();
        removeRow(ui->gridLayout, 0, true);
        removeRow(ui->gridLayout, 1, true);
        removeRow(ui->gridLayout, 2, true);
        removeRow(ui->gridLayout, 3, true);

        Mask m(method);
        for(int i = 0; i < m.dims(); i++) {
            tables.push_back(new QTableWidget(this));
            tables[i]->horizontalHeader()->setDefaultSectionSize(30);
            tables[i]->setRowCount(m.height());
            tables[i]->setColumnCount(m.width());
            for(int x = 0; x < m.width(); x++) {
                for(int y = 0; y < m.height(); y++) {
                    tables[i]->setItem(y, x, new QTableWidgetItem(QString::number(m(x,y,i))));
                }
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
    case 1:
    {
        method = Morphology;
        tables.clear();
        labels.clear();

        removeRow(ui->gridLayout, 0, true);
        removeRow(ui->gridLayout, 1, true);
        removeRow(ui->gridLayout, 2, true);
        removeRow(ui->gridLayout, 3, true);

        Mask m(method);
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
        ui->gridLayout->addWidget(labels[0], 0, 0);
        ui->gridLayout->addWidget(tables[0], 1, 0);
        break;
    }
    case 2:
    {
        method = Prewitt;

        tables.clear();
        labels.clear();

        removeRow(ui->gridLayout, 0, true);
        removeRow(ui->gridLayout, 1, true);

        Mask m(method);
        for(int i = 0; i < m.dims(); i++) {
            tables.push_back(new QTableWidget(this));
            tables[i]->horizontalHeader()->setDefaultSectionSize(30);
            tables[i]->setRowCount(m.height());
            tables[i]->setColumnCount(m.width());
            for(int x = 0; x < m.width(); x++) {
                for(int y = 0; y < m.height(); y++) {
                    tables[i]->setItem(y, x, new QTableWidgetItem(QString::number(m(x,y,i))));
                }
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
    case 3:
    {
        method = Canny;
        tables.clear();
        labels.clear();

        removeRow(ui->gridLayout, 0, true);
        removeRow(ui->gridLayout, 1, true);
        removeRow(ui->gridLayout, 2, true);
        removeRow(ui->gridLayout, 3, true);
        labels.push_back(new QLabel(this));
        labels.back()->setText("Еще не реализовано");
        ui->gridLayout->addWidget(labels[0], 0, 0);
        break;
    }
    case 4:
    {
        method = Laplas;
        tables.clear();
        labels.clear();

        removeRow(ui->gridLayout, 0, true);
        removeRow(ui->gridLayout, 1, true);
        removeRow(ui->gridLayout, 2, true);
        removeRow(ui->gridLayout, 3, true);
        labels.push_back(new QLabel(this));
        labels.back()->setText("Еще не реализовано");
        ui->gridLayout->addWidget(labels[0], 0, 0);
        break;
    }
    case 5:
    {
        method = Roberts;
        tables.clear();
        labels.clear();

        removeRow(ui->gridLayout, 0, true);
        removeRow(ui->gridLayout, 1, true);
        removeRow(ui->gridLayout, 2, true);
        removeRow(ui->gridLayout, 3, true);
        labels.push_back(new QLabel(this));
        labels.back()->setText("Еще не реализовано");
        ui->gridLayout->addWidget(labels[0], 0, 0);
        break;
    }
    case 6:
    {
        method = Binary;
        tables.clear();
        labels.clear();

        removeRow(ui->gridLayout, 0, true);
        removeRow(ui->gridLayout, 1, true);
        removeRow(ui->gridLayout, 2, true);
        removeRow(ui->gridLayout, 3, true);
        labels.push_back(new QLabel(this));
        labels.back()->setText("Глобальный порог");
        ui->gridLayout->addWidget(labels[0], 0, 0);
        break;
    }
    case 7:
    {
        method = Gauss;
        tables.clear();
        labels.clear();

        removeRow(ui->gridLayout, 0, true);
        removeRow(ui->gridLayout, 1, true);
        removeRow(ui->gridLayout, 2, true);
        removeRow(ui->gridLayout, 3, true);

        Mask m(method);
        for(int i = 0; i < m.dims(); i++) {
            tables.push_back(new QTableWidget(this));
            tables[i]->horizontalHeader()->setDefaultSectionSize(80);
            tables[i]->setRowCount(m.height());
            tables[i]->setColumnCount(m.width());
            for(int x = 0; x < m.width(); x++) {
                for(int y = 0; y < m.height(); y++) {
                    tables[i]->setItem(y, x, new QTableWidgetItem(QString::number(m(x,y,i))));
                }
            }
        }
        labels.push_back(new QLabel(this));
        labels.back()->setText("Маска");
        ui->gridLayout->addWidget(labels[0], 0, 0);
        ui->gridLayout->addWidget(tables[0], 1, 0);
        break;
    }
    }
}
