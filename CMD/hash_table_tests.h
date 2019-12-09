//
//  hash_table_tests.h
//  CMD
//
//  Created by Adhiraj Singh on 12/2/19.
//  Copyright © 2019 Adhiraj Singh. All rights reserved.
//

#ifndef hash_table_tests_h
#define hash_table_tests_h

#include "hash_table.h"
#include <assert.h>

void test_store_value_in_table () {
    char key[20] = "Hello my name jeff";
    int value = 500;
    
    HashTable *table = hash_table_new();
    hash_table_set_value(table, (char *)&value, 0, key, 20); // store the pointer to the value
    int *p = (int *)hash_table_get_value(table, key, 20, NULL);
    
    assert(p == &value);
    assert(*p == value);

    int value2 = 400;
    hash_table_set_value(table, (char *)&value2, sizeof(int), key, 20); // copy and store value2
    
    int size = 0;
    p = (int *)hash_table_get_value(table, key, 20, &size);
    
    assert(size == sizeof(int));
    assert(*p == value2);
}
void test_remove_value_in_table () {
    char key1[20] = "Hello my name jeff";
    char key2[20] = "hello my name jeff";
    int value1 = 500;
    int value2 = 300;
    
    HashTable *table = hash_table_new();
    
    hash_table_set_value(table, (char *)&value1, 0, key1, 20);
    hash_table_set_value(table, (char *)&value2, sizeof(int), key2, 20);
    
    assert(table->total_elements == 2);
    
    hash_table_set_value(table, NULL, 0, key1, 20);
    
    assert(table->total_elements == 1);
    
    int *rvalue1 = (int *)hash_table_get_value(table, key1, 20, NULL);
    
    assert(rvalue1 == NULL);

    int *rvalue2 = (int *)hash_table_get_value(table, key2, 20, NULL);
    
    assert(*rvalue2 == value2);

    hash_table_set_value(table, NULL, 0, key2, 20);
    
    rvalue2 = (int *)hash_table_get_value(table, key2, 20, NULL);
    
    assert(rvalue2 == NULL);
    
    assert(table->total_elements == 0);
}
void test_clash_value_in_table () {
    char key1[20] = "Hello my name jeff";
    char key2[20] = "hello my name jeff";
    int value1 = 500;
    float value2 = 300.0;
    
    HashTable *table = hash_table_new();
    
    hash_table_set_value(table, (char *)&value1, 0, key1, 20);
    hash_table_set_value(table, (char *)&value2, sizeof(float), key2, 20);
    
    int *rvalue1 = (int *)hash_table_get_value(table, key1, 20, NULL);
    float *rvalue2 = (float *)hash_table_get_value(table, key2, 20, NULL);
    
    assert(rvalue1 == &value1);
    assert(*rvalue1 == value1);
    
    assert(*rvalue2 == value2);
    
    assert(table->total_elements == 2);
}
void test_lots_of_elements () {
    
    HashTable *table = hash_table_new();
    
    for (int i = 0; i < 500000;i++) {
        uint64_t key = (i + 47)*3;
        int value = i;
        
        hash_table_set_value(table, (char *)&value, 0, (char *)&key, sizeof(uint64_t));
        int *rvalue = (int *)hash_table_get_value(table, (char *)&key, sizeof(uint64_t), NULL);
        
        assert(rvalue == &value);
        assert(*rvalue == value);
    }
    assert(table->total_elements == 500000);
    printf("total elements: %d\n", table->total_elements);
    
    for (int i = 0; i < 500000;i++) {
        uint64_t key = (i + 47)*3;
        
        hash_table_set_value(table, NULL, 0, (char *)&key, sizeof(uint64_t));
        int *rvalue = (int *)hash_table_get_value(table, (char *)&key, sizeof(uint64_t), NULL);
        
        assert(rvalue == NULL);
    }
    assert(table->total_elements == 0);
    printf("total elements: %d\n", table->total_elements);
}

void run_all_tests () {
    test_store_value_in_table();
    test_remove_value_in_table();
    test_clash_value_in_table();
    test_lots_of_elements();
}

#endif /* hash_table_tests_h */
