#include<bits/stdc++.h>
using namespace std;

int GCD(int a, int b, int &count){
    count++;
    if(a == 0) return b;
    return GCD(b % a, a, count);
}

int main(){
    int a, b;
    cout<<"Enter two numbers: ";
    cin>>a>>b;

    int count = 0;
    int g = GCD(a, b, count);

    cout<<"GCD = "<<g<<endl;
    cout<<"Iterations = "<<count<<endl;
}
