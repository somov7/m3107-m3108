#pragma once

#include <stdbool.h>

/**
 * im gonna push failed queries into stack
 * and when the time comes i'll return not only
 * the stack size(that equals quantity of failed requests)
 * but full list of failed requests.
 */

typedef struct node
{
    struct node *next;

    char *current_query;

    long long query_quantity;
    
} node;

typedef struct list
{

    struct node *last_node;

    long long data_counter;

} list;

/**
 * @brief
 * function to push failed queries
 * @param string
 */
void push(char *string, list *data_structure);

/**
 * @brief
 * function to return failed queries
 */
void pop(list *data_structure);

bool IsEmpty(list *data_structure);

/**
 * @brief
 *
 * @return size of stack
 */
long long GetSize(list *data_structure);

void clear(list *data_structure);

typedef struct data_base
{
    char *string;

    int size;

    int count_queries;

}data_base;

void add(data_base db, char *string);
