#include "mytreewidget.h"

MyTreeWidget::MyTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
}

QModelIndex MyTreeWidget::getIndexFromItem(QTreeWidgetItem *item, const int column)
{
    return indexFromItem(item, column);
}
