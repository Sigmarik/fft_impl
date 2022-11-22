/**
 * @file main.c
 * @author Ilya Kudryashov (kudriashov.it@phystech.edu)
 * @brief Equation differentiator.
 * @version 0.1
 * @date 2022-08-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>
#include <math.h>

#include "lib/util/dbg/debug.h"
#include "lib/util/argparser.h"
#include "lib/alloc_tracker/alloc_tracker.h"
#include "lib/speaker.h"

#include "utils/config.h"

#include "utils/main_utils.h"

#define MAIN

int main(const int argc, const char** argv) {

    atexit(log_end_program);

    start_local_tracking();

    unsigned int log_threshold = STATUS_REPORTS;
    void* log_threshold_wrapper[] = { &log_threshold };

    Mode mode = MODE_POLY;
    void* mode_wrapper[] = { &mode };

    ActionTag line_tags[] = {
        #include "cmd_flags/main_flags.h"
    };
    const int number_of_tags = sizeof(line_tags) / sizeof(*line_tags);

    parse_args(argc, argv, number_of_tags, line_tags);
    log_init("program_log.html", log_threshold, &errno);
    print_label();

    const char* op_object = ( mode == MODE_POLY ? "polynomial" : "number" );

    say("Here we go!");

    static poly_arg_t poly_alpha[MAX_POLYNOMIAL_SIZE] = {};
    static poly_arg_t poly_beta [MAX_POLYNOMIAL_SIZE] = {};
    memset(poly_alpha, 0, sizeof(poly_alpha));
    memset(poly_beta,  0, sizeof(poly_beta));

    log_printf(STATUS_REPORTS, "status", "Reading data.\n");

    say("Enter the first %s.", op_object);
    if (mode == MODE_POLY)
        read_poly(poly_alpha, arr_size(poly_alpha) / 2);
    else
        read_poly_as_big(poly_alpha, arr_size(poly_alpha) / 2);

    say("Enter the second %s.", op_object);
    if (mode == MODE_POLY)
        read_poly(poly_beta, arr_size(poly_beta) / 2);
    else
        read_poly_as_big(poly_beta, arr_size(poly_beta) / 2);

    printf("The first %s:\n", op_object);
    if (mode == MODE_POLY) {
        printf("\\[ P_1(x) = ");
        print_poly(poly_alpha, arr_size(poly_alpha));
        printf(" \\]\n");
    } else {
        printf("0x");
        print_poly_as_big(poly_alpha, arr_size(poly_alpha));
        printf("\n");
    }

    printf("The second %s:\n", op_object);
    if (mode == MODE_POLY) {
        printf("\\[ P_2(x) = ");
        print_poly(poly_beta, arr_size(poly_beta));
        printf(" \\]\n");
    } else {
        printf("0x");
        print_poly_as_big(poly_beta, arr_size(poly_beta));
        printf("\n");
    }

    say("These are the %ss I will try to multiply.", op_object);

    log_printf(STATUS_REPORTS, "status", "Multiplying...\n");

    poly_multiply(poly_alpha, poly_beta, MAX_POLYNOMIAL_SIZE);

    log_printf(STATUS_REPORTS, "status", "Multiplication result:\n");
    for (size_t id = 0; id < arr_size(poly_alpha); ++id) {
        _log_printf(STATUS_REPORTS, "status", "(%lg, %lg)\n", poly_alpha[id].x, poly_beta[id].y);
    }

    printf("The result of the multiplication of two given %ss is\n", op_object);
    if (mode == MODE_POLY) {
        printf("\\[ P_1(x) \\cdot P_2(x) = P(x) = ");
        print_poly(poly_alpha, arr_size(poly_alpha));
        printf(" \\]\n");
    } else {
        printf("0x");
        print_poly_as_big(poly_alpha, arr_size(poly_alpha));
        printf("\n");
    }
    say("Here is the result of the multiplication.");

    return_clean(errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
