#include "Clui.h"

namespace Clui {

template <class R>
BVector<R>::BVector(int n) {
    data = std::vector<R> (n);
}

template <class R>
int BVector<R>::fnum() {
    return R::fnum();
}

template <class R>
int BVector<R>::size() {
    return data.size();
}

template <class R>
void BVector<R>::insert_row(const std::string & row) {
    data.emplace_back(R(row));
}

template <class R>
void BVector<R>::remove_row(int i) {
    if (i >= data.size()) {
        eprintf("Out of Bound remove_row\n");
        throw std::runtime_error("ERROR!!!\n");
    }
    data.erase(data.begin() + i);
}

template <class R>
std::string BVector<R>::get_field(int i, int j) {
    if (i < 0 || i >= data.size()) {
        eprintf("Out of Bound get_field\n");
        throw std::runtime_error("ERROR!!!\n");
    }
    return data[i].get_field(j);
}

template <class R>
void BVector<R>::set_field(int i, int j, const std::string & s) {
    data[i].set_field(j, s);
}

}