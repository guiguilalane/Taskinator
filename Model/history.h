#ifndef HISTORY_H
#define HISTORY_H

#include <vector>
#include <list.h>

class List;

class History
{
public:
    History();
    History(List * l);
    ~History();

    // This fonction make a copy of l and insert it in a vector
    void autoSave(List * l);

private:
    std::vector<List *> historyTab_;
    std::vector<List *>::iterator it;
};

#endif // HISTORY_H
