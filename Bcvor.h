#ifndef DOM2_BCVOR_H
#define DOM2_BCVOR_H
#include <iostream>
using namespace std;

typedef struct zapis
{
    int id, idb, idc, tax;
    string name;
    double balance;
}Zapis;

class Bcvor{
public:
    bool list;
    //mozda mi ne treba
    int m;

    explicit Bcvor(int m = 5);
    //~Bcvor();

    const int *getVal() const;
    Bcvor *const *getPtr() const;
    zapis *const *getZap() const;




private:
    int n;
    int val[10];
    Bcvor *ptr[10];
    struct zapis *zap[10];
};


#endif
