#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <atomic>
#include <memory>

#include <mutex>

class Store;
class Movie
{
    std::atomic_bool fExit;
    const int widthHeight = 224;
    const std::string modelName = "/home/andy/Projects/C++/Files/ScriptModel/resnet50.pt";
    std::string fileName;
    std::shared_ptr<Store> store;
public:
    Movie(const std::string &val);
    bool calc();
    void onExit();
};

#endif // MOVIE_H
