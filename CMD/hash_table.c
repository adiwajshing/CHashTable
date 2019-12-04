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
    table->total_elements = 0;
    table->hash_function = hash_function_default;
    memset(table->elements, 0, sizeof(HashTableElement) * table->capacity);
    
    return table;
}

void free_element (HashTableElement *element, int item_count) {
    for (int i = 0; i < item_count;i++) {
        free(*(element->keys+i));
    }
    free(element->data_pointer);
    free(element->keys);
    free(element->keylens);
    
    element->data_pointer = NULL;
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
        num += 1;
    }
    
}
void increase_capacity_if_needed (HashTable *table) {

    float load_factor = table->total_elements/table->capacity;
    if (load_factor >= HASH_TABLE_LOAD_FACTOR_THRESHOLD) {
        
        int old_capacity = table->capacity;
        HashTableElement *old_elements = table->elements;
        
        table->capacity = next_prime_number(old_capacity*2 + 1);
        //printf("np: %d\n", table->capacity);
        table->elements = (HashTableElement *) malloc(sizeof(HashTableElement) * table->capacity);
        memset(table->elements, 0, sizeof(HashTableElement) * table->capacity);
        
        table->total_elements = 0;
        
        for (int i = 0;i < old_capacity; i++) {
            HashTableElement *element = old_elements+i;
            for (int j = 0;j < element->item_count;j++) {
                hash_table_set_value(table, element->data_pointer[j], element->keys[j], element->keylens[j]);
            }
            free_element(element, element->item_count);
        }
        free(old_elements);
    }
    
}

HashTableElement *get_element (HashTable *table, char *key, int keylen) {
    HashKeyType hash = table->hash_function(key, keylen);
    hash %= table->capacity;
    
    HashTableElement *element = table->elements + (int)hash;
    //printf("index = %ud", hash);
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
void append_value_in_element (HashTableElement *element, char *value, char *key, int keylen) {
    
    element->item_count += 1;
    
    char **pointers = malloc(sizeof(char*) * element->item_count);
    char **keys = malloc(sizeof(char*) * element->item_count);
    int *keylens = malloc(sizeof(int) * element->item_count);
    
    if (element->data_pointer != NULL) {
        
        for (int i = 0; i < element->item_count-1;i++) {
            pointers[i] = element->data_pointer[i];
            keys[i] = element->keys[i];
            keylens[i] = element->keylens[i];
        }
        
        free(element->data_pointer);
        free(element->keys);
        free(element->keylens);
    }
    
    pointers[element->item_count - 1] = value;
    keylens[element->item_count - 1] = keylen;
    
    char *keycopy = (char *) malloc(sizeof(char) * keylen);
    memcpy(keycopy, key, sizeof(char) * keylen);
    
    keys[element->item_count - 1] = keycopy;
    
    
    element->data_pointer = pointers;
    element->keys = keys;
    element->keylens = keylens;
}
void hash_table_set_value (HashTable *table, char *value, char *key, int keylen) {
    
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
                
                element->item_count -= 1;
                table->total_elements -= 1;
                
                char **pointers = malloc(sizeof(char*) * element->item_count);
                char **keys = malloc(sizeof(char*) * element->item_count);
                int *keylens = malloc(sizeof(int) * element->item_count);
                
                int j = 0;
                for (int i = 0; i < element->item_count+1;i++) {
                    if (i != index) {
                        pointers[j] = element->data_pointer[i];
                        keys[j] = element->keys[i];
                        keylens[j] = element->keylens[i];
                        j += 1;
                    }
                }
                
                free(element->keys[index]);
                
                free(element->data_pointer);
                free(element->keys);
                free(element->keylens);
                
                element->data_pointer = pointers;
                element->keys = keys;
                element->keylens = keylens;
            }
            
        }
        
    } else if (element->item_count == 0) { // in case there is nothing present at the element
        table->total_elements += 1;
        append_value_in_element(element, value, key, keylen);
    } else { // in case there are values already present
        
        int index = index_for_key_in_element(element, key, keylen);
        if (index < 0) { // in case the value key pair is not present, which means there is a clash
            table->total_elements += 1;
            append_value_in_element(element, value, key, keylen);
        } else { // in case the key is already present and one wants to change the value
            element->data_pointer[index] = value;
        }
        
    }
    
}
char *hash_table_get_value (HashTable *table, char *key, int keylen) {
    HashTableElement *element = get_element(table, key, keylen);
    
    if (element->item_count <= 0) {
        return NULL;
    }
    
    if (!HASH_TABLE_ALWAYS_CHECK_KEY_MATCH) {
        if (element->item_count == 1) {
            return element->data_pointer[0];
        }
    }
    
    int index = index_for_key_in_element(element, key, keylen);
    if (index < 0) {
        return NULL;
    }
    
    return element->data_pointer[index];
}
