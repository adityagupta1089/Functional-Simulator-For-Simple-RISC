#include <stdio.h>

int f(int a,int b,int c);
int g(int a,int b,int c);
int h(int a,int b,int c,int d);

int main(){
        int a=1;
        int b=10;
        int c=10;
        int d;
        d=f(a,b,c);
        printf("%d",d);
}

int f(int a,int b,int c){
        int i;
        for(i=0; i<5; i++) {
                a=b+c;
                b=a-c;
                c=c+g(a,b,c);
        }
        return a+b+c;
}

int g(int a,int b,int c){
        a=h(a,b,c,0);
        b=h(a,b,c,1);
        c=h(a,b,c,2);
        return a+b+c;
}
int h(int a,int b,int c,int d){
        switch(d) {
        case 0: return a + b - c;
        case 1: return a - b + c;
        case 2: return a + b + c;
        }
        return 0;
}
