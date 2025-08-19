#include<bits/stdc++.h>
using namespace std;

int GCD(int a, int b){
    if(a==0){
        return b;
    }
    return GCD(b%a, a);
}

int main(){
    int a, b;
    cout<<"enter two numbers: ";
    cin>>a>>b;

    int g1=GCD(a, b);
    int g2=GCD(b, a);

    cout<<"GCD of (a,b): "<<g1<<endl;
    cout<<"GCD of (b,a): "<<g2<<endl;
}