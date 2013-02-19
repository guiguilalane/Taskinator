#include "mytreewidget.h"

MyTreeWidget::MyTreeWidget(QWidget *parent) :
    QTreeWidget(parent)
{
}

MyTreeWidget::MyTreeWidget(MyTreeWidget *t)
{
    parcoursArbre(t, t);
}

void MyTreeWidget::parcoursArbre(MyTreeWidget *p, MyTreeWidget * t)
{
    for (int i = 0; i < p->topLevelItemCount(); i++)
    {
        parcoursArbreRecursif(p->topLevelItem(i), 0, t);
    }
}

void MyTreeWidget::parcoursArbreRecursif(QTreeWidgetItem *itemP, QTreeWidgetItem * itemT, MyTreeWidget * t)
{
    QTreeWidgetItem * elementItem = 0;
    if (itemT == 0){
        elementItem = new QTreeWidgetItem(this);
    }
    else {
        elementItem = new QTreeWidgetItem(itemT);
    }
    elementItem->setExpanded(itemP->isExpanded());
    QWidget * wid = new QWidget(t->itemWidget(itemP,0));
    this->setItemWidget(elementItem,0,wid);
    for (int i = 0; i < itemP->childCount(); i++)
    {
        parcoursArbreRecursif(itemP->child(i), elementItem, t);
    }
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
