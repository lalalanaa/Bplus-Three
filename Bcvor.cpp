#include <iostream>
#include "Bcvor.h"
using namespace std;

Bcvor::Bcvor(int m)
{
    list = true;
    n = 0;
    m = (m > 10 ? 10 : m);
    for(int i = 0; i < m; i++)
    {
        val[i] = 0;
        ptr[i] = nullptr;
        zap[i] = nullptr;
    }
}

const int *Bcvor::getVal() const {
    return val;
}

Bcvor *const *Bcvor::getPtr() const {
    return ptr;
}

zapis *const *Bcvor::getZap() const {
    return zap;
}
