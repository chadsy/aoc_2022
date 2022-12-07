// AOC 2022 results in C
// @chadsy
// Copyright (C) 2022 Chad Royal
// MIT License http://opensource.org/licenses/MIT
//
// Driver utilities for the Advent of Code 2022 contest

#include <ctype.h>
#include "aoc22.h"
#include "aocstack.h"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Simple stack that stores type `stackitem` which is currently just an int

typedef struct {
    int max;
    int sp;
    stackitem *data;
} internal_stack;

void stack_grow(internal_stack *stack);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

stackref stack_create(int min_cap) {
    internal_stack *stack = calloc(1, sizeof(internal_stack));
    stack->max = max(min_cap, 100);
    stack->data = calloc(stack->max, sizeof(stackitem));
    return (stackref) stack;
}

void stack_destroy(stackref s) {
    internal_stack *stack = (internal_stack *)s;
    free(stack->data);
    free(stack);
}

void stack_push(stackref s, stackitem i) {
    internal_stack *stack = (internal_stack *)s;
    if (stack->sp == stack->max) {
        stack_grow(stack);
    }
    stack->data[stack->sp] = i;
    stack->sp++;
}

void stack_insert(stackref s, stackitem i) {
    internal_stack *stack = (internal_stack *)s;
    if (stack->sp == stack->max) {
        stack_grow(stack);
    }
    memmove(stack->data + 1, stack->data, stack->sp * sizeof(stackitem));
    stack->data[0] = i;
    stack->sp++;
}

stackitem stack_pop(stackref s) {
    internal_stack *stack = (internal_stack *)s;
    if (stack->sp > 0) {
        return stack->data[--stack->sp];
    }
    return 0;
}

stackitem stack_peek(stackref s) {
    internal_stack *stack = (internal_stack *)s;
    if (stack->sp > 0) {
        return stack->data[stack->sp - 1];
    }
    return 0;
}

bool stack_is_empty(stackref s) {
    return ((internal_stack *)s)->sp == 0;
}

int stack_count(stackref s) {
    return ((internal_stack *)s)->sp;
}

int stack_max(stackref s) {
    return ((internal_stack *)s)->max;
}

void stack_dump(stackref s) {
    internal_stack *stack = (internal_stack *)s;
    if (stack->sp) {
        printf("bot: ");
        for (int i = 0; i <= min(stack->sp - 1, 10); i++) {
            printf("[%c] ", stack->data[i]);
        }
        printf(": top\n");
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void stack_grow(internal_stack *stack) {
    stack->max *= 2;
    stack->data = realloc(stack->data, stack->max * sizeof(stackitem));
}
