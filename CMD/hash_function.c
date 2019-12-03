//
//  hash_function.c
//  CMD
//
//  Created by Adhiraj Singh on 12/3/19.
//  Copyright © 2019 Adhiraj Singh. All rights reserved.
//

#include "hash_function.h"
#include <sys/time.h>
#include <math.h>
#include "chisqr.h"
#include <string.h>
#include <stdlib.h>

HashKeyType hash_function_default (char *data, int len) {
    return hash_function_singh(data, len);
}

///Implementation of the java string hashing function
HashKeyType hash_function_DJB2 (char *data, int len) {
    HashKeyType hash = 5381;
    for (int i = 0; i < len;i++) {
        hash += (hash<<5)-hash + data[i];
    }
    return hash;
}
uint8_t rotl (uint8_t n, unsigned int c)
{
    uint8_t lsbs = n & ((1 << c) - 1);
    return (n >> c) | (lsbs << (8-c));
}
HashKeyType hash_function_singh (char *data, int len) {
    
   /* const int arr_size_len = 81;
    const HashKeyType arr_sizes[] = {
        2,3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173
        , 179, 181, 191, 193, 197, 199, 211, 223, 227, 229
        , 233, 239, 241, 251, 257, 263, 269, 271, 277, 281
        , 283, 293, 307, 311, 313, 317, 331, 337, 347, 349
        , 353, 359, 367, 373, 379, 383, 389, 397, 401, 409
        , 419, 421
    };*/
    
    
    HashKeyType hash = 11;
    HashKeyType prime1 = 31;
    HashKeyType prime2 = 311;
    
    for (int i = 0; i < len;i++) {
        
        hash = (hash * prime1) + rotl((data[i] ^ prime2) ^ (len-i), 4);
        hash = hash * prime2;
    }
    hash += (hash << 3);
    return hash;
}

HashKeyType hash_function_jenkins (char *data, int len) {

    HashKeyType hash = 0;
    for(int i = 0; i < len; ++i)
    {
        hash += data[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}
uint64_t hash_function_murmur64A(char* data, int len)
{
    const uint64_t seed = 0xf7b5e8035be1e999;
    const uint64_t m = 0xc6a4a7935bd1e997;
    const int r = 37;

    uint64_t h = seed ^ (len * m);

    const uint64_t * ptr = (const uint64_t *)data;
    const uint64_t * end = ptr + (len/8);

    while(ptr != end)
    {
      uint64_t k = *ptr++;

      k *= m;
      k ^= k >> r;
      k *= m;

      h ^= k;
      h *= m;
    }

    const unsigned char * ptr2 = (const unsigned char*)ptr;

    switch(len & 7) {
      case 7: h ^= (uint64_t)((uint64_t)ptr2[6] << (uint64_t)48);
      case 6: h ^= (uint64_t)((uint64_t)ptr2[5] << (uint64_t)40);
      case 5: h ^= (uint64_t)((uint64_t)ptr2[4] << (uint64_t)32);
      case 4: h ^= (uint64_t)((uint64_t)ptr2[3] << (uint64_t)24);
      case 3: h ^= (uint64_t)((uint64_t)ptr2[2] << (uint64_t)16);
      case 2: h ^= (uint64_t)((uint64_t)ptr2[1] << (uint64_t)8 );
      case 1: h ^= (uint64_t)((uint64_t)ptr2[0]                );
              h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}
HashKeyType hash_function_FNV1A (char* data, int len) {
    
    HashKeyType hash = 0xcbf29ce484222325;
    for (int i = 0; i < len;i++) {
        hash = (hash^((HashKeyType)data[i]));
        hash = hash + (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 40);
    }
    return hash;
    
}
HashKeyType hash_function_CRC32 (char *data, int len) {
    uint64_t crc, mask;

    crc = 0xFFFFFFFFFFFFF;
    for (int i = 0; i < len;i++) {
        crc = crc ^ data[i];
        mask = -(crc & 1);
        crc = (crc >> 1) ^ (0xEDB88320342 & mask);
    }
    return ~crc;
}
float hash_function_compute_time (HashingFunction hash_function, int num_iterations) {
    struct timespec start, end;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    
    for (int i = 0; i < num_iterations;i++) {
        uint64_t key = (i + 47)*3;
        hash_function((char *) &key, sizeof(uint32_t));
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t n = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;
    
    return (float)n/1000000;
}
float hash_function_compute_complexity_ratio (HashingFunction hash_function) {
    
    const int num_iterations = 500000;
    
    float t = hash_function_compute_time(hash_function, num_iterations);
    float t2 = hash_function_compute_time(hash_function, num_iterations*2);
    float ratio = (float)t2/(float)t;
    return ratio;
}
float hash_function_variance (HashingFunction hash_function, int number_of_unique_items, int array_size, int distribution_type) {

    const int item_len = 64;
    char items[number_of_unique_items][item_len];
    
    switch (distribution_type) {
        case HASH_DIST_INCREMENTAL:
            for (int i = 0; i < number_of_unique_items;i++) {
                uint32_t *ptr = (uint32_t *) items[i];
                const int divs = item_len/sizeof(uint32_t);
                for (int j = 0; j < divs;j++) {
                    ptr[j] = (i*number_of_unique_items + j);
                }
            }
            break;
        case HASH_DIST_FULLRANDOM:
            
            for (int i = 0; i < number_of_unique_items;i++) {
                for (int j = 0; j < item_len;j++) {
                    items[i][j] = rand() % 256;
                }
            }
            
            break;
        case HASH_DIST_ALPHANUMERIC:
            for (int i = 0; i < number_of_unique_items;i++) {
                for (int j = 0; j < item_len;j++) {
                    items[i][j] = rand() % ('~'-'A') + 'A';
                }
            }
            break;
        default:
            break;
    }
    
    
    
    float items_per_slot[array_size];
    memset(items_per_slot, 0, sizeof(items_per_slot[0])*array_size);
    
    for (int j = 0; j < number_of_unique_items; j ++) {
        HashKeyType hash = hash_function( (char *)&items[j], item_len );
        items_per_slot[ hash % array_size ] += 1;
    }
    
    float variance = 0.0f;
    float expectation = (float)number_of_unique_items/array_size;
    
    for (int j = 0; j < array_size;j++) {
        variance += (items_per_slot[j]-expectation)*(items_per_slot[j]-expectation)/expectation;
    }
    
    return variance;
}
float hash_function_chi_square_test (HashingFunction hash_function, int distribution_type) {
    
    const int arr_size_len = 406;
    const int arr_sizes[] = {
        11, 13, 17, 19, 23, 29
        , 31, 37, 41, 43, 47, 53, 59, 61, 67, 71
        , 73, 79, 83, 89, 97, 101, 103, 107, 109, 113
        , 127, 131, 137, 139, 149, 151, 157, 163, 167, 173
        , 179, 181, 191, 193, 197, 199, 211, 223, 227, 229
        , 233, 239, 241, 251, 257, 263, 269, 271, 277, 281
        , 283, 293, 307, 311, 313, 317, 331, 337, 347, 349
        , 353, 359, 367, 373, 379, 383, 389, 397, 401, 409
        , 419, 421, 431, 433, 439, 443, 449, 457, 461, 463
        , 467, 479, 487, 491, 499, 503, 509, 521, 523, 541
        , 547, 557, 563, 569, 571, 577, 587, 593, 599, 601
        , 607, 613, 617, 619, 631, 641, 643, 647, 653, 659
        , 661, 673, 677, 683, 691, 701, 709, 719, 727, 733
        , 739, 743, 751, 757, 761, 769, 773, 787, 797, 809
        , 811, 821, 823, 827, 829, 839, 853, 857, 859, 863
        , 877, 881, 883, 887, 907, 911, 919, 929, 937, 941
        , 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013
        , 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069
        , 1087, 1091, 1093, 1097, 1103, 1109, 1117, 1123, 1129, 1151
        , 1153, 1163, 1171, 1181, 1187, 1193, 1201, 1213, 1217, 1223
        , 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291
        , 1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373
        , 1381, 1399, 1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451
        , 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499, 1511
        , 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583
        , 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657
        , 1663, 1667, 1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733
        , 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789, 1801, 1811
        , 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889
        , 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987
        , 1993, 1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053
        , 2063, 2069, 2081, 2083, 2087, 2089, 2099, 2111, 2113, 2129
        , 2131, 2137, 2141, 2143, 2153, 2161, 2179, 2203, 2207, 2213
        , 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287
        , 2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357
        , 2371, 2377, 2381, 2383, 2389, 2393, 2399, 2411, 2417, 2423
        , 2437, 2441, 2447, 2459, 2467, 2473, 2477, 2503, 2521, 2531
        , 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609, 2617
        , 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687
        , 2689, 2693, 2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741
        , 2749, 2753, 2767, 2777, 2789, 2791, 2797, 2801, 2803, 2819
    };
    const int num_items = 5000;
    
    float overall_variance = 0.0f;
    float overall_confidence = 0.0f;
    for (int i = 0; i < arr_size_len; i++) {
        int arr_size = arr_sizes[i];
        float expectation = (float)num_items/arr_size;
        
        float variance = hash_function_variance(hash_function, num_items, arr_size, distribution_type);
        double chisq = chisqr(arr_size, variance);

       // printf("arr_size = %d E = %f, V = %f, confidence = %f\n", arr_size, expectation, variance, chisq);
        overall_variance += (variance-1)*(variance-1);
        overall_confidence += chisq;
    }
    overall_confidence /= arr_size_len;
    printf("overall variance = %f, chisq = %f\n", overall_variance, overall_confidence);
    return overall_variance;
}
