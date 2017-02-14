/* Copyright 2011, Jacques Fortier. All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted, with or without modification.
 */
#include <stdint.h>
#include <stddef.h>

#include <vector>
#include <algorithm>


namespace cobs
{
	int cobs_encode(const std::vector<uint8_t> &input, std::vector<uint8_t> &output)
	{
		output.resize(input.size()+1);
		std::fill(output.begin(),
				  output.end(),
				  uint8_t(0));
		
		std::copy(input.begin(),
			      input.end(),
				  output.begin()+1);


	 
		auto zero_element =  std::find(output.begin(),output.end(),uint8_t(0));

		while(zero_element != output.end() )
		{


			auto previous_zero_element = zero_element;

			zero_element = std::find(std::next(zero_element),output.end(),uint8_t(0));

			*previous_zero_element = std::distance(previous_zero_element,zero_element);

			 previous_zero_element = zero_element;

		}

		
		return(0);
	}

	int cobs_decode(const std::vector<uint8_t> &input, std::vector<uint8_t> &output)
	{
		std::fill(output.begin(),output.end(),1);

		auto cobsCode = input.begin();
	    auto out_element = output.begin();
 
		int numberOfElements = *cobsCode -1;
 
        std::copy(input.begin()+1,input.begin()+1+numberOfElements,out_element);
        
        while(out_element != output.end())
        {   
            
			out_element = out_element + numberOfElements;
            *out_element = 0;
			++out_element;

			cobsCode =  cobsCode +  numberOfElements;
			++cobsCode;

            numberOfElements = *cobsCode -1;

			std::copy(cobsCode+1,cobsCode+1+numberOfElements,out_element);

		}
        return(0);
	}
}

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


typedef   std::vector<uint8_t> byteSequence;
typedef   std::vector<uint16_t> wordSequence;
typedef   std::vector<uint32_t> DWORDSequence;



template<typename T>
int cobs_encode(const std::vector<T> &input, std::vector<T> &output)
{
	size_t input_index = 0;
  size_t output_index = 1;
  size_t code_index = 0;
  T code = 1;  // count non zero bytes + 1
   
  output.resize( (input.size() ) + ( input.size()) / 254 + 2);
   
  while(input_index < input.size())
  {
    if(input[input_index] == 0)
    {
      output[code_index] = code;
      code = 1; // reset counter
      code_index = output_index++;
      input_index++;
    }
    else
    {
      output[output_index++] = input[input_index++];
      code++; // it is non zero -> increment
      if(code == 0xFF)
      {
        output[code_index] = code;
        code = 1;  
        code_index = output_index++;
      }
    }
  }
  
  output[code_index] = code;

  return 0;
}

template<typename T>
int cobs_encode(const T &input, T &output)

{
  size_t input_index = 0;
  size_t output_index = 1;
  size_t code_index = 0;
  uint8_t code = 1;  // count non zero bytes + 1
   
  output.resize( (input.size() ) + ( input.size()) / 254 + 2);
   
  while(input_index < input.size())
  {
    if(input[input_index] == 0)
    {
      output[code_index] = code;
      code = 1; // reset counter
      code_index = output_index++;
      input_index++;
    }
    else
    {
      output[output_index++] = input[input_index++];
      code++; // it is non zero -> increment
      if(code == 0xFF)
      {
        output[code_index] = code;
        code = 1;  
        code_index = output_index++;
      }
    }
  }
  
  output[code_index] = code;

  return 0;

}



void test()
{
  byteSequence input = byteSequence(11,0);
  byteSequence output;
  int rc = cobs_encode<byteSequence>(input,output);
}
