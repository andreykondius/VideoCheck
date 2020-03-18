#include "store.h"
uint uidGlob;
std::mutex mutStore;
std::vector< BBox > Store::bboxes;

Store::Store()
{    
}

void Store::clear()
{
    Store::bboxes.clear();
}

void Store::push(const int x1, const int y1, const int x2, const int y2)
{
    std::unique_lock<std::mutex> m(mutStore);

    static uint id = ++uidGlob;
    uid = id;

    BBox bbox(x1, y1, x2, y2);
    Store::bboxes.emplace_back(bbox);
}

int Store::countBboxes() const
{
    std::unique_lock<std::mutex> m(mutStore);
    return Store::bboxes.size();
}
