
#include <stdint.h>
#include <stddef.h>

#include <vector>
#include <algorithm>
#include <iterator>

/* Copyright 2017, Markus Leitz. All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted, with or without modification.
 */



namespace cobs
{
	typedef std::vector<uint8_t> ByteSequence;


ByteSequence cobs_encode(const ByteSequence &input)
{
  ByteSequence output;
  auto next_zero_byte = input.begin();
  auto previous_zero_byte = input.begin();

  while(next_zero_byte != input.end() )
  {

    next_zero_byte = std::find(next_zero_byte,
                               input.end(),
                               uint8_t(0));

    auto dist = std::distance(previous_zero_byte,next_zero_byte);

	// clip to  max distance
    dist = dist < 254 ? dist: 254;

    if(dist == 254)	next_zero_byte = previous_zero_byte + 254; 

    output.push_back(dist+1);  // add code byte to output

    output.insert(output.end(), previous_zero_byte, next_zero_byte);  //insert block of bytes between to code bytes , e.g two zeros

    if(   dist != 254
          && next_zero_byte != input.end() )
    {
      std:: advance(next_zero_byte,1);//next_zero_byte++;   // if we found a zero we move iterator to prepare for next std::find
    }

    previous_zero_byte = next_zero_byte;
  }

  if(input[input.size()-1] == uint8_t(0)) output.push_back(uint8_t(1)); // last element is zero , add 1 to output


  return(output);
}


ByteSequence cobs_decode(const ByteSequence &input )
{
  ByteSequence output;

  auto next_code_byte = input.begin();
  auto previous_code_byte = input.begin();

  while(next_code_byte != input.end() )
  {
   
    std::advance(next_code_byte,*next_code_byte);

    output.insert(output.end(),previous_code_byte+1,next_code_byte);

    if(    *previous_code_byte != 0xFF
           && next_code_byte != input.end())
    {
      output.push_back(0); //restore zero byte only in case if code byte was not 0xFF
    }

    previous_code_byte = next_code_byte;



  }

  return(output);
}


} // end namespace





/* Copyright 2011, Jacques Fortier. All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted, with or without modification.
 */

/* Stuffs "length" bytes of data at the location pointed to by
 * "input", writing the output to the location pointed to by
 * "output". Returns the number of bytes written to "output".
 *
 * Remove the "restrict" qualifiers if compiling with a
 * pre-C99 C dialect.
 */
size_t cobs_encode(const uint8_t *  input, size_t length, uint8_t *  output)
{
  size_t read_index = 0;
  size_t write_index = 1;
  size_t code_index = 0;
  uint8_t code = 1;

  while(read_index < length)
  {
    if(input[read_index] == 0)
    {
      output[code_index] = code;
      code = 1;
      code_index = write_index++;
      read_index++;
    }
    else
    {
      output[write_index++] = input[read_index++];
      code++;

      if(code == 0xFF)
      {
        output[code_index] = code;
        code = 1;
        code_index = write_index++;
      }
    }
  }

  output[code_index] = code;

  return write_index;
}

/* Copyright 2011, Jacques Fortier. All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted, with or without modification.
 */

/* Unstuffs "length" bytes of data at the location pointed to by
 * "input", writing the output * to the location pointed to by
 * "output". Returns the number of bytes written to "output" if
 * "input" was successfully unstuffed, and 0 if there was an
 * error unstuffing "input".
 *
 * Remove the "restrict" qualifiers if compiling with a
 * pre-C99 C dialect.
 */
size_t cobs_decode(const uint8_t *  input, size_t length, uint8_t *  output)
{
  size_t read_index = 0;
  size_t write_index = 0;
  uint8_t code;


  while(read_index < length)
  {
    code = input[read_index];

    if(read_index + code > length && code != 1)
    {
      return 0;
    }

    read_index++;

    for(uint8_t i = 1; i < code; i++)
    {
      output[write_index++] = input[read_index++];
    }

    if(code != 0xFF && read_index != length)
    {
      output[write_index++] = '\0';
    }
  }

  return write_index;
}




