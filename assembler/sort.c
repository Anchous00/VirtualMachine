#include "sort.h"

void sort_labels( struct labels_t *labels )
{
    int n = labels->labels_cnt;
    if (n <= 1) return;

    for (int i = 1; i < n; i++) {
        int j = i;
        while (j > 0 && labels->Keys[j - 1] > labels->Keys[j])
        {
            keys_swap(&labels->Keys[j - 1], &labels->Keys[j]);
            labels_swap(&labels->Labels[j - 1], &labels->Labels[j]);
            j--;
        }
    }

    return;
}

void keys_swap( hash_t *x1, hash_t *x2 )
{
    hash_t t = *x1;
    *x1 = *x2;
    *x2 = t;

    return;
}

void labels_swap( int *x1, int *x2 )
{
    int t = *x1;
    *x1 = *x2;
    *x2 = t;

    return;
}
