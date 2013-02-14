#ifndef MYTREEWIDGET_H
#define MYTREEWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

class MyTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    MyTreeWidget(QWidget *parent = 0);

    QModelIndex getIndexFromItem(QTreeWidgetItem* item, const int column = 0);
    
    void onActivatedItem(QTreeWidgetItem* item, int column = 0);

signals:
    
public slots:
    
};

#endif // MYTREEWIDGET_H
