#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (!q) {
        return;
    }

    list_ele_t *cursor = q->head, *forward;
    while (cursor) {
        free(cursor->value);
        forward = cursor->next;
        free(cursor);
        cursor = forward;
    }

    free(q);
    return;
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
    if (!q) {
        return false;
    }

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        return false;
    }

    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);

    newh->next = q->head;
    if (q->head) {
        q->head = newh;
    } else {
        newh->next = NULL;
        q->head = newh;
        q->tail = newh;
    }

    ++q->size;
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
    if (!q) {
        return false;
    }

    list_ele_t *newt = malloc(sizeof(list_ele_t));
    if (!newt) {
        return false;
    }

    newt->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newt->value) {
        free(newt);
        return false;
    }
    strncpy(newt->value, s, strlen(s) + 1);

    if (!q->head) {
        newt->next = NULL;
        q->head = newt;
        q->tail = newt;
    } else {
        newt->next = q->tail->next;
        q->tail->next = newt;
        q->tail = newt;
    }

    ++q->size;
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
    if (!q || !q->head) {
        return false;
    }

    if (sp != NULL) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_ele_t *cursor = q->head;
    q->head = q->head->next;
    free(cursor->value);
    free(cursor);
    --q->size;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
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
    if (!q || !q->head || !q->head->next) {
        return;
    }

    list_ele_t *done = NULL, *cursor = q->head, *next;
    q->tail = q->head;
    while (cursor) {
        next = cursor->next;
        cursor->next = done;
        done = cursor;
        cursor = next;
    }
    q->head = done;

    return;
}

/* Get the middle node
 * The original list must be at least length of 2
 */
void get_middle(list_ele_t *head, list_ele_t **l, list_ele_t **r)
{
    list_ele_t *fast = head->next, *slow = head;

    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    *l = head;
    *r = slow->next;
    slow->next = NULL;

    return;
}

void merge_sort(list_ele_t **head)
{
    // Early return
    if (!(*head) || !(*head)->next) {
        return;
    }

    // Split list
    list_ele_t *l = NULL, *r = NULL;
    get_middle(*head, &l, &r);

    merge_sort(&l);
    merge_sort(&r);

    // merge left and right
    list_ele_t *done;
    if (strcmp(l->value, r->value) < 0) {
        done = l;
        l = l->next;
    } else {
        done = r;
        r = r->next;
    }

    *head = done;
    while (l && r) {
        if (strcmp(l->value, r->value) < 0) {
            done->next = l;
            l = l->next;
        } else {
            done->next = r;
            r = r->next;
        }
        done = done->next;
    }

    done->next = l ? l : r;
    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || q->size <= 1) {
        return;
    }

    merge_sort(&(q->head));

    // Update q->tail
    list_ele_t *temp = q->head;
    while (temp->next) {
        temp = temp->next;
    }
    q->tail = temp;

    return;
}
