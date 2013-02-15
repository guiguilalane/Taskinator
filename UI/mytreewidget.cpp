#include "mytreewidget.h"

MyTreeWidget::MyTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
}

QModelIndex MyTreeWidget::getIndexFromItem(QTreeWidgetItem *item, const int column)
{
    return indexFromItem(item, column);
}

void MyTreeWidget::onActivatedItem(QTreeWidgetItem *item, int column)
{
    emit itemActivated(item, column);
}

void MyTreeWidget::onItemSelectionChanged()
{
    emit itemSelectionChanged();
}
