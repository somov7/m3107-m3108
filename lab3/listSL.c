#include <stdio.h>
#include <stdlib.h>
#include "listSL.h"
#include <string.h>

void push(node** head, char* data){
    node* tmp = malloc(sizeof(node));
    tmp->value = calloc(strlen(data) + 1, sizeof(char));
    strcpy(tmp->value, data);
    tmp->next = *head;
    (*head) = tmp;
}

char* pop(node** head){
    if (*head == NULL){
        printf("Error: list is empty");
        exit(-1);
    }
    char* res = (**head).value;
    *head = (*head)->next;
    return res;
}

node* getSecondLast(node*head){
    if (head == NULL)
        exit(-2);

    if (head->next == NULL)
        return NULL;

    while (head->next->next)
        head = head->next;

    return head;
}

char* popBack(node** head) {
    char* res;
    node *sLast = NULL;
    if (!head || !(*head))
        exit(-1);
    sLast = getSecondLast(*head);

    if (sLast == NULL){
        res = (*head)->value;
        free(*head);
        *head = NULL;
    }else{
        res = sLast->next->value;
        free(sLast->next);
        sLast->next = NULL;
    }
    return res;
}


