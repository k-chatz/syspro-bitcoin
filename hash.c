#include <stdio.h>
#include "hash.h"

struct item *hash_table[2048] = { NULL }; /* Initialize all of them to NULL */

/* Calculate the hash */
int hash_get(int key)
{
    /* Extremely simple hash alg, just take the lower bits of the key */
    return (key % (sizeof(hash_table)/sizeof(hash_table[0])));
}

void hash_insert(struct item *i)
{
    int index = hash_get(i->key);

    /* Insert onto the beginning of the list */
    i->hash_link = hash_table[index];
    hash_table[index] = i;
}

struct item *hash_index(int key)
{
    int index = hash_get(key);

    struct item *cur;
    for (cur = hash_table[index]; cur; cur = cur->hash_link)
        if (cur->key == key)
            return cur;

    return NULL;
}