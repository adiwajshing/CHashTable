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
    float r = hash_function_compute_complexity(hash_function_java_string);
    printf("ratio %f\n", r);
    hash_function_chi_square_test(hash_function_singh);
    
   // run_all_tests();
    return 0;
}
