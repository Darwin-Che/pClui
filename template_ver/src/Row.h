#ifndef ROW_H
#define ROW_H

#include <string>
#include <vector>

class Row {

public:

    virtual int fnum() = 0;

    virtual void get(int, std::string &) = 0;

    virtual void set(int, const std::string &) = 0;

    virtual std::vector<int> get_width() = 0;

};

#endif