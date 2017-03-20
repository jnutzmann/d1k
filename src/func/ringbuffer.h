/********************************************************************
ringbuffer.h - implements both a stack (filo) and queue (fifo).

Copyright (c) 2012, Jonathan Nutzmann, Marcus Ewert
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of UMNSVP, University of Minnesota nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY JONATHAN NUTZMANN ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL JONATHAN NUTZMANN BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

/**
  \brief a ringbuffer structure

  Note that it is possible to have this pre-initialized using designated
  initializers as follows:

  \code
  #define mybuffsize 100
  char mybuffer[mybuffsize];
  ringbuff myringbuff = {
    .buffer = mybuffer,
    .maxsize = mybuffsize,
  };
  \endcode
  The important thing is to set .buffer to the memory location of a char* buffer
  and to set .maxsize equal to the size of the buffer

  At the very core this provides the ability to push and pop chars onto a
  ringbuffer and either operation can happen at the beginning or the end. For
  fifo functionality push on the back and pop off the front. For filo functionality
  push on the back and pop off the back.

  The _s versions of the pop and push commands allow structs (or any datatype)
  to be pushed or popped onto the queue by interpreting the input data as a
  stream of bytes.
*/

#include "stdbool.h"
#include "stdint.h"

typedef uint16_t ringbuff_size_t;

typedef struct ringbuff_struct
{
    /// the location in memory the data is actually stored
    char * buffer;

    /** \brief internally used for the index of the front of the buffer

        This is the index of the element that is on the top
    */
    ringbuff_size_t front;

    /** \brief internally used for the index of the back of the buffer

        This is the index of the first vacant place in the buffer, 1 past the last
        occupied spot. With this implementation both front and back should be
        initialized to 0, meaning they can be left out of designated initializers.
    */
    ringbuff_size_t back;

    /** the size of the buffer (also the maximum size in bytes the ringbuffer can
        hold) */
    ringbuff_size_t maxsize;

    /// The current number of objects in the ringbuffer
    ringbuff_size_t count;
} ringbuff;

/**
  \brief initializes a ringbuffer structure to work with the buffer pointed by
  buffer of size buffsize

  Note: ringbuffers can also be initialized statically using designated
  initializers

  \param rb a pointer to the ringbuffer to initialize
  \param buffer a pointer to a memory buffer designated for this ringbuffer
  \param buffsize the size of the buffer for this ringbuffer in bytes
*/
void ringbuff_init(ringbuff * rb, char * buffer, ringbuff_size_t buffsize);


/**
  \brief pushes a char onto the front of the ringbuffer

  \param rb a pointer to the ringbuffer to add the character to
  \param c the character to add

  \returns 1 if successful, 0 if failed(the buffer is already full)
*/
bool ringbuff_push_front(ringbuff *rb, char c);

/**
  \brief pops a char off of the front of the ringbuffer

  \param rb a pointer to the ringbuffer to add the character to
  \param c a pointer to the character into which to put the result

  \returns 1 if successful, 0 if failed(the buffer is already empty)
*/
bool ringbuff_pop_front(ringbuff *rb, char* c);



/**
  \brief pushes a char onto the back of the ringbuffer

  \param rb a pointer to the ringbuffer to add the character to
  \param c the character to add

  \returns 1 if successfull, 0 if failed(the buffer is already full)
*/
bool ringbuff_push_back(ringbuff *rb, char c);


/**
  \brief pops a char off of the back of the ringbuffer

  \param rb a pointer to the ringbuffer to add the character to
  \param c a pointer to the character into which to put the result

  \returns 1 if successfull, 0 if failed(the buffer is already empty)
*/
bool ringbuff_pop_back(ringbuff *rb, char* c);


/**
  \brief pushes n bytes (or a struct) onto the front of the ringbuffer

  \param rb a pointer to the ringbuffer to add the data to
  \param data a pointer to the first byte of data to be added
  \param n the number of bytes to add

  \returns 1 if successfull, 0 if failed(the buffer doesn't have room)

  This will actually push the last byte of data onto the ringbuffer first so
  that the data will come off in the same order. This allows for pushing and
  poping structs from the front without accidentially switching endianness.
  \code
  char mybuffer[100];
  ringbuff rb = { .buffer=mybuffer, .maxsize=100};
  char c;
  struct {char a, char b} mystruct = {.a = 'a', .b = 'b'};

  //If you push structs on, you can easily pop those structs off
  ringbuff_push_front_s(&rb, &mystruct, sizeof(mystruct));

  ringbuff_pop_front_s(&rb, &mystruct, sizeof(mystruct));
  //mystruct.a is still 'a'
  //mystruct.b is still 'b'

  //The following is an example of bad coding, because structs were pushed on,
  //but characters where popped off. But, this might be insightfull for the
  //underlying mechanics
  ringbuff_push_front_s(&rb, &mystruct, sizeof(mystruct));
  ringbuff_pop_back(&rb, &c);
  //c is b
  ringbuff_pop_back(&rb, &c);
  //c is a
  \endcode

  If you push structs on, try to pop the structs off as full structs as
  demonstrated above.

*/
bool ringbuff_push_front_s(ringbuff *rb, void * data, ringbuff_size_t n);

/**
  \brief pops n bytes ( or a struct ) off of the front of the ringbuffer

  \param rb a pointer to the ringbuffer to pop the data from
  \param data a pointer to the location to store the first byte of data
  \param n the number of bytes to pop

  \returns 1 if successfull, 0 if failed(the buffer is already empty)

*/
bool ringbuff_pop_front_s(ringbuff *rb, void * data, ringbuff_size_t n);


/**
  \brief pushes n bytes (or a struct) onto the back of the ringbuffer

  \param rb a pointer to the ringbuffer to add the data to
  \param data a pointer to the first byte of data to be added
  \param n the number of bytes to add

  \returns 1 if successfull, 0 if failed(the buffer doesn't have room)

  This pushes the first byte first
*/
bool ringbuff_push_back_s(ringbuff *rb, void * data, ringbuff_size_t n);

/**
  \brief pops n bytes ( or a struct ) off of the back of the ringbuffer

  \param rb a pointer to the ringbuffer to pop the data from
  \param data a pointer to the location to store the first byte of data
  \param n the number of bytes to pop

  \returns 1 if successfull, 0 if failed(the buffer is already empty)

  Actually pops the last byte first
*/
bool ringbuff_pop_back_s(ringbuff *rb, void * data, ringbuff_size_t n);


/**
  \brief peeks at the nth char from the back of the ringbuffer

  \param rb a pointer to the ringbuffer to add the character to
  \param c a pointer to the character into which to put the result
  \param n the number from the end char that you want (0 is the last
  element)

  \returns 1 if successfull, 0 if failed(the buffer is smallert than n)
*/
bool ringbuff_peek_back(ringbuff * rb, char * c, ringbuff_size_t n);


/**
  \brief peeks at the nth char from the front of the ringbuffer

  \param rb a pointer to the ringbuffer to add the character to
  \param c a pointer to the character into which to put the result
  \param n the number from the start char that you want (0 is the first
  element)

  \returns 1 if successfull, 0 if failed(the buffer is smaller than n)
*/
bool ringbuff_peek_front(ringbuff * rb, char * c, ringbuff_size_t n);

/**
  \brief peeks at the nth struct from the back of the ringbuffer

  \param rb a pointer to the ringbuffer to add the character to
  \param data a pointer to the structure into which to put the result
  \param struct_size the size of the struct s
  \param n the number from the end char that you want (0 is the last
  element)

  \returns 1 if successfull, 0 if failed(the buffer is smallert than n*s_n)

  Note, this assumes that the buffer is entirely filled with structures of the
  same size
*/
bool ringbuff_peek_back_s(ringbuff * rb, void * data, ringbuff_size_t
struct_size, ringbuff_size_t n);

/**
  \brief peeks at the n-1th struct from the back of the ringbuffer

  \param rb a pointer to the ringbuffer to add the character to
  \param data a pointer to the structure into which to put the result
  \param struct_size the size of the struct s
  \param n the number from the end char that you want (0 is the last
  element)

  \returns 1 if successfull, 0 if failed(the buffer is smallert than n*s_n)

  Note, this assumes that the buffer is entirely filled with structures of the
  same size
*/
bool ringbuff_peek_front_s(ringbuff * rb, void * data, ringbuff_size_t
struct_size, ringbuff_size_t n);

#endif //RINGBUFFER_H
