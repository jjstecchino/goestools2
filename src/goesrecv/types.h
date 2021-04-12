#pragma once

#include <complex>
#include <vector>

#include "queue.h"

/**
 * Software Defined Radios (SDR) perform quadrature sampling
 * resulting in a stream of complex float numbers.
 * 
 * I = In phase sample (real component)
 * Q = Quadrature sample (imaginary component)
 * 
 * @brief Samples = type alias for a vector of complex float numbers
 *                  used to store a packet of data samples from the
 *                  receiver.
 */
typedef std::vector<std::complex<float> > Samples;
