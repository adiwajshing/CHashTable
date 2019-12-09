//
//  main.c
//  CMD
//
//  Created by Adhiraj Singh on 2/20/19.
//  Copyright © 2019 Adhiraj Singh. All rights reserved.
//

#include <stdio.h>
#include "hash_table_tests.h"
#include "hash_function.h"
#include "chisqr.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    //printf("interval = %f\n", chisqr(5, 1.61));
    //multiply(401, 52);
   /* float r = hash_function_compute_complexity_ratio(hash_function_singh);
    printf("ratio %f\n", r);
    
    int len;
    float bit_variance;
    hash_function_bit_distribution(hash_function_singh, 2039, HASH_DIST_ALPHANUMERIC, &len, &bit_variance);
    printf("bit variance = %f\n", bit_variance);
   
   // printf("chisq: %f\n", chisqr(10, 5));
    
    hash_function_chi_square_test(hash_function_singh, 0.05, HASH_DIST_ALPHANUMERIC);
    hash_function_chi_square_test(hash_function_singh, 0.05, HASH_DIST_FULLRANDOM);
    hash_function_chi_square_test(hash_function_singh, 0.05, HASH_DIST_INCREMENTAL);*/

    run_all_tests();
    return 0;
}
