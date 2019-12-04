//
//  hash_function.h
//  CMD
//
//  Created by Adhiraj Singh on 12/3/19.
//  Copyright © 2019 Adhiraj Singh. All rights reserved.
//

#ifndef hash_function_h
#define hash_function_h

#define HASH_DIST_ALPHANUMERIC 0
#define HASH_DIST_FULLRANDOM 1
#define HASH_DIST_INCREMENTAL 2

#include <stdio.h>

typedef uint64_t HashKeyType;
typedef HashKeyType (*HashingFunction)(char *data, int len);

HashKeyType hash_function_default (char *data, int len);

HashKeyType hash_function_DJB2 (char *data, int len);
HashKeyType hash_function_singh (char *data, int len);
HashKeyType hash_function_jenkins (char *data, int len);
HashKeyType hash_function_FNV1A (char* data, int len);
uint64_t hash_function_murmur64A(char* data, int len);
HashKeyType hash_function_CRC32 (char *data, int len);

///Returns the time taken to execute given iterations of the specified hash function in ms
uint32_t hash_function_compute_time (HashingFunction hash_function, int num_iterations);
///Executes a hash function on data of size N and then on data of size 2N and returns the ratio of time taken
float hash_function_compute_complexity_ratio (HashingFunction hash_function);
///Returns the variance of the hash function on the given array size
float hash_function_variance (HashingFunction hash_function, int number_of_unique_items, int array_size, int distribution_type);
float hash_function_chi_square_test (HashingFunction hash_function, int distribution_type);

float *hash_function_bit_distribution (HashingFunction hash_function, int mod, int distribution_type, int *len, float *variance);

#endif /* hash_function_h */
