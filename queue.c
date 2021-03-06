#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create an empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;
    q->head = q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q)
        return;

    while (q->head) {
        list_ele_t *prev = q->head;
        q->head = q->head->next;
        free(prev->value);
        free(prev);
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *new_element = q_new_element(s);
    if (!new_element)
        return false;
    new_element->next = q->head;
    if (q->head == NULL)
        q->tail = new_element;
    q->head = new_element;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;
    list_ele_t *new_element = q_new_element(s);
    if (!new_element)
        return false;
    if (q->tail == NULL) {
        q->head = q->tail = new_element;
        return true;
    }
    q->tail->next = new_element;
    q->tail = new_element;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    if (sp) {
        memset(sp, '\0', bufsize);
        strncpy(sp, q->head->value, bufsize - 1);
    }
    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    q->size--;
    free(tmp->value);
    free(tmp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
size_t q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || !q->head->next)
        return;

    list_ele_t *prev = NULL;
    list_ele_t *curr = q->head;
    list_ele_t *next = q->head->next;
    q->tail = q->head;
    while (next) {
        curr->next = prev;
        prev = curr;
        curr = next;
        next = next->next;
    }
    curr->next = prev;
    q->head = curr;
}

list_ele_t *q_element_reverse(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *rest = q_element_reverse(head->next);
    head->next->next = head;
    head->next = NULL;
    return rest;
}

list_ele_t *q_list_tail(list_ele_t *head)
{
    if (!head)
        return NULL;
    list_ele_t *tail = head;
    while (tail->next)
        tail = tail->next;
    return tail;
}

void merge_sort(list_ele_t **head, size_t size)
{
    if (!(*head) || !((*head)->next))
        return;

    list_ele_t *rhead = (*head)->next;
    list_ele_t *lhead = *head;

    for (int i = 0; i < (size >> 1) - 1; i++) {
        rhead = rhead->next;
        lhead = lhead->next;
    }
    lhead->next = NULL;
    lhead = *head;

    merge_sort(&lhead, size >> 1);
    if (size % 2 == 1)
        merge_sort(&rhead, (size >> 1) + 1);
    else
        merge_sort(&rhead, size >> 1);

    *head = NULL;
    list_ele_t **tmp = head;

    while (rhead && lhead) {
        if (strcmp(rhead->value, lhead->value) < 0) {
            *tmp = rhead;
            rhead = rhead->next;
        } else {
            *tmp = lhead;
            lhead = lhead->next;
        }
        tmp = &((*tmp)->next);
    }

    *tmp = rhead ? rhead : lhead;
}
/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head || !q->head->next)
        return;
    merge_sort(&q->head, q->size);
    q->tail = q_list_tail(q->head);
}

/*
 * Initialize a new queue element
 */
list_ele_t *q_new_element(char *s)
{
    list_ele_t *new_element = malloc(sizeof(list_ele_t));
    if (!new_element)
        return NULL;
    char *new_value = malloc((strlen(s) + 1) * sizeof(char));
    if (!new_value) {
        free(new_element);
        return NULL;
    }
    memset(new_value, '\0', strlen(s) + 1);
    strncpy(new_value, s, strlen(s) + 1);
    new_element->value = new_value;
    new_element->next = NULL;
    return new_element;
}