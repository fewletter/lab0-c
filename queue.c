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

    struct list_head *fast = head->next;
    struct list_head *slow = head->next;

    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }
    list_del(slow);
    q_release_element(list_entry(slow, element_t, list));
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    struct list_head *next = head->next->next;
    struct list_head *node = head->next;

    while (node != head) {
        while (next != head) {
            element_t *ele_next = list_entry(next, element_t, list);
            element_t *ele_node = list_entry(node, element_t, list);
            if (strcmp(ele_node->value, ele_next->value)) {
                list_del(&ele_next->list);
                q_release_element(ele_next);
            } else
                next = next->next;
        }
        node = node->next;
    }

    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head || list_empty(head) || list_is_singular(head))
        return;

    struct list_head *node;
    list_for_each (node, head->next)
        list_move(node, node->next);
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
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    struct list_head *base = head->prev, *node = head->prev->prev;
    while (node != head) {
        element_t *criterion = list_entry(base, element_t, list);
        element_t *tmp = list_entry(node, element_t, list);
        if (strcmp(criterion->value, tmp->value) < 0) {
            criterion = tmp;
        } else {
            list_del(&tmp->list);
        }
        node = node->prev;
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending order */
int q_merge(struct list_head *head)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}

// struct list_head *mergeTwoLists(struct list_head *L1, struct list_head *L2)
// {

// }

// struct list_head *merge_sort(struct list_head *head)
// {

// }

/* Sort elements of queue in ascending order */
void q_sort(struct list_head *head) {}