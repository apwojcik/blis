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
#include "level3/trsm/test_trsm.h"

class ztrsmEVT :
        public ::testing::TestWithParam<std::tuple<char,            // storage format
                                                   char,            // side
                                                   char,            // uplo
                                                   char,            // transa
                                                   char,            // diaga
                                                   gtint_t,         // m
                                                   gtint_t,         // n
                                                   dcomplex,        // alpha
                                                   gtint_t,         // lda_inc
                                                   gtint_t,         // ldb_inc
                                                   EVT_TYPE,        // EVT test for A
                                                   EVT_TYPE>> {};   // EVT test for B


TEST_P( ztrsmEVT, API )
{
    using T = dcomplex;
    //----------------------------------------------------------
    // Initialize values from the parameters passed through
    // test suite instantiation (INSTANTIATE_TEST_SUITE_P).
    //----------------------------------------------------------
    // matrix storage format(row major, column major)
    char storage = std::get<0>(GetParam());
    // specifies matrix A appears left or right in
    // the matrix multiplication
    char side = std::get<1>(GetParam());
    // specifies upper or lower triangular part of A is used
    char uploa = std::get<2>(GetParam());
    // denotes whether matrix a is n,c,t,h
    char transa = std::get<3>(GetParam());
    // denotes whether matrix a in unit or non-unit diagonal
    char diaga = std::get<4>(GetParam());
    // matrix size m
    gtint_t m  = std::get<5>(GetParam());
    // matrix size n
    gtint_t n  = std::get<6>(GetParam());
    // specifies alpha value
    T alpha = std::get<7>(GetParam());
    // lda, ldb, ldc increments.
    // If increments are zero, then the array size matches the matrix size.
    // If increments are nonnegative, the array size is bigger than the matrix size.
    gtint_t lda_inc = std::get<8>(GetParam());
    gtint_t ldb_inc = std::get<9>(GetParam());

    EVT_TYPE a_init = std::get<10>(GetParam());
    EVT_TYPE b_init = std::get<11>(GetParam());

    // Set the threshold for the errors:
    // Check gtestsuite trsm.h or netlib source code for reminder of the
    // functionality from which we estimate operation count per element
    // of output, and hence the multipler for epsilon.
    // No adjustment applied yet for complex data.
    double thresh;
    if (m == 0 || n == 0 || alpha == testinghelpers::ZERO<T>())
        thresh = 0.0;
    else
        if ( side == 'l' || side == 'L' )
            thresh = 3*m*testinghelpers::getEpsilon<T>();
        else
            thresh = 3*n*testinghelpers::getEpsilon<T>();

    //----------------------------------------------------------
    //     Call test body using these parameters
    //----------------------------------------------------------
    test_trsm<T>( storage, side, uploa, transa, diaga, m, n, alpha, lda_inc, ldb_inc, thresh, a_init, b_init );
}

/**
 * @brief Test ZTRSM for extreme values
 * Code paths taken for:
 *      TRSV         -> 1
 *      AVX2 Small   -> 151, 82
 *      Native       -> 503, 512
 */
INSTANTIATE_TEST_SUITE_P(
        evt,
        ztrsmEVT,
        ::testing::Combine(
            ::testing::Values('c'
#ifndef TEST_BLAS
            ,'r'
#endif
            ),                                                               // storage format
            ::testing::Values('l','r'),                                      // side  l:left, r:right
            ::testing::Values('u','l'),                                      // uplo  u:upper, l:lower
            ::testing::Values('n','c', 't'),                                 // transa
            ::testing::Values('n','u'),                                      // diaga , n=nonunit u=unit
            ::testing::Values(1, 151, 503),                                  // m
            ::testing::Values(1, 82,  512),                                  // n
            ::testing::Values(dcomplex{-2.4, 2.0},
                              dcomplex{-0.0, 2.3},
                              dcomplex{-2.4, 0.0},
                              dcomplex{ 0.0, 0.0}),                          // alpha
            ::testing::Values(gtint_t(0)),                                   // increment to the leading dim of a
            ::testing::Values(gtint_t(0)),                                   // increment to the leading dim of b
            ::testing::Values(NO_EVT, NaN, INF, NaN_INF, DIAG_NaN, DIAG_INF,
                              NEG_INF, NEG_NaN),                             // EVT test for A
            ::testing::Values(NO_EVT, NaN, INF, NaN_INF, NEG_INF, NEG_NaN)   // EVT test for B
        ),
        ::trsmEVTPrint<dcomplex>()
    );

/**
 * @brief Test ZTRSM with differnt values of alpha
 *      code paths covered:
 *          TRSV              -> 1
 *          TRSM_AVX2_small   -> 3
 *          TRSM_NATIVE       -> 501
 */
INSTANTIATE_TEST_SUITE_P(
        Alpha,
        ztrsmEVT,
        ::testing::Combine(
            ::testing::Values('c'),                                          // storage format
            ::testing::Values('l','r'),                                      // side  l:left, r:right
            ::testing::Values('u','l'),                                      // uplo  u:upper, l:lower
            ::testing::Values('n', 'c', 't'),                                // transa
            ::testing::Values('n','u'),                                      // diaga , n=nonunit u=unit
            ::testing::Values(1, 3, 501),                                    // n
            ::testing::Values(1, 3, 501),                                    // m
            ::testing::Values(dcomplex{NAN, -2.0},
                              dcomplex{-2.0, NAN},
                              dcomplex{INFINITY, 3.1f},
                              dcomplex{NAN, -INFINITY}),                     // alpha
            ::testing::Values(gtint_t(0), gtint_t(5)),                       // increment to the leading dim of a
            ::testing::Values(gtint_t(0), gtint_t(3)),                       // increment to the leading dim of b
            ::testing::Values(NO_EVT),                                       // EVT test for A
            ::testing::Values(NO_EVT)                                        // EVT test for B
        ),
        ::trsmEVTPrint<dcomplex>()
    );
