#include "header.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>

bool IsEmpty(list *data_structure)
{
    if (data_structure->last_node == NULL)
        return 1;
    else
        return 0;
}

void push(char *string, list *data_structure)
{
    node *new_node = (node *)(calloc(1, sizeof(node)));

    new_node->current_query = (char *)(calloc((int)strlen(string), sizeof(char)));

    memcpy(new_node->current_query, string, (int)strlen(string));

    new_node->next = data_structure->last_node;

    data_structure->last_node = new_node;

    new_node = NULL;

    data_structure->data_counter++;

    data_structure->last_node->query_quantity++;
}

void pop(list *data_structure)
{
    while (!IsEmpty(data_structure))
    {
        struct node *temp = data_structure->last_node;

        data_structure->last_node = data_structure->last_node->next;

        printf("%s\n", temp->current_query);

        data_structure->data_counter--;

        free(temp);
    }
}

long long GetSize(list *data_structure)
{
    return data_structure->data_counter;
}

void clear(list *data_structure)
{
    while (!IsEmpty(data_structure))
    {
        struct node *temp = data_structure->last_node;

        data_structure->last_node = data_structure->last_node->next;

        data_structure->data_counter--;

        free(temp);
    }
}