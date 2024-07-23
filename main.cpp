#include<iostream>
#include "hello.h"
#include<string>
using namespace std;
int main(){
    string s="hello world";
    for(int i=0;i<s.size();i++){
        s[i]=toupper(s[i]);
    }
    cout<<s<<endl;
    return 0;
}