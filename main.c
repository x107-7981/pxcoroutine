#include <stdio.h>
#include <string.h>
#include "pxcoroutine.h"

unsigned long long c = 1ull;

void func2(pxcrt_handle hpxcrt,int x){
    if(x != 10)
        func2(hpxcrt,x+1);
    int a = x;
    pxcrt_await(hpxcrt);
    printf("AAAAAAAAAAAAAA!\n");
    printf("a : %d\n",a);
    return;
}

pxcrt_func func(pxcrt_handle hpxcrt, pxcrt_promise prm_ptr){
    func2(hpxcrt,0);
    printf("BBBBBBBBBBBBBB!\n");
}

pxcrt_handle func1(){
    int a=1,b=2,c=3,d=4;
    return pxcrt_create(NULL,NULL,4055,func,NULL);
}

int main(){
    printf("Hello World!\n");
    pxcrt_handle hpxcrt = func1();
    for(int i=0;i<=10;i++)
        pxcrt_recover(hpxcrt);
    printf("Good bye!\n");
    return 0;
}
