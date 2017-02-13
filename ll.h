#ifndef _XOR_LL_H
#define _XOR_LL_H

struct ll_node {
        void* data;
        struct ll_node* ptr;
};

struct ll_header {
        struct ll_node* head;
        struct ll_node* tail;

        size_t length;
};

#endif

