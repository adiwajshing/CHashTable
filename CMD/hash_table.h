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

///If the threshold is crossed, the capacity of the table will be doubled
#define HASH_TABLE_LOAD_FACTOR_THRESHOLD 0.6
///If set to 1, then even if there is one value at a specified index, a match will still be done
#define HASH_TABLE_ALWAYS_CHECK_KEY_MATCH 1
#define HASH_TABLE_DEFAULT_CAPACITY 43

typedef uint32_t HashKeyType;
typedef HashKeyType (*HashingFunction)(char *data, int len);

struct HashTableElement {
    char **data_pointer;
    char **keys;
    int *keylens;
    
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

///Get value from the hash table for a specified key
char *hash_table_get_value (HashTable *table, char *key, int keylen);
///Store a value in the hash table
void hash_table_set_value (HashTable *table, char *value, char *key, int keylen);

#endif /* hash_table_h */
