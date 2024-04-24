#include <stddef.h>
#include <stdlib.h>

#include "quick.h"

//以 linux api 建構

void quick_sort(void *priv, struct list_head *head, list_cmp_func_t cmp)
{
    int n = q_size(head);
    int i = 0;
    int max_level = 2 * n;
    struct list_head **begin = malloc(sizeof(struct list_head *) * max_level);
    LIST_HEAD(result);    

    begin[0] = malloc(sizeof(struct list_head));
    INIT_LIST_HEAD(begin[0]);
    list_splice_tail_init(head, begin[0]);

    while (i >= 0)
    {
        if (!list_is_singular(begin[i]) && !list_empty(begin[i])){
            /*not singular*/
            LIST_HEAD(left);
            LIST_HEAD(right);
            LIST_HEAD(tmp);

            struct list_head *pivot  = begin[i]->next;
            list_del(pivot);                //將 pivot 從原有的list去除            


            while(!list_empty(begin[i])){
                struct list_head *cur  = begin[i]->next;
                if (cmp(priv,cur ,pivot) >= 0 ){  // if not equal the sorting will be unstable
                    list_move_tail(cur, &right);
                } else {
                    list_move_tail(cur, &left);   
                }
            }

            begin[i+1] = malloc(sizeof(struct list_head));
            INIT_LIST_HEAD(begin[i+1]);

            begin[i+2] = malloc(sizeof(struct list_head));
            INIT_LIST_HEAD(begin[i+2]);
            
            list_splice_tail_init(&left, begin[i]);
            list_splice_tail_init(&right, begin[i+2]);

            list_add(pivot, &tmp); 
            list_splice_tail_init(&tmp, begin[i+1]);
   
            i += 2;

        } else { 
            /*singular*/
            if (!list_empty(begin[i])){
                /* not empty*/
                struct list_head *pivot  = begin[i]->next;
                list_add(pivot, &result);

                free(begin[i]);
                begin[i] = NULL;               
            }
            i--;
        }
    }
    free(begin);
    list_splice_tail_init(&result, head);
}