#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "list.h"

typedef int
    __attribute__((nonnull(2, 3))) (*list_cmp_func_t)(void *,
                                                      const struct list_head *,
                                                      const struct list_head *);

// typedef struct element{
//     struct list_head list;
//     int val;
//     int seq;
// } element_t;

// static inline void show_list(struct list_head *head)
// {
//     if (list_empty(head)){
//         printf("null\n");
//         return ;
//     }
//     element_t *cur;
    
//     list_for_each_entry (cur, head, list)
//         printf("%d.%d ", cur->val, cur->seq);
        
//     printf("\n");
// }

static inline int q_size(struct list_head *head)
{
    if (!head)
        return 0;

    int len = 0;
    struct list_head *li;

    list_for_each (li, head)
        len++;
    return len;
}

__attribute__((nonnull(1, 3))) void 
quick_sort(void *priv, struct list_head *head, list_cmp_func_t cmp);

__attribute__((nonnull(1, 3))) void 
quick_sort_mid(void *priv, struct list_head *head, list_cmp_func_t cmp);