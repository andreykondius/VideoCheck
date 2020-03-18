#ifndef STORE_H
#define STORE_H

#include <vector>
#include <mutex>

extern uint uidGlob;
extern std::mutex mutStore;

struct BBox
{
    int x1,y1,x2,y2;
    BBox(const int x1, const int y1, const int x2, const int y2) : x1(x1), y1(y1), x2(x2), y2(y2){}
};

class Store
{
    uint uid;
    static std::vector< BBox > bboxes;
public:
    Store();
    void clear();
    void push(const int x1, const int y1, const int x2, const int y2);
    int countBboxes() const;
};

#endif // STORE_H
