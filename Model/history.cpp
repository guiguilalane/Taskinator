#include "history.h"

using namespace std;

History::History()
{
   // historyTab_(8);
    it = historyTab_.begin();
}

History::History(List * l)
{
    //historyTab_(8);
    //historyTab_.push_back();
    it = historyTab_.begin();
}

History::~History()
{}

History::autoSave(List * l)
{

}
