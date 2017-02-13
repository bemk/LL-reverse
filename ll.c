#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ll.h"

/* Let's create a pointer type that allows the compiler to do maths. Make it
 * 64-bits, for simplicity sake, though compiler optimisation could be done here */
typedef unsigned long long pointer;

static struct ll_node* ll_node_get(struct ll_header* list,
                unsigned int position)
{
        struct ll_node* iterator = list->head;
        struct ll_node* last = NULL;
        int idx = 0;
        for (; idx < position; idx++) {
                struct ll_node* tmp = iterator;
                if (iterator != NULL) {
                        /* Convert pointer to maths pointer */
                        pointer ptr = (pointer) iterator->ptr;
                        /* Xor the pointer to back and forth with the back
                         * pointer */
                        ptr ^= (pointer) last;

                        /* Update the back pointer */
                        last = tmp;
                        /* And update the next pointer */
                        iterator = (struct ll_node*) ptr;

                        /* Rinse and repeat until we have found our desired
                         * location or hit null. */
                } else {
                        /* Ehrm, we hit null, this can't be good. */
                        return NULL;
                }
        }
        return iterator;
}

void ll_iterate(struct ll_header* list, void (*func)(void*))
{
        struct ll_node* iterator = list->head;
        struct ll_node* last = NULL;

        while (iterator != NULL) {
                func(iterator->data);

                pointer ptr = (pointer) iterator->ptr;

                ptr ^= (pointer) last;

                last = iterator;
                iterator = (struct ll_node*) ptr;
        }
}

static int remove_head_internal(struct ll_node** list_end)
{

        /* Get the pointer in the second node */
        struct ll_node* next = (*list_end)->ptr;
        if (next == NULL) {
                /* Return error if there is no next node */
                return -1;
        }

        /* Convert this pointer to a mathy one */
        pointer ptr = (pointer) next->ptr;
        /* Xor the head pointer out of the next pointer */
        ptr ^= (pointer) (*list_end);

        /* Free up the head */
        memset(*list_end, 0, sizeof(**list_end));
        free(*list_end);
        /* Update the head (or taiL) position in the header */
        *list_end = next;
        next->ptr = (struct ll_node*)ptr;

        /* Return success */
        return 0;
}

static void remove_implementation(struct ll_header* list, int idx, int head)
{
        if (idx != 0) {
                fprintf(stderr, "Removing from the middle of the list is not "
                                "yet implemented");
                /* Removing from the inside of the list not yet implemented */
                return;
        }

        int retval = 0;
        retval = remove_head_internal((head == 0) ? &list->tail : &list->head);

        if (retval == -1) {
                /* Our internal function didn't work, because there was only 1
                 * node left */
                memset(list->head, 0, sizeof(*list->head));
                free(list->head);

                list->head = NULL;
                list->tail = NULL;
        }

        /* And update the length of the list */
        list->length--;
}

void ll_remove_head(struct ll_header* list)
{
        remove_implementation(list, 0, 1);
}

void ll_remove_tail(struct ll_header* list)
{
        remove_implementation(list, 0, 0);
}

static void append_internal(struct ll_node** list_end, struct ll_node* node)
{
        /* Convert the pointer in the tail to a mathy one */
        pointer ptr = (pointer) (*list_end)->ptr;
        /* Xor the next node to it */
        ptr ^= (pointer) node;

        /* Make sure that the node has a back reference */
        node->ptr = (*list_end);

        /* Update the pointer in the tail node */
        (*list_end)->ptr = (struct ll_node*) ptr;
        /* And update the header to have the new tail (or head) */
        (*list_end) = node;
}

static long long append_implementation(struct ll_header* list, void* data,
                int head)
{
        if (list == NULL) {
                return -1;
        }

        /* Allocate a new node */
        struct ll_node* node = malloc(sizeof(*node));
        memset(node, 0, sizeof(*node));
        node->data = data;

        /* Make sure we're not going to be referencing null pointers */
        if (list->head == NULL) {
                list->head = node;
                list->tail = node;
                list->length++;
                return 0;
        }
        /* For prepending, use the head, for appending use the tail */
        else if (head == 0) {
                append_internal(&list->tail, node);
                return (list->length)++;
        } else {
                append_internal(&list->head, node);
                list->length++;
                return 0;
        }
}

long long ll_append(struct ll_header* list, void* data)
{
        return append_implementation(list, data, 0);
}

long long ll_prepend(struct ll_header* list, void* data)
{
        return append_implementation(list, data, 1);
}

struct ll_header* ll_create()
{
        struct ll_header* head = malloc(sizeof(*head));

        memset(head, 0, sizeof(*head));

        return head;
}

void ll_delete(struct ll_header* list)
{
        if (list == NULL) {
                return;
        }

        if (list->head == NULL) {
                /* if we have no members, it's pretty darn easy. */
                memset(list, 0, sizeof(*list));
                free(list);
                return;
        }

        if (list->head == list->tail) {
                /* Looks like we have only one member */
                memset(list->head, 0, sizeof(*(list->head)));
                free(list->head);
                memset(list, 0, sizeof(*list));
                free(list);
        } else {
                /* We have more than one member, loop through all of them until
                 * they're gone */
                while (list->length != 0) {
                        ll_remove_head(list);
                }
                /* And let another part of this function handle the actual
                 * cleanup */
                ll_delete(list);
        }
}

void ll_reverse(struct ll_header* list)
{
        if (list == NULL) {
                return;
        }

        /* Because this list has no actual direction, we can just swap the head
         * and tail pointers and be done with it. */
        list->head = (struct ll_node*) ((pointer) list->head
                        ^ (pointer) list->tail);
        list->tail = (struct ll_node*) ((pointer) list->tail
                        ^ (pointer) list->head);
        list->head = (struct ll_node*) ((pointer) list->head
                        ^ (pointer) list->tail);

}

#ifdef STANDALONE

void print_word(void* data)
{
        char* str = (char*) data;
        printf("Data: %s\n", str);
}

int main(int argc, char** argv)
{
        char* words[] = { "hello", "world", "hello", "happy", "cat" };

        size_t count = sizeof(words) / sizeof(char*);
        struct ll_header* list = ll_create();
        size_t idx = 0;
        for (; idx < count; idx++) {
                ll_append(list, words[idx]);
        }

        ll_iterate(list, print_word);
        ll_reverse(list);
        printf("Reversed the list\n");
        ll_iterate(list, print_word);
        ll_remove_tail(list);
        ll_remove_tail(list);
        printf("Removed nodes\n");
        ll_iterate(list, print_word);
        ll_reverse(list);
        printf("Reversed list\n");
        ll_iterate(list, print_word);

        ll_delete(list);

}
#endif
