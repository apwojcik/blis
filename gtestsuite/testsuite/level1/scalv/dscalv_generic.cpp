/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2023 - 2024, Advanced Micro Devices, Inc. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
	- Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	- Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	- Neither the name(s) of the copyright holder(s) nor the names of its
	  contributors may be used to endorse or promote products derived
	  from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <gtest/gtest.h>
#include "test_scalv.h"

class dscalvGeneric :
        public ::testing::TestWithParam<std::tuple<char,
                                                   gtint_t,
                                                   gtint_t,
                                                   double>> {};


// Tests using random integers as vector elements.
TEST_P( dscalvGeneric, API )
{
    using T = double;
    //----------------------------------------------------------
    // Initialize values from the parameters passed through
    // test suite instantiation (INSTANTIATE_TEST_SUITE_P).
    //----------------------------------------------------------
    // denotes whether alpha or conj(alpha) will be used:
    char conj_alpha = std::get<0>(GetParam());
    // vector length:
    gtint_t n = std::get<1>(GetParam());
    // stride size for x:
    gtint_t incx = std::get<2>(GetParam());
    // alpha
    T alpha = std::get<3>(GetParam());

    // Set the threshold for the errors:
    // Check gtestsuite scalv.h or netlib source code for reminder of the
    // functionality from which we estimate operation count per element
    // of output, and hence the multipler for epsilon.
    double thresh;
    if (n == 0)
        thresh = 0.0;
    else if (alpha == testinghelpers::ZERO<T>() || alpha == testinghelpers::ONE<T>())
        thresh = 0.0;
    else
        thresh = testinghelpers::getEpsilon<T>();

    //----------------------------------------------------------
    //     Call generic test body using those parameters
    //----------------------------------------------------------
    test_scalv<T>( conj_alpha, n, incx, alpha, thresh );
}

// Black box testing for generic use of dscal.
INSTANTIATE_TEST_SUITE_P(
        unitPositiveIncrement,
        dscalvGeneric,
        ::testing::Combine(
            // conj(alpha): uses n (no_conjugate) since it is real.
            ::testing::Values('n'),
            // m: size of vector.
            ::testing::Range(gtint_t(10), gtint_t(101), 10),
            // incx: stride of x vector.
            ::testing::Values(
                                gtint_t(1)
            ),
            // alpha: value of scalar.
            ::testing::Values(
                                double( 0.0),
                                double( 7.0),
                                double(-3.0)
            )
        ),
        (::scalvGenericPrint<double, double>())
    );

INSTANTIATE_TEST_SUITE_P(
        nonUnitPositiveIncrement,
        dscalvGeneric,
        ::testing::Combine(
            // conj(alpha): uses n (no_conjugate) since it is real.
            ::testing::Values('n'),
            // m: size of vector.
            ::testing::Range(gtint_t(10), gtint_t(101), 10),
            // incx: stride of x vector.
            ::testing::Values(
                                gtint_t(2),
                                gtint_t(3)
            ),
            // alpha: value of scalar.
            ::testing::Values(
                                double( 0.0),
                                double( 7.0),
                                double(-3.0)
            )
        ),
        (::scalvGenericPrint<double, double>())
    );

#ifdef TEST_BLIS_TYPED
// Test when conjugate of x is used as an argument. This option is BLIS-api specific.
// Only test very few cases as sanity check since conj(x) = x for real types.
// We can modify the values using implementantion details.
INSTANTIATE_TEST_SUITE_P(
        conjalpha,
        dscalvGeneric,
        ::testing::Combine(
            ::testing::Values('c'),                                          // c: use conjugate
            ::testing::Values(gtint_t(3), gtint_t(30), gtint_t(112)),        // m size of vector takes values from 10 to 100 with step size of 10.
            ::testing::Values(gtint_t(1)),                                   // stride size for x
            ::testing::Values(double(-3.0))                                  // alpha
        ),
        (::scalvGenericPrint<double, double>())
    );
#endif

#if defined(BLIS_ENABLE_OPENMP) && defined(AOCL_DYNAMIC)
INSTANTIATE_TEST_SUITE_P(
        AOCLDynamic,
        dscalvGeneric,
        ::testing::Combine(
            // conj(alpha): uses n (no_conjugate) since it is real.
            ::testing::Values('n'),
            // m: size of vector.
            ::testing::Values(
                               gtint_t(   30000),     // nt_ideal = 1
                               gtint_t(  100000),     // nt_ideal = 2
                               gtint_t(  500000),     // nt_ideal = 8
                               gtint_t( 2500000),     // nt_ideal = 12
                               gtint_t( 4000000),     // nt_ideal = 16
                               gtint_t( 7000000),     // nt_ideal = 24
                               gtint_t(10000000),     // nt_ideal = 32
                               gtint_t(25000000)      // nt_ideal = max_available
            ),
            // incx: stride of x vector.
            ::testing::Values(
                                gtint_t(1)
            ),
            // alpha: value of scalar.
            ::testing::Values(
                                double( 7.0)
            )
        ),
        (::scalvGenericPrint<double, double>())
    );
#endif
