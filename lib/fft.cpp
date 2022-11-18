#include "fft.h"

#include <stdint.h>
#include <string.h>
#include <math.h>

#include "lib/util/dbg/logger.h"
#include "lib/util/dbg/debug.h"
#include "lib/alloc_tracker/alloc_tracker.h"

static const double PI = 3.1415926535;
static double CMP_EPSILON = 1e-7;

bool equal(double alpha, double beta) {
    return abs(alpha - beta) < CMP_EPSILON;
}

/*
Even though the code should explain itself, this does not apply for FFT
algorithm as its complexity and ingenuity can only be lowered to "holy sh*t"
mark. So... the code below is overflowed with comments not without a reason, as
I want to use it as a guide for my tiny-little brain that will have no power to
revise FFT with standard formulas before the exam.
*/

//* RU - Root of the Unity - that little spike on the "even and nice" pointy-wheel representing easy-to-compute
//* all-the-same complex numbers.

void fft(poly_arg_t* begin, poly_arg_t* end, bool inverse, poly_arg_t* swap_buffer) {
    if (end <= begin + 1) return;
    //*                      ^-- As this is the equivalent of the constant, we don't need to do anything
    //*                          as, well, the constant gives us a constant value no matter which point
    //*                          do we calculate our polynomial at.

    size_t size = (size_t)(end - begin);

    //* Check if the size of the array is a power of 2.
    _LOG_FAIL_CHECK_(~size & (size - 1), "error", ERROR_REPORTS, return, &errno, EINVAL);

    bool create_buffer = false;

    //* Swapping can be done in-place, but... but this will not make the code more readable.
    if (!swap_buffer) {
        swap_buffer = (poly_arg_t*) calloc(size, sizeof(*swap_buffer));
        _LOG_FAIL_CHECK_(swap_buffer, "error", ERROR_REPORTS, return, &errno, ENOMEM);
        create_buffer = true;
    }

    poly_arg_t* buffer_mid = swap_buffer + size / 2;

    //* Split the polynomial into even and odd sections for easier value duplication.
    for (size_t id = 0; id * 2 < size; ++id) {
        swap_buffer[id] = begin[id * 2];
        buffer_mid [id] = begin[id * 2 + 1];
    }

    memcpy(begin, swap_buffer, size * sizeof(*begin));

    poly_arg_t* mid = begin + size / 2;

    fft(begin, mid, inverse, swap_buffer);
    fft(mid,   end, inverse, swap_buffer + size / 2);

    //* Then we evaluate our polynomial on half of the "RU circle", which gives as the second half automatically
    //* as ODD(x) = -ODD(-x), EVEN(x) = EVEN(-x), so we can "duplicate" each point to it's diametral counterpart
    //* and easily get poly's value there.
    for (size_t id = 0; id < size / 2; ++id) {
        double angle = (double)id * 2.0 * PI / (double)size;
        if (inverse) angle *= -1.0;
        //*            ^-- for not-explained-on-the-lecture reason inverse DFT transformation
        //*                matrix is almost the same as the direct one by the exception that
        //*                every coefficient is 1-inverse (raised to the power of -1), which
        //*                so happens to be our values rotated to the other side!

        Vec2D ru = { .x = cos(angle), .y = sin(angle) };
        //*    ^-- i-th RU for later duplication. It iterates through only half of the circle,
        //*        as we get the second half automatically.

        Vec2D value_a = begin[id];
        Vec2D value_b = mid[id];

        //* Here it is! Here we calculate the value at i-th RU and determine poly's value at diametrical counterpart!
        begin[id] = value_a + ru * value_b;
        mid[id] = value_a - ru * value_b;
    }

    if (create_buffer) free(swap_buffer);

    return;
}

void poly_multiply(poly_arg_t* alpha, poly_arg_t* beta, size_t size) {
    //*     Roses are red,
    //*     Violets are blue.
    //*     I am committing a war crime if this statement
    //*     Will not always be true.
    //*                 |
    //*                 |
    //*                 v
    _LOG_FAIL_CHECK_(~size & (size - 1), "error", ERROR_REPORTS, return, &errno, EINVAL);

    fft(alpha, alpha + size, false, NULL);
    fft(beta,  beta  + size, false, NULL);

    for (size_t id = 0; id < size; id++) {
        alpha[id] = alpha[id] * beta[id];
    }

    fft(alpha, alpha + size, true, NULL);
    fft(beta,  beta  + size, true, NULL);

    for (size_t id = 0; id < size; ++id) {
        alpha[id] /= (double)size;
        beta[id]  /= (double)size;
    }
}
