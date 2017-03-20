
/********************************************************************
ringbuffer.c - implements both a stack (filo) and queue (fifo).

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

///@file ringbuffer.c
///@author Marcus Henry Ewert
///@date 2012-01-27

#include "ringbuffer.h"

// Authors note: Jan 27, 2012
//
// This implementation might not be the fastest, but I was aiming more for
// clarity and simplicity.  I have been testing all functionality during the
// development to ensure there are not off-by-1 errors or the like


void ringbuff_init(ringbuff * rb, char * buffer, ringbuff_size_t buffsize)
{
  rb->buffer = buffer;
  rb->maxsize = buffsize;
  rb->count = 0;
  rb->front = 0;
  rb->back = 0;
}

static bool ringbuff_push_front_int(ringbuff *rb, char c, bool disableInterrupts)
{
  //if (disableInterrupts) interrupts_dis();

  //check if there is room
  if(rb->count >= rb->maxsize)
  {
    //indicate failure
    //if (disableInterrupts) interrupts_en();
    return 0;
  }

  //decrement front to indicate that we are adding a char
  if(rb->front == 0)
  {
    rb->front = rb->maxsize-1;
  }
  else
  {
    rb->front--;
  }

  rb->buffer[rb->front] = c;
  rb->count++;

  //if (disableInterrupts) interrupts_en();

  //indicate success
  return 1;
}

static bool ringbuff_push_back_int(ringbuff *rb, char c, bool disableInterrupts)
{


  //check if there is room
  if(rb->count >= rb->maxsize)
  {
    //indicate failure
    return 0;
  }

  //if (disableInterrupts) interrupts_dis();

  rb->buffer[rb->back] = c;
  rb->count++;
  //increment back to indicate that we are adding a char
  rb->back++;

  if(rb->back >= rb->maxsize)
  {
    rb->back = 0;
  }

  //if (disableInterrupts) interrupts_en();

  //indicate success
  return 1;
}

static bool ringbuff_pop_front_int(ringbuff *rb, char* c, bool disableInterrupts)
{

  if(rb->count <= 0)
  {
    return 0;
  }

  //if (disableInterrupts) interrupts_dis();

  *c = rb->buffer[rb->front];
  rb->front++;

  if(rb->front >= rb->maxsize)
  {
    rb->front = 0;
  }

  rb->count--;

  //if (disableInterrupts) interrupts_en();

  return 1;
}

static bool ringbuff_pop_back_int(ringbuff *rb, char* c, bool disableInterrupts)
{
  //check if the ringbuffer is empty
  if(rb->count <= 0)
  {
    //indicate failure
    return 0;
  }

  //if (disableInterrupts) interrupts_dis();

  //decrement back to get to the empty index
  if(rb->back == 0)
  {
    rb->back = rb->maxsize-1;
  }
  else
  {
    rb->back--;
  }
  *c = rb->buffer[rb->back];

  rb->count--;

  //if (disableInterrupts) interrupts_en();

  return 1;
}

bool ringbuff_push_front(ringbuff *rb, char c)
{
  ringbuff_push_front_int(rb,c,true);
}

bool ringbuff_pop_front(ringbuff *rb, char* c)
{
  ringbuff_pop_front_int(rb,c,true);
}

bool ringbuff_push_back(ringbuff *rb, char c)
{
  ringbuff_push_back_int(rb,c,true);
}

bool ringbuff_pop_back(ringbuff *rb, char* c)
{
  ringbuff_pop_back_int(rb,c,true);
}

bool ringbuff_push_front_s(ringbuff *rb, void * data, ringbuff_size_t n)
{
  char * cdata = data;
  int i;

  //see if there is room for the addition
  if(rb->count + n > rb->maxsize){
    //indicate failure if there is not
    return 0;
  }

  //interrupts_dis();

  for(i = n-1; i >= 0; i--)
  {
    ringbuff_push_front_int(rb, cdata[i],false);
  }

  //interrupts_en();

  return 1;
}

bool ringbuff_pop_front_s(ringbuff *rb, void * data, ringbuff_size_t n)
{
  char * cdata = data;
  int i;



  //see if there are enough bytes of data in the ringbuffer for this to work
  if(rb->count < n)
  {
    return 0;
  }

  //interrupts_dis();

  for(i = 0; i < n; i++)
  {
    ringbuff_pop_front_int(rb, &(cdata[i]),false);
  }

  //interrupts_en();

  return 1;
}

bool ringbuff_push_back_s(ringbuff *rb, void * data, ringbuff_size_t n)
{
  char * cdata = data;
  int i;

  //see if there is room for the addition
  if(rb->count + n > rb->maxsize)
  {
    return 0;
  }

  //interrupts_dis();

  for(i = 0; i < n; i++)
  {
    ringbuff_push_back_int(rb, cdata[i],false);
  }

  //interrupts_en();

  return 1;
}

bool ringbuff_pop_back_s(ringbuff *rb, void * data, ringbuff_size_t n)
{
  char * cdata = data;
  int i;

  //see if there are enough bytes of data in the ringbuffer for this to work
  if(rb->count < n)
  {
    return 0;
  }

  //interrupts_dis();

  for(i = n-1; i >= 0; i--)
  {
    ringbuff_pop_back_int(rb, &(cdata[i]),false);
  }

  //interrupts_en();

  return 1;
}

bool ringbuff_peek_back(ringbuff * rb, char * c, ringbuff_size_t n)
{
  ringbuff_size_t index;

  // n should be in the range from 0..rb->count
  if ( n >= rb->count)
  {
    return 0;
  }

  //interrupts_dis();

  //add 1 because back points 1 past the end
  n += 1;
  if( n > rb->back ){
    index = rb->back + rb->maxsize - n;
  }
  else{
    index = rb->back - n;
  }

  *c = rb->buffer[index];

  //interrupts_en();

  return 1;
}

bool ringbuff_peek_front(ringbuff * rb, char * c, ringbuff_size_t n)
{
  ringbuff_size_t index;

  // n should be in the range from 0..rb->count
  if ( n >= rb->count)
  {
    return 0;
  }

  //interrupts_dis();

  index=n + rb->front;
  if(index >= rb->maxsize)
  {
    index -= rb->maxsize;
  }

  *c = rb->buffer[index];

  //interrupts_en();

  return 1;
}

bool ringbuff_peek_back_s(ringbuff * rb, void * data, ringbuff_size_t struct_size, ringbuff_size_t n)
{
  ringbuff_size_t index_from_back=(n+1)*struct_size;
  ringbuff_size_t index;

  int i;
  if(index_from_back > rb->count)
  {
    return 0;
  }

  //interrupts_dis();

  if(index_from_back > rb->back)
  {
    index = rb->back + rb->maxsize - index_from_back;
  }
  else
  {
    index = rb->back - index_from_back;
  }

  for(i = 0; i < struct_size; i++)
  {
    ((char *)data)[i] = rb->buffer[index++];

    if(index >= rb->maxsize)
      index = 0;
  }

  //interrupts_en();

  return 1;
}

bool ringbuff_peek_front_s(ringbuff * rb, void * data, ringbuff_size_t struct_size, ringbuff_size_t n)
{
  ringbuff_size_t index_from_front=(n)*struct_size;
  ringbuff_size_t index;
  int i;

  if(index_from_front > rb->count)
  {
    return 0;
  }

  //interrupts_dis();

  index = index_from_front + rb->front;
  if(index > rb->maxsize)
  {
    index -= rb->maxsize;
  }

  for(i = 0; i < struct_size; i++)
  {
    ((char *)data)[i] = rb->buffer[index++];
    if(index >= rb->maxsize)
      index = 0;
  }

  //interrupts_en();

  return 1;
}
