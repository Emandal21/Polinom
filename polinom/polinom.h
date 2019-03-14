#ifndef POLINOM_H
#define POLINOM_H
#include <iostream>
#include <string>
#include <cmath>
#include<stack>

using namespace std;

class Polinom{
public:
    struct Clan{
        int stepen;
        double koeficijent;
        Clan *sljedeci, *prethodni;
        Clan(int s, double k, Clan* sljed, Clan* pret): stepen(s), koeficijent(k), sljedeci(sljed), prethodni(pret){}
        int vratiStepen(){ return stepen;}
        double vratiKoeficijent() {return koeficijent;}
    };
    class Iterator{
        Clan *trenutni;
    public:
        Iterator(): trenutni(nullptr) {}
        Iterator(Clan *trenutni):trenutni{trenutni}{}
        Iterator operator++(int) {
            Iterator stari = *this;
            trenutni = trenutni->sljedeci;
            return stari;
        }
        Iterator &operator++() {
            trenutni = trenutni->sljedeci;
            return *this;
        }
        Iterator operator--(int) {
            Iterator stari = *this;
            trenutni = trenutni->prethodni;
            return stari;
        }
        Iterator &operator--() {
            trenutni = trenutni->prethodni;
            return *this;
        }
        int vratiStepen(){return trenutni->stepen; }
        double vratiKoeficijent(){ return trenutni->koeficijent; }
        bool operator==(const Iterator it) { return trenutni == it.trenutni; }
        bool operator!=(const Iterator it) { return trenutni != it.trenutni; }
        friend class Polinom;
    };
    int br_clanova;
    Clan *head, *tail;
    void dodajNaKraj(int stepen, double koeficijent);
    void dodajNaPocetak(int stepen, double koeficijent);
    void dodajClan(int stepen, double koeficijent);
    void obrisiClan(int stepen);
    void obrisiSveClanove();
    bool nemaClanova(){ return !br_clanova; }
    Iterator Begin() {return head->sljedeci;}
    Iterator End(){return tail;}
    int brojClanova() const { return br_clanova; }
public:
    Polinom();
    Polinom(int stepen, double koeficijent);
    Polinom(const Polinom& kopija);
    const Polinom& operator=(const Polinom& kopija);
    friend Polinom operator+(const Polinom& prvi, const Polinom& drugi) ;
    friend const Polinom operator-(const Polinom& prvi, const Polinom& drugi);
    friend const Polinom operator*(const Polinom& prvi, const Polinom& drugi);
    friend const Polinom operator^(const Polinom& prvi, int stepen);
    friend istream& operator>>(istream& Upis, Polinom& pol);
    friend ostream& operator<<(ostream& Ispis, const Polinom& pol);
    ~Polinom();
};


#endif // POLINOM_H

