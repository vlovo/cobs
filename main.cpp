#include <time.h>
#include <cstdint>
#include <time.h>
#include <iostream>
#include "cobs.h"
 


int main(int argc, char *argv[])
{


  int rc= 0;

  uint8_t tmp[] = {0,0,0,10,20,0,40,0,60,70,80,90,100,110,0,130,0,150,160};
    
  
  cobs::ByteSequence input( tmp, tmp+19 );
  cobs::ByteSequence output;

  output =	cobs::cobs_encode(input);
  input = cobs::cobs_decode(output);

  return(rc);

}