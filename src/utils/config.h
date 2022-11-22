/**
 * @file config.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief List of constants used in main program.
 * @version 0.1
 * @date 2022-11-01
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef MAIN_CONFIG_H
#define MAIN_CONFIG_H

#include <stdlib.h>
#include <math.h>

static const int NUMBER_OF_OWLS = 10;

static const size_t MAX_POLYNOMIAL_SIZE = 1024;

static const size_t MAX_READ_SIZE = 2048;

static const unsigned int BIG_INT_BASE = 4096;

//* = 3;
static const unsigned int BIG_INT_STEP = (unsigned int)(log2(BIG_INT_BASE) / log2(16));

#endif