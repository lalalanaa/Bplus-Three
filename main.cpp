#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
using namespace std;

typedef struct zapis
{
    long long int id, idb, idc, tax;
    string name;
    double balance;
}Zapis;

typedef struct cvor
{
    bool list;
    int n, level;
    long long int val[10];
    struct cvor *ptr[11], *parent;
    struct zapis *zapis[10];
}Cvor;

Cvor* napraviNoviCvor(Cvor *parent, int m = 10, bool list = true)
{
    Cvor *novi = new Cvor;
    novi -> n = 0;
    novi -> list = list;
    novi -> parent = parent;
    for(int i = 0; i < m; i++)
    {
        novi -> val[i] = 0;
        novi -> ptr[i] = nullptr;
        novi -> ptr[i+1] = nullptr;
        novi -> zapis[i] = nullptr;
    }
    return novi;
}

Cvor* nadjiList(Cvor *root, long long int id, int &br)
{
    br++;
    Cvor *tr = root;
    while(!tr -> list)
    {
        br++;
        for(int i = 0; i < tr->n; i++)
        {
            if(i == tr -> n - 1 and id > tr->val[i])
            {
                tr = tr->ptr[i + 1];
                break;
            }
            if(id <= tr->val[i])
            {
                tr = tr->ptr[i];
                break;
            }
        }}
    return tr;
}

Zapis* nadjiZapis(Cvor *root, long long int id, int &br)
{
    Cvor *list = nadjiList(root, id, br);
    for(int i = 0; i < list->n and id >= list->val[i]; i++)
        if(id == list->val[i])
            return list->zapis[i];
    return nullptr;
}

//1 je udesno
void ubaciKljucUCvor(Cvor *cvor, long long int id, Zapis *z, Cvor *c = nullptr, int smer = 0)
{
    int i = 0;
    if(smer)
        cvor->ptr[cvor->n + 1] = cvor->ptr[cvor->n];
    while(i < cvor->n and id > cvor -> val[i]) i++;
    if(i != cvor->n and cvor->val[i] == id) return;
    for(int j = cvor -> n; j > i; j--)
    {
        cvor -> val[j] = cvor -> val[j-1];
        if(cvor -> list)
            cvor -> zapis[j] = cvor -> zapis[j-1];
        else
            cvor -> ptr[j+1-smer] = cvor -> ptr[j-smer];
    }
    cvor -> n += 1;
    cvor -> val[i] = id;
    if(cvor -> list)
        cvor -> zapis[i] = z;
    else
    {
        cvor -> ptr[i + 1 -smer] = c;
        c->parent = cvor;
    }
}

void split(Cvor *parent, Cvor *pun, int m)
{
    int raz = ceil((double)m/2);
    Cvor *brat = napraviNoviCvor(parent, m, pun->list);
    ubaciKljucUCvor(parent, pun->val[raz - 1], nullptr, brat);

    brat -> ptr[m - raz] = pun -> ptr[m];
    if(brat->ptr[m - raz])
        brat -> ptr[m - raz]->parent = brat;
    for(int i = raz, j = 0; i < m; i++, j++)
    {
        brat->val[j] = pun->val[i];
        pun->val[i] = 0;
        if(brat->list)
        {
            brat->zapis[j] = pun->zapis[i];
            pun->zapis[i] = nullptr;
        }
        else
        {
            brat->ptr[j] = pun->ptr[i];
            brat->ptr[j]->parent = brat;
            pun->ptr[i] = nullptr;
        }
    }
    pun -> n = raz;
    brat -> n = m - raz;

    if(pun->list)
    {
        brat->ptr[0] = pun->ptr[0];
        pun->ptr[0] = brat;
    }
    else
    {
        pun->n -= 1;
        pun->val[raz] = 0;
        pun->ptr[raz + 1] = nullptr;
    }
}

Cvor* ubaciKljuc(Cvor *root, int m, long long int id, Zapis *z)
{
    int pom = 0;
    Cvor *list = nadjiList(root, id, pom);
    ubaciKljucUCvor(list, id, z);

    if(list -> n == m)
    {
        while(list != root and list -> n == m)
        {
            split(list->parent, list, m);
            list = list->parent;
        }
        if(root -> n == m)
        {
            Cvor *novi = napraviNoviCvor(nullptr, m, false);
            novi->ptr[0] = root;
            root->parent = novi;
            split(novi, root, m);
            root = novi;
        }
    }
    return root;
}

void nadjiKZapisa(Cvor *root)
{
    int n, br = 0;
    long long int id;
    ofstream file("izlaz.txt");
    cout << "unesite broj zapisa koji se pretrazuju" << endl;
    cin >> n;
    cout << "unesite zapise koji se pretrazuju" << endl;
    for(int i = 0; i < n; i++)
    {
        cin >> id;
        id = (id < 43000000000 ? id + 43000000000 : id);
        auto z = nadjiZapis(root, id, br);
        if(z)
            file << z->id << "|" << z->idb << "|" << z->idc << "|" << z->name << "|" << z->tax << "|" << z->balance << endl;
    }
    cout << "ukupan broj kroaka je " << br << endl;
}

void nadjiKSuccZapisa(Cvor *root)
{
    long long int id;
    int i = 0, br = 0, k;
    ofstream file("izlazKSucc.txt");
    cout << "unesite broj sukcesivnih zapisa koji se pretrazuju" << endl;
    cin >> k;
    cout << "unesite od kog zapisa krece pretraga" << endl;
    cin >> id;
    id = (id < 43000000000 ? id + 43000000000 : id);
    Cvor* list = nadjiList(root, id, br);
    br--;
    while(i < k and list)
    {
        for(int j = 0; j < list->n and i < k; j++)
            if(list->val[j] >= id)
            {
                auto z = list->zapis[j];
                file << z->id << "|" << z->idb << "|" << z->idc << "|" << z->name << "|" << z->tax << "|" << z->balance << endl;
                i++;
            }
        br++;
        list = list->ptr[0];
    }
    cout << "broj koraka je " << br << endl;
}

Zapis* procitajLiniju(istream &file)
{
    char c;
    auto z = new Zapis;
    file >> z->id;
    file.get(c);
    file >> z->idb;
    file.get(c);
    file >> z->idc;
    file.get(c);
    getline(file, z->name, '|');
    file >> z->tax;
    file.get(c);
    file >> z->balance;
    return z;
}

Cvor* unosIzFajla(int m)
{
    string s;
    Cvor *root = napraviNoviCvor(nullptr, m);
    //cout << "unesite naziv fajla:" << endl;
    //cin >> s;
    s = "CustomerAccount20.txt";
    ifstream file (s);

    if(!file.is_open())
    {
        cout << "ne postoji fajl sa unetim imenom, pokusajte ponovo" << endl;
        return nullptr;
    }
    auto z = procitajLiniju(file);
    while(!file.eof())
    {
        root = ubaciKljuc(root, m, z->id, z);
        z = procitajLiniju(file);
    }
    return root;
}

void postaviLevel(Cvor *root)
{
    queue <cvor *> queue;
    queue.push(root);

    while(!queue.empty())
    {
        cvor *tr = queue.front();
        queue.pop();
        for(int i = 0; i <= tr->n; i++)
            if(!tr->list)
            {
                tr->ptr[i]->level = tr->level + 1;
                queue.push(tr->ptr[i]);
            }
    }
}

void ispisiStablo(Cvor *root, int m)
{
    int curr = 30, prev;
    queue <cvor *> queue;
    queue.push(root);
    postaviLevel(root);

    while(true)
    {
        for(int k = 0; k < curr; k++ )
            cout << " ";

        if(queue.empty())
            return;
        cvor *tr = queue.front();
        queue.pop();

        for(int j = 0; j <= tr->n; j++)
        {
            if(!tr->list)
                queue.push(tr -> ptr[j]);
        }

        cout << "(";
        for(int j = 0; j < tr->n; j++)
            cout << tr -> val[j]%100000 << (j == tr->n - 1 ? "" : " ");
        cout << ") ";

        if(!queue.empty() and queue.front()->level != tr->level)
        {
            prev = curr / (m - 2) - 2;
            curr = (curr - m) / m;
            cout << endl;
        }

        else
            for(int l = 0; l < prev; l++)
                cout << " ";
    }
}

void obrisiStablo(cvor *root)
{
    queue <cvor *> queue;
    queue.push(root);

    while(!queue.empty())
    {
        cvor *tr = queue.front();
        queue.pop();

        for(int i = 0; i < tr->n + 1; i++)
        {
            if(tr->list && i != tr->n)
                delete tr->zapis[i];
            else if(!tr->list)
                queue.push(tr->ptr[i]);
        }
        delete tr;
    }
}

//smer -> ulevo = 0, udesno = 1
Cvor* nadjiBrata(Cvor *tr, int smer = 1)
{
    Cvor *p = tr->parent;
    for(int i = 0; p and i <= p->n; i++)
    {
        if(i and p->ptr[i] == tr and !smer)
            return p->ptr[i - 1];
        if(i != p->n and p->ptr[i] == tr and smer)
            return p->ptr[i + 1];
    }
    return nullptr;
}

int nadjiRazdvojni(Cvor *levi, Cvor *desni = nullptr)
{
    int i = 0;
    if(levi)
        for (; i < levi->parent->n and levi->parent->ptr[i] != levi; i++);
    else
        i = desni->parent->n - 1;
    return i;
}

void nadjiPrethodnika(Cvor* tr, long long int key)
{
    while(tr->parent and tr == tr->parent->ptr[tr->parent->n])
        tr = tr->parent;
    if(tr->parent)
        tr->parent->val[nadjiRazdvojni(tr)] = key;
}

//smer -> ulevo = 0, udesno = 1
void obrisiKljucIzLista(Cvor *cvor, long long int id, int smer = 1)
{
    int i = 0;
    while(i < cvor->n and id != cvor->val[i]) i++;
    for(int j = i; j < cvor->n; j++)
    {
        cvor->val[j] = cvor->val[j+1];
        if(cvor->list)
            cvor->zapis[j] = cvor->zapis[j+1];
        else
            cvor->ptr[j + smer] = cvor->ptr[j+1+smer];
    }
    cvor->ptr[cvor->n ] = nullptr;
    cvor->n--;
}

//zameni kljuc u nekom pretku sa list->val[list->n - 2]
void merge(Cvor *parent, Cvor *cvor, int m, long long int id = 0)
{
    int bru = ceil((double)m/2) - 1, brl = floor((double)m/2);
    Cvor* lbrat = nadjiBrata(cvor, 0);
    Cvor* dbrat = nadjiBrata(cvor, 1);
    if(cvor->list)
        obrisiKljucIzLista(cvor, id);

    if(cvor->n < (cvor->list ? brl : bru))
    {
        //pozajmica desni brat
        if(dbrat and dbrat->n - 1 >= (cvor->list ? brl : bru))
        {
            if(cvor->list)
            {
                parent->val[nadjiRazdvojni(cvor, dbrat)] = dbrat->val[0];
                ubaciKljucUCvor(cvor, dbrat->val[0], dbrat->zapis[0]);
                obrisiKljucIzLista(dbrat, dbrat->val[0]);
            }
            else
            {
                ubaciKljucUCvor(cvor, parent->val[nadjiRazdvojni(cvor, dbrat)], nullptr, dbrat->ptr[0]);
                parent->val[nadjiRazdvojni(cvor, dbrat)] = dbrat->val[0];
                obrisiKljucIzLista(dbrat, dbrat->val[0], 0);
            }
        }
            //pozajmica levi brat
        else if(lbrat and lbrat->n - 1 >= (cvor->list ? brl : bru))
        {
            if(cvor->list)
            {
                parent->val[nadjiRazdvojni(lbrat, cvor)] = lbrat->val[lbrat->n - 1];
                ubaciKljucUCvor(cvor, lbrat->val[lbrat->n - 1], lbrat->zapis[lbrat->n - 1]);
                obrisiKljucIzLista(lbrat, lbrat->val[lbrat->n - 1]);
            }
            else
            {
                ubaciKljucUCvor(cvor, parent->val[nadjiRazdvojni(lbrat, cvor)], nullptr, lbrat->ptr[lbrat->n]);
                parent->val[nadjiRazdvojni(lbrat, cvor)] = lbrat->val[lbrat->n - 1];
                obrisiKljucIzLista(lbrat, lbrat->val[lbrat->n - 1], 1);
            }
        }
        //merge
        else
        {
            if(dbrat)
            {
                if(!cvor->list)
                    ubaciKljucUCvor(cvor, cvor->parent->val[nadjiRazdvojni(cvor, dbrat)], nullptr, dbrat->ptr[0]);
                for(int i = 0; i < dbrat->n; i++)
                {
                    if(cvor->list)
                        ubaciKljucUCvor(cvor, dbrat->val[i], dbrat->zapis[i]);
                    else
                        ubaciKljucUCvor(cvor, dbrat->val[i], nullptr, dbrat->ptr[i+1]);
                }
                obrisiKljucIzLista(cvor->parent, cvor->parent->val[nadjiRazdvojni(cvor, dbrat)], 1);
            }
            else
            {
                if(!cvor->list)
                    ubaciKljucUCvor(cvor, cvor->parent->val[nadjiRazdvojni(lbrat, cvor)], nullptr, lbrat->ptr[lbrat->n], 1);
                for(int i = 0; i < lbrat->n; i++)
                {
                    if(cvor->list)
                        ubaciKljucUCvor(cvor, lbrat->val[i], lbrat->zapis[i], nullptr);
                    else
                        ubaciKljucUCvor(cvor, lbrat->val[i], nullptr, lbrat->ptr[i], 1);
                }
                obrisiKljucIzLista(cvor->parent, cvor->parent->val[nadjiRazdvojni(lbrat, cvor)], 0);
            }
        }
    }
}

Cvor* obrisiKljuc(Cvor *root, int m, long long int id)
{
    int pom = 0, bru = ceil((double)m/2) - 1, brl = floor((double)m/2);
    auto list = nadjiList(root, id, pom);
    auto z = nadjiZapis(root, id, pom);
    if(!z)
    {
        cout << "zapis ne postoji u stablu" << endl;
        return root;
    }

    while(list != root)
    {
        merge(list -> parent, list, m, (list->list ? id : 0));
        if(list->parent->n == 0 and list->parent == root)
        {
            delete root;
            root = list;
            root->parent = nullptr;
            break;
        }
        list = list -> parent;
        if(list -> n >= (list->list ? brl : bru))
            break;
    }
    list = nadjiList(root, 1, pom);
    while(list)
    {
        nadjiPrethodnika(list, list->val[list->n - 1]);
        list = list->ptr[0];
    }
    cout << "kljuc je obrisan!" << endl;
    return root;
}

void meni2()
{
    cout << "\nunesite redni broj zeljene stavke:\n\t(1) formiranje B+ stabla"
            "\n\t(2) ispis B+ stabla\n\t(3) dodavanje novog zapisa"
            "\n\t(4) brisanje zapisa\n\t(5) pretrazivanje jednog podatka\n\t"
            "(6) pretrazivanje k-podataka\n\t(7) pretrazivanje k-sukcesivnih podataka\n\t"
            "(8) brisanje B+ stabla\n\n\t(0) kraj programa\n\n";
}

int main(){
    int m, br, izbor;
    long long int id;
    auto *z = new Zapis;
    auto root = napraviNoviCvor(nullptr);
    root->level = 1;
    cout << "dobrodosli! :)" << endl;
    while(true)
    {
        meni2();
        cin >> izbor;
        switch(izbor)
        {
            case 0:
                cout << "prijatan dan! :)";
                return 0;

            case 1:
                if(!root->n)
                {
                    cout << "unesite red stabla"<< endl;
                    cin >> m;
                    root = unosIzFajla(m);
                    cout << "stablo je formirano!" << endl;
                }
                else
                    cout << "stablo vec postoji" << endl;
                break;

            case 2:
                if(root->n)
                    ispisiStablo(root, m);
                else
                    cout << "stablo nije uneto" << endl;
                break;

            case 3:
                if(root->n)
                {
                    cout << "unesite kljuc koji se dodaje" << endl;
                    cin >> id;
                    id = (id < 43000000000 ? id + 43000000000 : id);
                    z = procitajLiniju(cin);
                    root = ubaciKljuc(root, m, id, z);
                    cout << "kljuc je unet!" << endl;
                }
                else
                    cout << "stablo nije uneto" << endl;
                break;

            case 4:
                if(root->n)
                {
                    cout << "unesite kljuc koji se brise" << endl;
                    cin >> id;
                    id = (id < 43000000000 ? id + 43000000000 : id);
                    root = obrisiKljuc(root, m, id);
                }
                else
                    cout << "stablo nije uneto" << endl;
                break;

            case 5:
                if(root->n)
                {
                    cout << "unesite kljuc koji se pretrazuje" << endl;
                    cin >> id;
                    id = (id < 43000000000 ? id + 43000000000 : id);
                    br = 0;
                    z = nadjiZapis(root, id, br);
                    if(z)
                        cout << z->id << "|" << z->idb << "|" << z->idc << "|" << z->name << "|" << z->tax << "|" << z->balance << endl;
                    else
                        cout << "zapis ne postoji" << endl;
                }
                else
                    cout << "stablo nije uneto" << endl;
                break;

            case 6:
                if(root->n)
                    nadjiKZapisa(root);
                else
                    cout << "stablo nije uneto" << endl;
                break;

            case 7:
                if(root->n)
                    nadjiKSuccZapisa(root);
                else
                    cout << "stablo nije uneto" << endl;
                break;

            case 8:
                cout << "hehe :)" << endl;
                if(root->n)
                {
                    obrisiStablo(root);
                    root = napraviNoviCvor(nullptr);
                    root->level = 1;
                    cout << "stablo je obrisano!" << endl;
                }
                else
                    cout << "stablo nije uneto" << endl;
                break;

            default:
                cout << "izabrali ste nepostojecu opciju, pokusajte ponovo" << endl;
        }
    }
}