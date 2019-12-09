//
//  hash_table.c
//  CMD
//
//  Created by Adhiraj Singh on 12/2/19.
//  Copyright © 2019 Adhiraj Singh. All rights reserved.
//

#include "hash_table.h"
#include <math.h>

HashTable *hash_table_new (void) {
    return hash_table_new_with_capacity(HASH_TABLE_DEFAULT_CAPACITY);
}
HashTable *hash_table_new_with_capacity (int capacity) {
    HashTable *table = (HashTable *) malloc(sizeof(HashTable));
    table->capacity = capacity;
    
    table->elements = (HashTableElement *) malloc(sizeof(HashTableElement) * table->capacity);
    memset(table->elements, 0, sizeof(HashTableElement) * table->capacity);
    
    table->total_elements = 0;
    table->hash_function = hash_function_default;
    
    return table;
}

/// free all the memory associated with an element in the table
void free_element (HashTableElement *element, int item_count) {
    for (int i = 0; i < item_count;i++) {
        free(*(element->keys+i));
        if (element->valuelens[i] > 0) { // if the value was copied and stored in the table, only free the memory then; otherwise memory allocated by the other programmer might get freed
            free(*(element->values+i));
        }
    }
    
    // free all pointers
    free(element->values);
    free(element->valuelens);
    free(element->keys);
    free(element->keylens);
    
    // set them to null
    element->values = NULL;
    element->valuelens = NULL;
    element->keys = NULL;
    element->keylens = NULL;
}
void hash_table_free (HashTable *table) {
    for (int i = 0; i < table->capacity;i++) {
        free_element(table->elements+i, table->elements[i].item_count);
    }
    
    free(table->elements);
    free(table);
}
/// Returns the next prime number larger than the one specified, assumes the number given in the argument is odd
int next_prime_number (int num) {

    while (1) {
        
        int sq = sqrt((double)num);
        int i;
        for (i = 2; i <= sq;i++) {
            if (num % i == 0) {
                break;
            }
        }
        if (i > sq) {
            return num;
        }
        num += 2;
    }
}
/// Increases the capacity of the table whenever the table is filled to a certain extent
void increase_capacity_if_needed (HashTable *table) {

    float load_factor = table->total_elements/table->capacity;
    if (load_factor >= HASH_TABLE_LOAD_FACTOR_THRESHOLD) { // increase capacity when load exceeds threshhold
        
        int old_capacity = table->capacity;
        HashTableElement *old_elements = table->elements;
        
        table->capacity = next_prime_number(old_capacity*2 + 1); // new capacity is the next prime number larger than 2x the current capacity
        table->elements = (HashTableElement *) malloc(sizeof(HashTableElement) * table->capacity);
        memset(table->elements, 0, sizeof(HashTableElement) * table->capacity);
        
        table->total_elements = 0;
        
        
        // following block puts all elements from the old element array into the new one
        for (int i = 0;i < old_capacity; i++) {
            HashTableElement *element = old_elements+i;
            for (int j = 0;j < element->item_count;j++) {
                hash_table_set_value(table, element->values[j], element->valuelens[j], element->keys[j], element->keylens[j]);
            }
            free_element(element, element->item_count);
        }
        free(old_elements); // free the old elements
    }
    
}
// get the element 
HashTableElement *get_element (HashTable *table, char *key, int keylen) {
    HashKeyType hash = table->hash_function(key, keylen);
    hash %= table->capacity;
    
    HashTableElement *element = table->elements + (int)hash;
    return element;
}
int index_for_key_in_element (HashTableElement *element, char *key, int keylen) {
    for (int i = 0;i < element->item_count;i++) {
        if (element->keylens[i] == keylen && memcmp(element->keys[i], key, keylen) == 0) {
            return i;
        }
        
    }
    return -1;
}
void append_value_in_element (HashTableElement *element, char *value, int valuelen, char *key, int keylen) {
    
    element->item_count += 1;
    
    if (valuelen > 0) {
        char *new_value = malloc(sizeof(char) * valuelen);
        memcpy(new_value, value, sizeof(char) * valuelen);
        value = new_value;
    }
    
    char *keycopy = (char *) malloc(sizeof(char) * keylen);
    memcpy(keycopy, key, sizeof(char) * keylen);
    
    if (element->values == NULL) {
        element->values = malloc(sizeof(char*) * element->item_count);
        element->valuelens = malloc(sizeof(int) * element->item_count);
        element->keys = malloc(sizeof(char*) * element->item_count);
        element->keylens = malloc(sizeof(int) * element->item_count);
    } else {
        element->values = realloc(element->values, sizeof(char*) * element->item_count);
        element->valuelens = realloc(element->valuelens, sizeof(int) * element->item_count);
        element->keys = realloc(element->keys, sizeof(char*) * element->item_count);
        element->keylens = realloc(element->keylens, sizeof(int) * element->item_count);
    }
    
    element->valuelens[element->item_count - 1] = valuelen;
    element->values[element->item_count - 1] = value;
    
    element->keylens[element->item_count - 1] = keylen;
    element->keys[element->item_count - 1] = keycopy;
}
void hash_table_set_value (HashTable *table, char *value, int valuelen, char *key, int keylen) {
    
    increase_capacity_if_needed(table);
    
    HashTableElement *element = get_element(table, key, keylen);
    
    if (value == NULL) { // In case the value is NULL, which means the value for the key has to be removed
        if (element->item_count == 1) {
            element->item_count -= 1;
            table->total_elements -= 1;
            free_element(element, 1);
        } else if (element->item_count > 1) {
            int index = index_for_key_in_element(element, key, keylen);
            
            if (index >= 0) {
                
                free(element->keys[index]);
                if (element->valuelens[index] > 0) {
                    free(element->values[index]);
                }
                
                for (int i = index+1; i < element->item_count;i++) {
                    element->values[i-1] = element->values[i];
                    element->valuelens[i-1] = element->valuelens[i];
                    element->keys[i-1] = element->keys[i];
                    element->keylens[i-1] = element->keylens[i];
                }
                
                element->item_count -= 1;
                table->total_elements -= 1;
                
                element->values = realloc(element->values, sizeof(char*) * element->item_count);
                element->valuelens = realloc(element->valuelens, sizeof(int) * element->item_count);
                element->keys = realloc(element->keys, sizeof(char*) * element->item_count);
                element->keylens = realloc(element->keylens, sizeof(int) * element->item_count);
            }
            
        }
        
    } else if (element->item_count == 0) { // in case there is nothing present at the element
        table->total_elements += 1;
        append_value_in_element(element, value, valuelen, key, keylen);
    } else { // in case there are values already present
        
        int index = index_for_key_in_element(element, key, keylen);
        if (index < 0) { // in case the value key pair is not present, which means there is a clash
            table->total_elements += 1;
            append_value_in_element(element, value, valuelen, key, keylen);
        } else { // in case the key is already present and one wants to change the value
            if (valuelen > 0) {
                char *new_value = malloc(sizeof(char) * valuelen);
                memcpy(new_value, value, sizeof(char) * valuelen);
                value = new_value;
            }
            element->values[index] = value;
            element->valuelens[index] = valuelen;
        }
        
    }
    
}
char *hash_table_get_value (HashTable *table, char *key, int keylen, int *valuelen) {
    HashTableElement *element = get_element(table, key, keylen);
    
    if (element->item_count <= 0) {
        valuelen = NULL;
        return NULL;
    }
    
    if (!HASH_TABLE_ALWAYS_CHECK_KEY_MATCH) {
        if (element->item_count == 1) {
            if (valuelen != NULL) {
                *valuelen = element->valuelens[0];
            }
            
            return element->values[0];
        }
    }
    
    int index = index_for_key_in_element(element, key, keylen);
    if (index < 0) {
        valuelen = NULL;
        return NULL;
    }
    if (valuelen != NULL) {
        *valuelen = element->valuelens[index];
    }
    return element->values[index];
}
