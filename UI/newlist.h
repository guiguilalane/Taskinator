#ifndef NEWLIST_H
#define NEWLIST_H

#include <QDialog>

namespace Ui {
class NewList;
}

class NewList : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewList(QWidget *parent = 0);
    ~NewList();
    
private:
    Ui::NewList *ui;
};

#endif // NEWLIST_H
