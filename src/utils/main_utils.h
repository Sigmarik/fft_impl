/**
 * @file common.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Utility features useful in all programs.
 * @version 0.1
 * @date 2022-10-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <cstring>
#include <ctype.h>

#include "config.h"
#include "lib/alloc_tracker/alloc_tracker.h"

#include "lib/fft.h"

enum Mode {
    MODE_POLY,
    MODE_NUM,
};

/**
 * @brief Array with stored size.
 * 
 */
struct MemorySegment {
    int* content = NULL;
    size_t size = 1024;
};

void MemorySegment_ctor(MemorySegment* segment);
void MemorySegment_dtor(MemorySegment* segment);

#define MemorySegment_dump(segment, importance) do { \
    log_printf(importance, "dump", "Memory segment at %p. Size: %ld Content: %p\n", segment, (segment)->size, (segment)->content); \
    _MemorySegment_dump(segment, importance); \
} while (0)

void _MemorySegment_dump(MemorySegment* segment, unsigned int importance);

/**
 * @brief Limit specified value between left and right border.
 * 
 * @param value variable to check
 * @param left left border
 * @param right right border
 * @return 
 */
int clamp(const int value, const int left, const int right);

/**
 * @brief Print a bunch of owls.
 * 
 * @param argc unimportant
 * @param argv unimportant
 * @param argument unimportant
 */
void print_owl(const int argc, void** argv, const char* argument);

/**
 * @brief Disable all voice lines.
 * 
 * @param argc unimportant
 * @param argv unimportant
 * @param argument unimportant
 */
void mute_speaker(const int argc, void** argv, const char* argument);

/**
 * @brief Print program label and build date/time to console and log.
 * 
 */
void print_label();

/**
 * @brief Read single number from stdin.
 * 
 */
double read_number();

/**
 * @brief Read polynomial coefficients from the user.
 * 
 * @param polynomial polynomial to read
 * @param size size of the polynomial
 */
void read_poly(poly_arg_t* polynomial, size_t size);

/**
 * @brief Print the polynomial in the form of kX^n + kX^(n-1) + ... + kX + C
 * 
 * @param polynomial polynomial to print
 * @param size size of the polynomial (degree will be determined automatically)
 */
void print_poly(const poly_arg_t* polynomial, size_t size);

/**
 * @brief Shift all polynomial coefficients so it would become an integer representation.
 * 
 * @param poly polynomial to broom
 * @param size size of the polynomial (not the power!)
 */
void broom_up(poly_arg_t* poly, size_t size);

/**
 * @brief Read VERY big integer and compact it into a polynomial.
 * 
 * @param poly polynomial to fill with the number
 * @param size size of the polynomial
 */
void read_poly_as_big(poly_arg_t* poly, size_t size);

/**
 * @brief Print polynomial in the format of big integer.
 * 
 * @param poly polynomial to print
 * @param size size of the polynomial
 */
void print_poly_as_big(const poly_arg_t* poly, size_t size);

/**
 * @brief Get degree of the polynomial
 * 
 * @param poly polynomial
 * @param size size of the polynomial
 * @return degree of the polynomial
 */
size_t poly_degree(const poly_arg_t* poly, size_t size);

/**
 * @brief Get size of the array.
 * 
 * @param array
 */
#define arr_size(array) ( sizeof(array) / sizeof(*array) )

/**
 * @brief Clamp the value by the upper limit and notify the user if the value was modified.
 * 
 * @param value value to clamp
 * @param border maximal value the variable is allowed to be
 */
#define gentle_clamp(value, border) do {                                                                        \
    if (( value ) > ( border )) {                                                                               \
        log_printf(WARNINGS, "warning", "Size of the " #value " (%lld) was bigger than " #border " (%lld).\n",  \
                                        (long long) ( value ), (long long) ( border ));                         \
        value = border;                                                                                         \
        printf("Given value was too big. It was reset to %lld.\n", (long long)( value ));                       \
    }                                                                                                           \
} while (0)

/**
 * @brief Read user input and do actions depending on if user entered yes or no.
 * 
 * @param action_yes code to execute on YES
 * @param action_no code to execute on NO
 */
#define yn_branch(action_yes, action_no) do {                               \
    char __answer = '\0';                                                   \
    scanf(" %c", &__answer);                                                \
    __answer = (char)tolower(__answer);                                     \
                                                                            \
    while (getc(stdin) != '\n');                                            \
                                                                            \
    if (__answer == 'y') {                                                  \
        log_printf(STATUS_REPORTS, "status", "User answered with YES.\n");  \
        action_yes;                                                         \
        break;                                                              \
    }                                                                       \
    if (__answer == 'n') {                                                  \
        log_printf(STATUS_REPORTS, "status", "User answered with NO.\n");   \
        action_no;                                                          \
        break;                                                              \
    }                                                                       \
    printf("yes/no expected, try again.\n>>> ");                            \
} while(true)

#endif