#include <iostream>
#include <string>
#include <cmath>
#include "polinom.h"
#include <stack>

using namespace std;

int main()
{
    try{
        Polinom pol1;
        cout<<"Unesite prvi polinom: ";
        cin>>pol1;
        Polinom pol2;
        cout<<"Unesite drugi polinom: ";
        cin>>pol2;
        cout<<"("<<pol1<<")+("<<pol2<<") = "<<pol1+pol2<<endl;
        cout<<"("<<pol1<<")-("<<pol2<<") = "<<pol1-pol2<<endl;
        cout<<"("<<pol1<<")*("<<pol2<<") = "<<(pol1*pol2)<<endl;
        int stepen;
        cout<<"Unesite stepen: ";
        cin>>stepen;
        cout<<"("<<pol1<<")^"<<stepen<<" = "<<(pol1^stepen)<<endl;
    }
    catch(const char poruka[]){
        cout<<endl<<poruka<<endl;
    }

    return 0;
}
