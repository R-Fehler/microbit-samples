#include <stdio.h>

int a = -50;
long sum1= 0;
long sum2= 0;
#define bufferSize 40

int main(void){
int buffer1[bufferSize];
int buffer2[bufferSize];

for(int i = 0;i<bufferSize;i++){
buffer1[i] = a;
a = a+1;
printf("buffer1: %d \n", a);
}
for(int i = 0;i<bufferSize;i++){
    buffer2[i] = a;
    a++;
    printf("buffer2: %d \n", a);
}

for(int i = 0 ;i<bufferSize ; i++){
    sum1 = sum1 + buffer1[i];
    sum2 = sum2 + buffer2[i];
    printf("sum1: %d \n",sum1);
    printf("sum2: %d\n",sum2);
    printf("sum1*sum2= %d \n",(sum1*sum2)/100);
    }
}