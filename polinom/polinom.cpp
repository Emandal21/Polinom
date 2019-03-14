#include "polinom.h"
#ifndef POLINOM_CPP
#define POLINOM_CPP

//neclanice

char Znak(double a){
    if(a<0) return '-';
    else return '+';
}

int PretvoriUInt(char znak){
    return znak-'0';
}

int Prioritet(char operacija){
    if(operacija=='+' || operacija=='-') return 1;
    if(operacija=='*' ) return 2;
    else if(operacija=='^') return 3;
    else return 0;
}


void Izvrsi_Operaciju(stack<Polinom> &s, char operacija){
  Polinom rezultat;
  if(Prioritet(operacija)<=3) {
    if(s.size()<2)
      throw "Nedostaje operand";
    Polinom operand2 = s.top();
    s.pop();
    Polinom operand1 = s.top();
    s.pop();
    if(operacija == '+')
      rezultat = operand1+operand2;
    else if(operacija == '-'){
      rezultat = operand1-operand2;
    }
    else if(operacija == '*')
      rezultat = operand1*operand2;
    else if(operacija == '^') {
      rezultat = operand1^(int(operand2.Begin().vratiKoeficijent()));
    }
  }
  else {
    if(s.size()<1)
      throw "Nedostaje operand";
  }
  s.push(Polinom(rezultat));
}

//clanice

Polinom::Polinom(): br_clanova(0) {
    head=new Clan(0,0, nullptr, nullptr);
    tail=new Clan(0,0, nullptr, head);
    head->sljedeci=tail;
}

Polinom::Polinom(int stepen, double koeficijent): br_clanova(0){
    head=new Clan(0, 0, nullptr, nullptr);
    tail=new Clan(0, 0, nullptr, nullptr);
    head->sljedeci=tail->prethodni=new Clan(stepen, koeficijent, tail, head);
    br_clanova++;
}

void Polinom::dodajNaKraj(int stepen, double koeficijent){
    if(br_clanova > 0 && tail->prethodni->stepen == stepen) {
        tail->prethodni->koeficijent += koeficijent;
        if(tail->prethodni->koeficijent == 0) { obrisiClan(tail->prethodni->stepen); return;}
    }
    else{
        tail->prethodni = tail->prethodni->sljedeci = new Clan(stepen, koeficijent, tail, tail->prethodni);
        br_clanova++;
    }
}

void Polinom::dodajNaPocetak(int stepen, double koeficijent){
    if(br_clanova > 0 && head->sljedeci->stepen == stepen) {
        head->sljedeci->koeficijent += koeficijent;
        if(head->sljedeci->koeficijent == 0) { obrisiClan(head->sljedeci->stepen); return;}
    }
    else {
        head->sljedeci = head->sljedeci->prethodni = new Clan(stepen, koeficijent, head->sljedeci, head);
        br_clanova++;
    }
}
void Polinom::dodajClan(int stepen, double koeficijent){
    if(koeficijent==0) return;
    if(stepen >= head->sljedeci->stepen) {
        dodajNaPocetak(stepen, koeficijent);
        return;
    }
    else if(stepen <= tail->prethodni->stepen) { dodajNaKraj(stepen, koeficijent); return;}

    Polinom::Iterator it=(++Begin());
    while(it!=(--End()) && it.vratiStepen() > stepen) it++;
    if(it.vratiStepen()==stepen) it.trenutni->koeficijent+=koeficijent;
    else {
        Polinom::Clan* novi=new Clan(stepen, koeficijent, it.trenutni, it.trenutni->prethodni);
        it.trenutni->prethodni->sljedeci=novi;
        it.trenutni->prethodni=novi;
    }
    br_clanova++;
}

void Polinom::obrisiClan(int stepen){
    Polinom::Clan* pom;
    if(br_clanova != 0){
        if(stepen==head->sljedeci->stepen){
            pom=head->sljedeci;
            head->sljedeci->sljedeci->prethodni=head;
            head->sljedeci=head->sljedeci->sljedeci;

        }
        else if(stepen==tail->prethodni->stepen){
            pom=tail->prethodni;
            tail->prethodni->prethodni->sljedeci=tail;
            tail->prethodni=tail->prethodni->prethodni;
        }
        else{
            Polinom::Iterator it=(++Begin());
            while((it.trenutni->stepen) != stepen && it != (--End())) it++;
            if(it==(End())) throw "Clan za brisanje ne postoji!";
            pom=it.trenutni;
            it.trenutni->sljedeci->prethodni=it.trenutni->prethodni;
            it.trenutni->prethodni->sljedeci=it.trenutni->sljedeci;
        }
        br_clanova--;
        delete pom;
    }
}

void Polinom::obrisiSveClanove(){
    while(head->sljedeci != tail){
        obrisiClan(tail->prethodni->stepen);
    }
}

Polinom::Polinom(const Polinom& kopija): br_clanova(0){
    head=new Clan(0, 0, nullptr, nullptr);
    tail=new Clan(0, 0, nullptr, head);
    head->sljedeci=tail;
    Polinom::Clan* novi= kopija.head->sljedeci;
    for(novi; novi!=kopija.tail; novi=novi->sljedeci){
        dodajNaKraj(novi->vratiStepen(), novi->vratiKoeficijent());
    }
}

const Polinom& Polinom::operator=(const Polinom& kopija){
    if(this==&kopija) return *this;
    obrisiSveClanove();
    Polinom::Clan* novi= kopija.head->sljedeci;
    for(novi; novi!=kopija.tail; novi=novi->sljedeci){
        dodajNaKraj(novi->vratiStepen(), novi->vratiKoeficijent());
    }
    return *this;
}

Polinom operator+(const Polinom& prvi, const Polinom& drugi){
    if(prvi.brojClanova()==0) return drugi;
    if(drugi.brojClanova()==0) return prvi;
    Polinom rezultat;
    Polinom::Clan* it1=prvi.head->sljedeci;
    Polinom::Clan* it2=drugi.head->sljedeci;
    while(1){
        if(it1==prvi.tail && it2==drugi.tail) break;
        if(it1==prvi.tail){
            while(it2!=drugi.tail){
                rezultat.dodajClan(it2->vratiStepen(), it2->vratiKoeficijent());
                it2=it2->sljedeci;
            }
            break;
        }
        if(it2==drugi.tail){
            while(it1!=prvi.tail){
                rezultat.dodajClan(it1->vratiStepen(), it1->vratiKoeficijent());
                it1=it1->sljedeci;
            }
            break;
        }
        if(it1->vratiStepen() == it2->vratiStepen()){
            rezultat.dodajClan(it1->vratiStepen(), it1->vratiKoeficijent() + it2->vratiKoeficijent());
            it1=it1->sljedeci;
            it2=it2->sljedeci;
        }
        else if(it1->vratiStepen() > it2->vratiStepen()){
            rezultat.dodajClan(it1->vratiStepen(), it1->vratiKoeficijent());
            it1=it1->sljedeci;
        }

        else if(it2->vratiStepen() > it1->vratiStepen()){
            rezultat.dodajClan(it2->vratiStepen(), it2->vratiKoeficijent());
            it2=it2->sljedeci;
        }
    }
    return rezultat;
}

const Polinom operator-(const Polinom& prvi,const Polinom& drugi){
    Polinom rezultat;
    if(drugi.brojClanova()==0) return prvi;
    if(prvi.brojClanova()==0) {
        Polinom::Clan* it=drugi.head->sljedeci;
        for(it; it!=drugi.tail; it=it->sljedeci) rezultat.dodajClan(it->vratiStepen(), it->vratiKoeficijent()*(-1));
        return rezultat;
    }
    Polinom::Clan* it1=prvi.head->sljedeci;
    Polinom::Clan* it2=drugi.head->sljedeci;
    while(1){
        if(it1==prvi.tail && it2==drugi.tail) break;
        if(it1==prvi.tail){
            while(it2!=drugi.tail){
                rezultat.dodajClan(it2->vratiStepen(), -1*it2->vratiKoeficijent());
                it2=it2->sljedeci;
            }
            break;
        }
        if(it2==drugi.tail){
            while(it1!=prvi.tail){
                rezultat.dodajClan(it1->vratiStepen(), it1->vratiKoeficijent());
                it1=it1->sljedeci;
            }
            break;
        }
        if(it1->vratiStepen() == it2->vratiStepen()){
            rezultat.dodajClan(it1->vratiStepen(), it1->vratiKoeficijent() - it2->vratiKoeficijent());
            it1=it1->sljedeci;
            it2=it2->sljedeci;
        }
        else if(it1->vratiStepen() > it2->vratiStepen()){
            rezultat.dodajClan(it1->vratiStepen(), it1->vratiKoeficijent());
            it1=it1->sljedeci;
        }

        else if(it2->vratiStepen() > it1->vratiStepen()){
            rezultat.dodajClan(it2->vratiStepen(), -1*it2->vratiKoeficijent());
            it2=it2->sljedeci;
        }
    }
    return rezultat;
}

const Polinom operator*(const Polinom& prvi,const Polinom& drugi){
    Polinom rezultat;
    Polinom::Clan* brojac1=prvi.head->sljedeci;
    for(int i=0; i<prvi.br_clanova; i++){
        Polinom::Clan* brojac2=drugi.head->sljedeci;
        for(int j=0; j<drugi.brojClanova(); j++){
            rezultat.dodajClan(brojac1->vratiStepen()+brojac2->vratiStepen(), (brojac1->vratiKoeficijent())*(brojac2->vratiKoeficijent()));
            brojac2=brojac2->sljedeci;
        }
        brojac1=brojac1->sljedeci;
    }
    return rezultat;
}

const Polinom operator^(const Polinom& prvi, int stepen){
    if(stepen==0) {
        Polinom rez(0,1);
        return rez;
    }
    Polinom rez;
    if(stepen==1) return prvi;
    if(stepen%2==0) {
        rez=(prvi^stepen/2)*(prvi^stepen/2);
    }
    else {
        rez=(prvi^stepen/2)*(prvi^(stepen+1)/2);
    }
    return rez;
}

ostream& operator<<(ostream& Ispis, const Polinom& pol){
    if(pol.br_clanova==0){
        Ispis<<0;
        return Ispis;
    }
    if((pol.tail->prethodni)==pol.head->sljedeci){
        if(pol.head->sljedeci->stepen==0) Ispis<<pol.head->sljedeci->koeficijent;
        else if(pol.head->sljedeci->stepen!=0 && pol.head->sljedeci->stepen!=1) {
                if(abs(pol.head->sljedeci->koeficijent)!=1)Ispis<<pol.head->sljedeci->koeficijent<<"x^"<<pol.head->sljedeci->stepen;
                else if(pol.head->sljedeci->koeficijent==-1) Ispis<<Znak(pol.head->sljedeci->koeficijent)<<"x^"<<pol.head->sljedeci->stepen;
                else Ispis<<"x^"<<pol.head->sljedeci->stepen;
        }
        else if(pol.head->sljedeci->stepen!=0 && pol.head->sljedeci->stepen==1) {
                if(abs(pol.head->sljedeci->koeficijent)!=1)Ispis<<pol.head->sljedeci->koeficijent<<"x";
                else if(pol.head->sljedeci->koeficijent==-1) Ispis<<Znak(pol.head->sljedeci->koeficijent)<<"x";
                else Ispis<<"x";
        }
        return Ispis;
    }
    for(Polinom::Clan* clan=pol.head->sljedeci; clan!=pol.tail->prethodni; clan=clan->sljedeci){
        if(clan==pol.head->sljedeci){
            if(clan->stepen==0) Ispis<<clan->koeficijent<<" ";
            else if(clan->stepen!=0 && clan->stepen!=1) {
                if(abs(clan->koeficijent)!=1)Ispis<<clan->koeficijent<<"x^"<<clan->stepen<<" ";
                else if(clan->koeficijent==-1) Ispis<<Znak(clan->koeficijent)<<"x^"<<clan->stepen<<" ";
                else Ispis<<"x^"<<clan->stepen<<" ";
            }
            else if(clan->stepen!=0 && clan->stepen==1) {
                if(abs(clan->koeficijent)!=1)Ispis<<clan->koeficijent<<"x"<<" ";
                else if(clan->koeficijent==-1) Ispis<<Znak(clan->koeficijent)<<"x"<<" ";
                else Ispis<<"x"<<" ";
            }
        }
        else{
            if(clan->stepen==0) Ispis<<clan->koeficijent<<" ";
            else if(clan->stepen!=1) {
                if(abs(clan->koeficijent)!=1)Ispis<<Znak(clan->koeficijent)<<" "<<abs(clan->koeficijent)<<"x^"<<clan->stepen<<" ";
                else if(clan->koeficijent==-1) Ispis<<Znak(clan->koeficijent)<<" x^"<<clan->stepen<<" ";
                else Ispis<<Znak(clan->koeficijent)<<" x^"<<clan->stepen<<" ";
            }
            else if(clan->stepen==1) {
                if(abs(clan->koeficijent)!=1)Ispis<<Znak(clan->koeficijent)<<" "<<abs(clan->koeficijent)<<"x"<<" ";
                else if(clan->koeficijent==-1) Ispis<<Znak(clan->koeficijent)<<" x"<<" ";
                else Ispis<<Znak(clan->koeficijent)<<" x"<<" ";
            }
        }
    }
    if(abs(pol.tail->prethodni->koeficijent)!=1){
        if(pol.tail->prethodni->stepen==0) Ispis<<Znak(pol.tail->prethodni->koeficijent)<<" "<<abs(pol.tail->prethodni->koeficijent);
        else if(pol.tail->prethodni->stepen==1) Ispis<<Znak(pol.tail->prethodni->koeficijent)<<" "<<abs(pol.tail->prethodni->koeficijent)<<"x";
        else Ispis<<Znak(pol.tail->prethodni->koeficijent)<<" "<<abs(pol.tail->prethodni->koeficijent)<<"x^"<<pol.tail->prethodni->stepen;
    }
    else {
        if(pol.tail->prethodni->stepen==0) Ispis<<Znak(pol.tail->prethodni->koeficijent)<<" "<<1;
        else if(pol.tail->prethodni->stepen==1) Ispis<<Znak(pol.tail->prethodni->koeficijent)<<" x";
        else Ispis<<Znak(pol.tail->prethodni->koeficijent)<<" x^"<<pol.tail->prethodni->stepen;
    }

}

istream& operator>>(istream& Upis, Polinom& pol){
    stack<char> operacije;
    stack<Polinom> polinomi;
    char prethodni = '(';
    while(Upis.peek()!='\n') {
      while(Upis.peek()==' ')
        Upis.get();
      if(Upis.peek()>='0' && Upis.peek()<='9') {
        if(prethodni== ')' or prethodni == '0' or prethodni == 'x')
            throw "Pogresan unos, nedostaje operator";
        double broj;
        Upis>>broj;
        Polinom pol(0, broj);
        polinomi.push(pol);
        prethodni = '0';
       }

      if(Upis.peek()=='x'){
        Upis.get();
        if(prethodni=='x')
             throw "Pogresan unos, nedostaje operator";
        if(prethodni=='0' || prethodni==')')
             operacije.push('*');
        Polinom pol(1,1);
        polinomi.push(pol);
        prethodni='x';
       }
      else if(Upis.peek()=='(') {
        char znak;
        Upis>>znak;
        if(prethodni=='x')
          throw "Pogresan unos, nedostaje operator";
        if(prethodni=='0' || prethodni==')')
          operacije.push('*');
        operacije.push(znak);
        prethodni = znak;
      }
      else if(Upis.peek()==')') {
        char znak;
        Upis>>znak;
        while(!operacije.empty() && Prioritet(operacije.top())>0) {
            Izvrsi_Operaciju(polinomi,operacije.top());
            operacije.pop();
        }
        if(operacije.empty())
            throw "Neizbalansirane zagrade";
        operacije.pop();
        prethodni = znak;
      }
      else if(Prioritet(Upis.peek())>0) {
        char znak;
        Upis>>znak;
        if((prethodni=='(') && (znak=='+' || znak=='-')) {
          polinomi.push(Polinom(0, 0));
          prethodni = '0';
        }
        else if(Prioritet(prethodni)>0 || prethodni=='(')
            throw "Unijeli ste previse operatora";
        while(!operacije.empty() && Prioritet(operacije.top())>=Prioritet(znak)) {
            Izvrsi_Operaciju(polinomi,operacije.top());
            operacije.pop();
        }
        operacije.push(znak);
        prethodni = znak;
       }
    }
    while(!operacije.empty()) {
    if(Prioritet(operacije.top())==0)
      throw "Neizbalansirane zagrade";
    Izvrsi_Operaciju(polinomi,operacije.top());
    operacije.pop();
    }
    if(polinomi.size()!=1)
        throw "Pogresan unos";
    pol = polinomi.top();
    cin.get();
    return Upis;
}

Polinom::~Polinom(){
    obrisiSveClanove();
    delete head;
    delete tail;
}

#endif // POLINOM_CPP
