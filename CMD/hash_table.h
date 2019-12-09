//
//  hash_table.h
//  CMD
//
//  Created by Adhiraj Singh on 12/2/19.
//  Copyright © 2019 Adhiraj Singh. All rights reserved.
//

/*
 
 Implementation of a basic Hash Table where the hashing function can be interchanged with whatever one wants.
 Keys are mapped to value pointers.
 When inserting an element, the key is copied and then stored as a key, value pair but the value pointer is stored as it is.
 
*/

#ifndef hash_table_h
#define hash_table_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_function.h"

// If the threshold is crossed, the capacity of the table will be doubled
#define HASH_TABLE_LOAD_FACTOR_THRESHOLD 0.65
// If set to 1, then even if there is one value at a specified index, a match will still be done
#define HASH_TABLE_ALWAYS_CHECK_KEY_MATCH 1
#define HASH_TABLE_DEFAULT_CAPACITY 43

///Structure to hold the values stored at a certain index in the array of the HashTable elements.
///Contains the array of corresponding key, value pairs at this index. Element stores an array to account for hash value clashes.
struct HashTableElement {
    ///Array of the pointers of data stored in the element
    char **values;
    ///Size of the values at the corresponding values array
    int *valuelens;
    char **keys;
    int *keylens;
    ///Number of items in this index. Ideally should be 1.
    int item_count;
};
typedef struct HashTableElement HashTableElement;

struct HashTable {
    int capacity;
    int total_elements;
    HashTableElement *elements;
    
    ///The hash function that will be used, can be interchanged
    HashingFunction hash_function;
};
typedef struct HashTable HashTable;

///Create a new hash table with default capacity
HashTable *hash_table_new (void);
///Create a new hash table with specified capacity
HashTable *hash_table_new_with_capacity (int capacity);
///Free all memory associated with a hash table
void hash_table_free (HashTable *table);

///Get value from the hash table for a specified key. valuelen is set to the length of the value.
char *hash_table_get_value (HashTable *table, char *key, int keylen, int *valuelen);
///Store a value in the hash table. Set valuelen = 0 to store the pointer to the value.
void hash_table_set_value (HashTable *table, char *value, int valuelen, char *key, int keylen);

#endif /* hash_table_h */
