//
//  hash_function.h
//  CMD
//
//  Created by Adhiraj Singh on 12/3/19.
//  Copyright © 2019 Adhiraj Singh. All rights reserved.
//

#ifndef hash_function_h
#define hash_function_h

#include <stdio.h>

typedef uint64_t HashKeyType;
typedef HashKeyType (*HashingFunction)(char *data, int len);

HashKeyType hash_function_default (char *data, int len);

HashKeyType hash_function_java_string (char *data, int len);
HashKeyType hash_function_singh (char *data, int len);
HashKeyType hash_function_jenkins (char *data, int len);
HashKeyType hash_function_FNV1A (char* data, int len);
uint64_t hash_function_murmur64A(char* data, int len);
HashKeyType hash_function_CRC32 (char *data, int len);

///Executes a hash function on data of size N and then on data of size 2N and returns the ratio of time taken
float hash_function_compute_complexity (HashingFunction hash_function);
float hash_function_variance (HashingFunction hash_function, int number_of_unique_items, int array_size);
void hash_function_chi_square_test (HashingFunction hash_function);

#endif /* hash_function_h */
