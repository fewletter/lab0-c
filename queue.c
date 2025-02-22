#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    if (!l)
        return;

    element_t *node, *next;
    list_for_each_entry_safe (node, next, l, list) {
        list_del(&node->list);
        q_release_element(node);
    }
    free(l);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    INIT_LIST_HEAD(&element->list);
    int len = strlen(s);
    element->value = malloc((len + 1) * sizeof(char));
    if (!element->value) {
        free(element);
        return false;
    }
    strncpy(element->value, s, len);
    *(element->value + len) = '\0';
    list_add(&element->list, head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    INIT_LIST_HEAD(&element->list);
    int len = strlen(s);
    element->value = malloc((len + 1) * sizeof(char));
    if (!element->value) {
        free(element);
        return false;
    }
    strncpy(element->value, s, len);
    *(element->value + len) = '\0';
    list_add_tail(&element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *target = list_first_entry(head, element_t, list);
    if (sp) {
        strncpy(sp, target->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }

    list_del_init(&target->list);
    return target;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;

    element_t *target = list_last_entry(head, element_t, list);
    if (sp) {
        strncpy(sp, target->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }

    list_del_init(&target->list);
    return target;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int size = 0;
    struct list_head *node = NULL;
    list_for_each (node, head)
        size++;
    return size;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;

    struct list_head *left = head->next;
    struct list_head *right = head->prev;

    while (left != right && left->next != right) {
        left = left->next;
        right = right->prev;
    }
    list_del(right);
    q_release_element(list_entry(right, element_t, list));
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head) || list_is_singular(head))
        return false;
    bool diff = false;
    struct list_head *curr, *safe;
    list_for_each_safe (curr, safe, head) {
        struct list_head *next = curr->next;
        element_t *curr_entry = list_entry(curr, element_t, list);
        element_t *next_entry = list_entry(next, element_t, list);
        if ((next != head) && (!strcmp(curr_entry->value, next_entry->value))) {
            list_del(curr);
            q_release_element(curr_entry);
            diff = true;
        } else if (diff) {
            list_del(curr);
            q_release_element(curr_entry);
            diff = false;
        }
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *node, *safe;
    for (node = (head)->next, safe = node->next; node != (head) && safe != head;
         node = node->next, safe = node->next) {
        list_move(node, safe);
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;

    struct list_head *front = head->prev, *now = head, *next = NULL;
    while (next != head) {
        next = now->next;
        now->next = front;
        now->prev = next;
        front = now;
        now = next;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || k <= 1)
        return;

    struct list_head *node, *start, *anchor = head;
    LIST_HEAD(empty);
    int count = 0;
    int time_mark = 0;
    int size = q_size(head);
    list_for_each_safe (node, start, head) {
        count++;
        if (count <= k && (size - time_mark) >= k) {
            list_move(node, &empty);
            if (count == k) {
                list_splice_init(&empty, anchor);
                count = 0;
                time_mark += k;
                anchor = start->prev;
            }
        }
    }
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    struct list_head *node, *safe;
    char *s = list_entry(head->prev, element_t, list)->value;
    for (node = (head)->prev, safe = node->prev; node != (head);
         node = safe, safe = node->prev) {
        element_t *tmp = list_entry(node, element_t, list);
        if (node != head->prev) {
            if (strcmp(s, tmp->value) < 0) {
                s = tmp->value;
            } else {
                list_del(&tmp->list);
                q_release_element(tmp);
            }
        }
    }

    return q_size(head);
}

struct list_head *mergeTwolists(struct list_head *L1, struct list_head *L2)
{
    struct list_head *head = NULL, **ptr = &head, **node = NULL;
    while (L1 && L2) {
        element_t *L1_entry = list_entry(L1, element_t, list);
        element_t *L2_entry = list_entry(L2, element_t, list);
        node = strcmp(L1_entry->value, L2_entry->value) < 0 ? &L1 : &L2;
        *ptr = *node;
        ptr = &(*ptr)->next;
        *node = (*node)->next;
    }
    *ptr = (struct list_head *) ((uintptr_t) L1 | (uintptr_t) L2);
    return head;
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (list_is_singular(head))
        return list_entry(head, queue_contex_t, chain)->size;

    struct list_head *merge_node = NULL;

    queue_contex_t *q_head = list_entry(head->next, queue_contex_t, chain);
    queue_contex_t *iter;
    list_for_each_entry (iter, head, chain) {
        iter->q->prev->next = NULL;
        merge_node = mergeTwolists(merge_node, iter->q->next);
        INIT_LIST_HEAD(iter->q);
    }

    q_head->q->next = merge_node;
    struct list_head *curr = q_head->q, *next = curr->next;
    while (next) {
        next->prev = curr;
        curr = next;
        next = next->next;
    }
    curr->next = q_head->q;
    q_head->q->prev = curr;

    return q_size(q_head->q);
}

struct list_head *merge_sort(struct list_head *head)
{
    if (!head || !head->next)
        return head;
    struct list_head *slow = head;
    for (struct list_head *fast = head->next; fast && fast->next;
         fast = fast->next->next) {
        slow = slow->next;
    }
    struct list_head *mid = slow->next;
    slow->next = NULL;
    struct list_head *left = merge_sort(head), *right = merge_sort(mid);
    return mergeTwolists(left, right);
}

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
    head->prev->next = NULL;
    head->next = merge_sort(head->next);
    struct list_head *curr = head, *next = curr->next;
    while (next) {
        next->prev = curr;
        curr = next;
        next = next->next;
    }
    curr->next = head;
    head->prev = curr;
}