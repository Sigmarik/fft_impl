/**
 * @file fft.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief FFT-based math module.
 * @version 0.1
 * @date 2022-11-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef FFT_H
#define FFT_H

#include <stdlib.h>

struct Vec2D {
    double x = 0;
    double y = 0;

    Vec2D operator*(Vec2D beta) { return {x * beta.x - y * beta.y, x * beta.y + y * beta.x}; }
    Vec2D operator+(Vec2D beta) { return {x + beta.x, y + beta.y}; }
    Vec2D operator-(Vec2D beta) { return {x - beta.x, y - beta.y}; }
    void operator/=(double dbl) { x /= dbl; y /= dbl; }
};

typedef Vec2D poly_arg_t;

/**
 * @brief Compare two doubles and tell if they are equal.
 * 
 * @param alpha first double
 * @param beta second double
 * @return bool if values are within the set delta radius from each other
 */
bool equal(double alpha, double beta);

/**
 * @brief Fast fourier transform.
 * 
 * @param begin pointer to the start of the coefficient array
 * @param end pointer to the end of coefficient array
 * @param inverse (useless)
 * @param swap_buffer 
 */
void fft(poly_arg_t* begin, poly_arg_t* end, bool inverse = false, poly_arg_t* swap_buffer = NULL);

/**
 * @brief Multiply two polynomials. The first one is getting changed.
 * 
 * @param alpha first polynomial
 * @param beta second polynomial
 * @param size size of two arrays (must be the power of 2)
 */
void poly_multiply(poly_arg_t* alpha, poly_arg_t* beta, size_t size);

#endif