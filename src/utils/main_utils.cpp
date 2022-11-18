#include "main_utils.h"

#include <stdlib.h>
#include <stdarg.h>

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
    log_printf(STATUS_REPORTS, "status", "Entered polynomial:\n");
    for (size_t id = 0; id < size; ++id) {
        if (id == 0)
            printf("Enter equation constant:\n");
        else
            printf("Enter coefficient before x^%lld:\n", (long long) id);
        polynomial[id] = { .x = read_number(), .y = 0.0 };
        _log_printf(STATUS_REPORTS, "status", "x^%lld: %lg", (long long) id, polynomial[id].x);
    }
}

void print_poly(const poly_arg_t* polynomial, size_t size) {
    size_t degree = 0;
    for (size_t id = 0; id < size; id++) {
        if (!equal(polynomial[id].x, 0.0))
            degree = id;
    }
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
