/*

   BLIS
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.

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
#include "test_asumv.h"

class dasumvGenericTest :
        public ::testing::TestWithParam<std::tuple<gtint_t, gtint_t>> {};

TEST_P( dasumvGenericTest, RandomData )
{
    using T = double;
    //----------------------------------------------------------
    // Initialize values from the parameters passed through
    // test suite instantiation (INSTANTIATE_TEST_SUITE_P).
    //----------------------------------------------------------
    // vector length:
    gtint_t n = std::get<0>(GetParam());
    // stride size for x:
    gtint_t incx = std::get<1>(GetParam());

    // Set the threshold for the errors:
    double thresh = n*testinghelpers::getEpsilon<T>();

    //----------------------------------------------------------
    //     Call test body using these parameters
    //----------------------------------------------------------
    test_asumv<T>( n, incx, thresh );
}

// Prints the test case combination
class dasumvGenericTestPrint {
public:
    std::string operator()(
        testing::TestParamInfo<std::tuple<gtint_t, gtint_t>> str) const {
        gtint_t n     = std::get<0>(str.param);
        gtint_t incx  = std::get<1>(str.param);
#ifdef TEST_BLAS
        std::string str_name = "dasumv_";
#elif TEST_CBLAS
        std::string str_name = "cblas_dasumv";
#else  //#elif TEST_BLIS_TYPED
        std::string str_name = "bli_dasumv";
#endif
        str_name    = str_name + "_n" + std::to_string(n);
        std::string incx_str = ( incx > 0) ? std::to_string(incx) : "m" + std::to_string(std::abs(incx));
        str_name    = str_name + "_incx" + incx_str;
        return str_name;
    }
};

INSTANTIATE_TEST_SUITE_P(
        unitPositiveIncrement,
        dasumvGenericTest,
        ::testing::Combine(
            // m: size of vector.
            ::testing::Values(
                gtint_t( 1),
                gtint_t( 2),
                gtint_t( 3),
                gtint_t( 5),
                gtint_t( 7),
                gtint_t( 9),
                gtint_t(10),
                gtint_t(15),
                gtint_t(20),
                gtint_t(55),
                gtint_t(99)
            ),
            // incx: stride of x vector.
            ::testing::Values(
                gtint_t(1)
            )
        ),
        ::dasumvGenericTestPrint()
    );

INSTANTIATE_TEST_SUITE_P(
        nonUnitPositiveIncrement,
        dasumvGenericTest,
        ::testing::Combine(
            // m: size of vector.
            ::testing::Values(
                gtint_t( 1),
                gtint_t( 2),
                gtint_t( 3),
                gtint_t( 5),
                gtint_t( 7),
                gtint_t( 9),
                gtint_t(10),
                gtint_t(15),
                gtint_t(20),
                gtint_t(55),
                gtint_t(99)
            ),
            // incx: stride of x vector.
            ::testing::Values(
                gtint_t(2),
                gtint_t(3)
            )
        ),
        ::dasumvGenericTestPrint()
    );

// @note: ASUMV is supposed to set sum as 0 and return early in case incx <= 0,
//  but since it is currently not following this, failures are being observed.
#ifndef TEST_BLIS_TYPED
INSTANTIATE_TEST_SUITE_P(
        negativeIncrement,
        dasumvGenericTest,
        ::testing::Combine(
            // m: size of vector.
            ::testing::Values(
                gtint_t( 1),
                gtint_t( 2),
                gtint_t( 3),
                gtint_t( 5),
                gtint_t( 7),
                gtint_t( 9),
                gtint_t(10),
                gtint_t(15),
                gtint_t(20),
                gtint_t(55),
                gtint_t(99)
            ),
            // incx: stride of x vector.
            ::testing::Values(
                gtint_t(-1),
                gtint_t(-2),
                gtint_t(-3)
            )
        ),
        ::dasumvGenericTestPrint()
    );
#endif