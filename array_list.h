//
// Created by divan on 30/01/19.
//

#pragma once

#include <cassert>

#include "types.h"
#include "util.h"
#include "allocator.h"

#define ARRAY_LIST_BLOCK_SIZE 128
#define ARRAY_LIST_BLOCK_MASK 0x7F

template<typename T>
struct ArrayListBlock {
   T data[ARRAY_LIST_BLOCK_SIZE];
   i64 count;
   ArrayListBlock<T> *prev;
   ArrayListBlock<T> *next;
};

struct ArrayListCursor {
   void *block;
   i64 index;
};

template<typename T>
struct ArrayList {
   Allocator *allocator;
   i64 count;
   ArrayListBlock<T> *first;
   ArrayListBlock<T> *last;
};

template<typename T>
void arl_init(ArrayList<T> *arl, Allocator *allocator = nullptr) {
   arl->allocator = allocator;
   arl->count = 0;
   arl->first = nullptr;
   arl->last = nullptr;
}

template<typename T>
ArrayList<T> arl_make(Allocator *allocator = nullptr) {
   return ArrayList<T>{allocator, 0, nullptr, nullptr};
}

template<typename T>
void arl_grow(ArrayList<T> *arl) {
   ArrayListBlock<T> *new_block = std_alloc_type(arl->allocator, ArrayListBlock<T>);
   new_block->count = 0;
   new_block->next = nullptr;

   if (arl->last) {
      arl->last->next = new_block;
      new_block->prev = arl->last;
      arl->last = new_block;
   } else {
      arl->first = new_block;
      arl->last = new_block;
   }
}

template<typename T>
T *arl_push(ArrayList<T> *arl, T item) {
   if (!arl->last || arl->last->count == array_size(arl->last->data)) arl_grow(arl);
   assert(arl->last->count < array_size(arl->last->data));

   arl->count++;
   auto ptr = arl->last->data + (arl->last->count++);
   *ptr = item;
   return ptr;
}

template<typename T>
T *arl_push(ArrayList<T> *arl) {
   if (!arl->last || arl->last->count == array_size(arl->last->data)) arl_grow(arl);
   assert(arl->last->count < array_size(arl->last->data));

   arl->count++;
   return arl->last->data + (arl->last->count++);
}

template<typename T>
bool arl_pop(ArrayList<T> *arl) {
   if (!arl->last) return false;

   if (arl->last->count > 0) {
      arl->last->count--;
   } else {
      auto block = arl->last;
      arl->last = block->prev;

      if (arl->last) {
         arl->last->next = nullptr;
      } else {
         arl->first = nullptr;
      }

      std_free(arl->allocator, block);
   }
   return true;
}

template<typename T>
bool arl_cursor_step(ArrayList<T> *arl, ArrayListCursor *cursor) {
   if (!cursor->block) {
      cursor->block = arl->first;
      cursor->index = 0;
   } else {
      cursor->index++;
      auto arl_block = (ArrayListBlock<T>*)cursor->block;
      if (cursor->index >= arl_block->count) {
         cursor->block = arl_block->next;
         cursor->index = 0;
      }
   }

   return cursor->block;
}

template<typename T>
T *arl_cursor_get(ArrayListCursor cursor) {
   if (!cursor.block) return nullptr;
   auto arl_block = (ArrayListBlock<T>*)cursor.block;
   return arl_block->data + cursor.index;
}