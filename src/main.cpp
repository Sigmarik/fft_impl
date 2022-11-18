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

#pragma GCC diagnostic ignored "-Wstack-usage="

int main(const int argc, const char** argv) {
    SILENCE_UNUSED(CMP_EPSILON);

    atexit(log_end_program);

    start_local_tracking();

    unsigned int log_threshold = STATUS_REPORTS;
    void* log_threshold_wrapper[] = { &log_threshold };

    ActionTag line_tags[] = {
        #include "cmd_flags/main_flags.h"
    };
    const int number_of_tags = sizeof(line_tags) / sizeof(*line_tags);

    parse_args(argc, argv, number_of_tags, line_tags);
    log_init("program_log.html", log_threshold, &errno);
    print_label();

    say("Here we go!");

    poly_arg_t poly_alpha[MAX_POLYNOMIAL_SIZE] = {};
    poly_arg_t poly_beta [MAX_POLYNOMIAL_SIZE] = {};
    memset(poly_alpha, 0, sizeof(poly_alpha));
    memset(poly_beta,  0, sizeof(poly_beta));

    say("What is the degree of the first polynomial?");
    printf("Enter degree of the first polynomial:\n");
    size_t size_a = (size_t) floor(read_number());
    gentle_clamp(size_a, MAX_POLYNOMIAL_SIZE / 2 - 1);
    size_a += 1;
    read_poly(poly_alpha, size_a);

    say("What is the degree of the second polynomial?");
    printf("Enter degree of the second polynomial:\n");
    size_t size_b = (size_t) floor(read_number());
    gentle_clamp(size_b, MAX_POLYNOMIAL_SIZE / 2 - 1);
    size_b += 1;
    read_poly(poly_beta, size_b);

    fft(poly_alpha, poly_alpha + MAX_POLYNOMIAL_SIZE, false, NULL);

    printf("The first polynomial is\n\\[P_1(x) = ");
    print_poly(poly_alpha, sizeof(poly_alpha) / sizeof(*poly_alpha));
    printf("\\]\n");

    // printf("The second polynomial is\n\\[P_2(x) = ");
    // print_poly(poly_beta, sizeof(poly_beta) / sizeof(*poly_beta));
    // printf("\\]\n");

    // say("These are the polynomials I will try to multiply.");

    // poly_multiply(poly_alpha, poly_beta, MAX_POLYNOMIAL_SIZE);

    // printf("Double-inverse second polynomial is\n\\[P_2(x) = ");
    // print_poly(poly_beta, sizeof(poly_beta) / sizeof(*poly_beta));
    // printf("\\]\n");

    // printf("The result of the multiplication of two given polynomials is\n\\[P_1(x) \\cdot P_2(x) = P(x) = ");
    // print_poly(poly_alpha, sizeof(poly_alpha) / sizeof(*poly_alpha));
    // printf("\\]\n");
    // say("Here is the result of the multiplication.");

    return_clean(errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
