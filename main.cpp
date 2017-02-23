
#include "cobs.h"
#include <time.h>
#include <iostream>


int main(int argc, char *argv[])
{
    int rc= 0;


    uint8_t tmp[100];
    uint8_t out[101];
    ByteSequence output;

    for(int k=0; k<100; ++k)
    {


        for(int i=0; i< 100; ++i)
        {
            tmp[i] = rand() % 4;

            if(tmp[i] != 0 ) tmp[i]*=10;
        }

        ByteSequence input( tmp, tmp+100 );

        clock_t start = clock();

        for(int j=0; j<1000000; ++j)
        {







            /*cobs::cobs_encode(input,output);
            cobs::cobs_decode(output,input);*/
            cobs_encode(tmp,100,out);
            cobs_decode(out,101,tmp);
            //cobs::cobs_decode(output,input);
        }

        clock_t end = clock();

        std::cout << end-start << "\n";
    }

    int k =0;
    std::cin >>  k;
    return(rc);

}