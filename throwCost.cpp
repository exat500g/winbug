#include <iostream>
#include <time.h>

using namespace std;

int main(int , char *[])
{
    unsigned int maxLoop=1000000000;
    {
        volatile int doSomething=0;  //禁用优化以模拟做其他事的效果
        unsigned int i = 1;

        clock_t start = clock();
        while(1){
            do{
                if(i >= maxLoop){
                    goto ENDLOOP;
                }
            }while(false);
            doSomething++;
            i++;
        }
ENDLOOP:

        cout<<"Time Break is : "<<clock() - start<<endl;

        i = 1;
        start = clock();
        while(1){
            try{
                if(i >= maxLoop){
                    throw i;
                }
            }catch(unsigned int k){
                break;
            }
            doSomething++;
            i++;
        }
        cout<<"Time Throw is : "<<clock() - start<<endl;
    }
    return 0;
}

