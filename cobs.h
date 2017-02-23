
#include <stdint.h>
#include <stddef.h>

#include <vector>
#include <algorithm>

/* Copyright 2017, Markus Leitz. All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted, with or without modification.
 */

typedef std::vector<uint8_t> ByteSequence;

namespace cobs
{



int cobs_encode(const ByteSequence &input, ByteSequence &output)
{

    output.resize(input.size()+1);  // the encoded buffer is one element larger

    std::fill(output.begin(),       // set all elements of the output to zero, that ensures that first element is zero too 
              output.end(),
              uint8_t(0));

    std::copy(input.begin(),        // copy input to output  with offset of one element
              input.end(),
              output.begin()+1);


    auto next_zero_element =  output.begin();   // the first element contains zero , so point to it
    auto previous_zero_element = next_zero_element;  // on start , next and previous are identical

    while(next_zero_element != output.end() )
    {

        next_zero_element = std::find(std::next(next_zero_element),output.end(),uint8_t(0));   

        *previous_zero_element = std::distance(previous_zero_element,next_zero_element);

        previous_zero_element = next_zero_element;

    }


    return(0);
}

int cobs_decode(const ByteSequence &input,ByteSequence &output)
{

    output.resize(input.size()-1);
    ByteSequence tmp = ByteSequence(input);


    auto next_zero_element = tmp.begin();
    auto previous_zero_element = next_zero_element;

    while(next_zero_element != tmp.end())
    {

        next_zero_element += (*previous_zero_element);
        *previous_zero_element = uint8_t(0);
        previous_zero_element = next_zero_element;

    }

    std::copy(tmp.begin()+1,tmp.end(),output.begin());

    return(0);
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




