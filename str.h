//
// Created by divan on 05/01/19.
//

#pragma once

#include "memory.h"
#include "allocator.h"

//
// String
//

struct String {
   int length;
   char *data;
};

inline bool str_blank(String str) {return str.length == 0 || !str.data;}
inline bool str_nonblank(String str) {return str.length > 0 && str.data;}

String str_copy(MemoryArena *arena, const char* str, int length);
String str_copy(MemoryArena *arena, const char* str);
String str_copy(MemoryArena *arena, String str);

String str_copy(Allocator *allocator, const char* str, int length);
String str_copy(Allocator *allocator, const char* str);
String str_copy(Allocator *allocator, String str);

bool str_equal(String a, String b);
bool str_equal(String a, const char* b);
bool str_start_with(String a, String b);
int str_cmp(String a, String b);

String str_print(MemoryArena *arena, const char *fmt, ...);

String as_string(char* str);

#define const_as_string(str) (String{sizeof(str)-1, const_cast<char*>(str)})

#define str_prt(s) s.length, s.data

//
// String builder
//

struct StringBuilder {
   i32 capacity;
   i32 length;
   char *buffer;
   MemoryArena* arena;
};

void strb_init(StringBuilder *sb, MemoryArena* arena);
void strb_destroy(StringBuilder *sb);
String strb_done(StringBuilder *sb);
void strb_reset(StringBuilder *sb);
void strb_print(StringBuilder *sb, const char *fmt, ...);
void strb_add(StringBuilder *sb, char str);
void strb_add(StringBuilder *sb, const char *str);
void strb_add(StringBuilder *sb, String str);
void strb_add(StringBuilder *sb, const char *str, i32 size);