#include "main_utils.h"

#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "lib/util/dbg/logger.h"
#include "lib/util/dbg/debug.h"

#include "lib/speaker.h"

void MemorySegment_ctor(MemorySegment* segment) {
    segment->content = (int*) calloc(segment->size, sizeof(*segment->content));
}

void MemorySegment_dtor(MemorySegment* segment) {
    free(segment->content);
    segment->content = NULL;
    segment->size = 0;
}

void _MemorySegment_dump(MemorySegment* segment, unsigned int importance) {
    for (size_t id = 0; id < segment->size; ++id) {
        _log_printf(importance, "dump", "[%6lld] = %d\n", (long long) id, segment->content[id]);
    }
}

int clamp(const int value, const int left, const int right) {
    if (value < left) return left;
    if (value > right) return right;
    return value;
}

// Amazing, do not change anything!
// Completed the owl, sorry.
void print_owl(const int argc, void** argv, const char* argument) {
    SILENCE_UNUSED(argc); SILENCE_UNUSED(argv); SILENCE_UNUSED(argument);
    printf("-Owl argument detected, dropping emergency supply of owls.\n");
    for (int index = 0; index < NUMBER_OF_OWLS; index++) {
        puts(R"(    A_,,,_A    )");
        puts(R"(   ((O)V(O))   )");
        puts(R"(  ("\"|"|"/")  )");
        puts(R"(   \"|"|"|"/   )");
        puts(R"(     "| |"     )");
        puts(R"(      ^ ^      )");
    }
}

void mute_speaker(const int argc, void** argv, const char* argument) {
    SILENCE_UNUSED(argc); SILENCE_UNUSED(argv); SILENCE_UNUSED(argument);
    speaker_set_mute(false);
}

void print_label() {
    printf("FFT multiplication implementation by Ilya Kudryashov.\n");
    printf("Program can multiply very big things like numbers or polynomials.\n");
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n", __DATE__, __TIME__);
}

double read_number() {
    double result = 0;
    while (true) {
        printf(">>> ");
        int length = 0;
        scanf("%lg%n", &result, &length);
        while (getc(stdin) != '\n');
        if (length) break;
        say("No, this is not how you play the game.");
        printf("Number expected.\n");
    }
    return result;
}

void read_poly(poly_arg_t* polynomial, size_t size) {
    if (!polynomial) return;

    say("Enter degree of the polynomial.");
    printf("Enter degree of the first polynomial:\n");
    size_t degree = (size_t) floor(read_number());
    gentle_clamp(degree, size - 1);
    degree += 1;

    log_printf(STATUS_REPORTS, "status", "Entered polynomial:\n");
    for (size_t id = 0; id < degree; ++id) {
        if (id == 0)
            printf("Enter equation constant:\n");
        else
            printf("Enter coefficient before x^%lld:\n", (long long) id);
        polynomial[id] = { .x = read_number(), .y = 0.0 };
        _log_printf(STATUS_REPORTS, "status", "x^%lld: %lg", (long long) id, polynomial[id].x);
    }
}

void print_poly(const poly_arg_t* polynomial, size_t size) {
    if (!polynomial) return;
    size_t degree = poly_degree(polynomial, size);
    for (size_t id = degree; id != (size_t)( -1 ); --id) {
        double coeff = polynomial[id].x;
        if (equal(coeff, 0.0)) continue;

        if (coeff < 0.0) {
            if (id != degree) printf(" - ");
            else printf("-");
        } else if (id != degree) {
            printf(" + ");
        }

        if (id == 0 || !equal(coeff, 1.0))
            printf("%lg", abs(coeff));

        if (id > 1) {
            printf("x^%lld", (long long) id);
        } else if (id == 1) {
            printf("x");
        }
    }
}

void broom_up(poly_arg_t* poly, size_t size) {
    if (!poly) return;
    unsigned int carry = 0;
    for (size_t id = 0; id < size; id++) {
        unsigned int value = (unsigned int) round(poly[id].x);
        value += carry;
        carry = value / BIG_INT_BASE;
        value %= BIG_INT_BASE;
    }
}

void read_poly_as_big(poly_arg_t* poly, size_t size) {
    if (!poly) return;

    say("Enter the number in 16-base.");
    printf("16-base integer:\n>>>");

    static char input[MAX_READ_SIZE] = "";
    scanf("%s", input);

    size_t length = strlen(input);
    for (size_t id = 0; id * 2 < length - 1; ++id) {
        input[id] ^= input[length - id - 1];
        input[length - id - 1] ^= input[id];
        input[id] ^= input[length - id - 1];
    }
    log_printf(STATUS_REPORTS, "status", "Big int after inverse transformation: %s.\n", input);
    if (length > size * BIG_INT_STEP) length = size * BIG_INT_STEP - 1;

    for (size_t id = 0; id < length; id++) {
        int num = 0;
        if (input[id] >= '0' && input[id] - '0' <= 10) {
            num = input[id] - '0';
        } else if (input[id] >= 'a' && input[id] - 'a' < (int)BIG_INT_BASE - 10) {
            num = 10 + input[id] - 'a';
        } else {
            printf("Invalid character \'%c\' detected.\n", input[id]);
            log_printf(ERROR_REPORTS, "error", "Invalid character \'%c\' detected while reading user big int input.\n", input[id]);
            break;
        }
        poly[id / BIG_INT_STEP].x += num << (4 * (id % BIG_INT_STEP));
    }
}

void print_poly_as_big(const poly_arg_t* poly, size_t size) {
    if (!poly) return;

    size_t degree = poly_degree(poly, size);

    for (size_t digit = degree; digit != (size_t)-1; --digit) {
        printf("%x", (unsigned int) round(poly[digit].x));
    }
}

size_t poly_degree(const poly_arg_t* poly, size_t size) {
    if (!poly) return 0;

    size_t degree = 0;

    for (size_t id = 0; id < size; id++) {
        if (!equal(poly[id].x, 0.0))
            degree = id;
    }

    return degree;
}
