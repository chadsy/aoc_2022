// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Stack Implementation for AOC driver

#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

typedef void *stackref;

//typedef union {
//    void *vp;
//    int val;
//} stackitem;
typedef int stackitem;

stackref stack_create(int min_cap);
void stack_destroy(stackref s);
void stack_push(stackref s, stackitem i);
void stack_insert(stackref s, stackitem i);
stackitem stack_pop(stackref s);
stackitem stack_peek(stackref s);
bool stack_is_empty(stackref s);
int stack_count(stackref s);
int stack_max(stackref s);
void stack_dump(stackref s);