#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <iostream>

class MyTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MyTreeWidget(QWidget *parent = 0);
    MyTreeWidget(MyTreeWidget * t);

    QModelIndex getIndexFromItem(QTreeWidgetItem* item, const int column = 0);
    
    void onActivatedItem(QTreeWidgetItem* item, int column = 0);

    void onItemSelectionChanged();
    void parcoursArbre(MyTreeWidget *p, MyTreeWidget * t);
    void parcoursArbreRecursif(QTreeWidgetItem *itemP, QTreeWidgetItem *elementItem, MyTreeWidget * t);

signals:
    
public slots:
    
};

#endif // MYTREEWIDGET_H
