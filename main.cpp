#include <time.h>
#include <cstdint>
#include <iostream>
#include "cobs.h"
 


int main(int argc, char *argv[])
{


  int rc= 0;
  const int DATASIZE = 19;

  uint8_t in[] = {0,0,0,10,20,0,40,0,60,70,80,90,100,110,0,130,0,150,160};
  uint8_t out[DATASIZE + 1 + DATASIZE / 254];

  
  cobs::ByteSequence input( in, in+DATASIZE);
  cobs::ByteSequence output;

  output =	cobs::cobs_encode(input);
  cobs::ByteSequence result = cobs::cobs_decode(output);

 size_t written =  cobs_encode(in, DATASIZE, out);
  
  return(rc);


}