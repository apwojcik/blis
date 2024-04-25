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

#include "trsv.h"
#include "inc/check_error.h"
#include "common/testing_helpers.h"
#include "common/wrong_inputs_helpers.h"
#include <stdexcept>
#include <algorithm>
#include <gtest/gtest.h>

template <typename T>
class TRSV_IIT_ERS_Test : public ::testing::Test {};
typedef ::testing::Types<float, double, scomplex, dcomplex> TypeParam;
TYPED_TEST_SUITE(TRSV_IIT_ERS_Test, TypeParam);


#ifdef TEST_BLAS

using namespace testinghelpers::IIT;

/*
    Incorrect Input Testing(IIT)

    BLAS exceptions get triggered in the following cases(for TRSV):
    1. When UPLO  != 'L' || UPLO   != 'U'                  (info = 1)
    2. When TRANS != 'N' || TRANS  != 'T' || TRANS != 'C'  (info = 2)
    3. When DIAG  != 'U' || DIAG   != 'N'                  (info = 3)
    4. When n < 0                                          (info = 4)
    5. When lda < N                                        (info = 6)
    6. When incx == 0                                      (info = 8)
*/


/**
 * @brief Test TRSV when UPLO argument is incorrect
 *        when info == 1
 *
 */
TYPED_TEST(TRSV_IIT_ERS_Test, invalid_UPLO)
{
    using T = TypeParam;
    T alpha = T{1};

    std::vector<T> x = testinghelpers::get_random_vector<T>(0, 1, N, INC);
    std::vector<T> x_ref(x);

    trsv<T>( STORAGE, 'A', TRANS, DIAG, N, &alpha, nullptr, LDA, x.data(), INC);
    computediff<T>( "x", N, x.data(), x_ref.data(), INC );

#ifdef CAN_TEST_INFO_VALUE
    gtint_t info = bli_info_get_info_value();
    computediff<gtint_t>( "info", info, 1 );
#endif
}

/**
 * @brief Test TRSV when TRANS argument is incorrect
 *        when info == 2
 *
 */
TYPED_TEST(TRSV_IIT_ERS_Test, invalid_TRANS)
{
    using T = TypeParam;
    T alpha = T{1};

    std::vector<T> x = testinghelpers::get_random_vector<T>(0, 1, N, INC);
    std::vector<T> x_ref(x);

    trsv<T>( STORAGE, UPLO, 'A', DIAG, N, &alpha, nullptr, LDA, x.data(), INC);
    computediff<T>( "x", N, x.data(), x_ref.data(), INC );

#ifdef CAN_TEST_INFO_VALUE
    gtint_t info = bli_info_get_info_value();
    computediff<gtint_t>( "info", info, 2 );
#endif
}

/**
 * @brief Test TRSV when DIAG argument is incorrect
 *        when info == 3
 */
TYPED_TEST(TRSV_IIT_ERS_Test, invalid_DIAG)
{
    using T = TypeParam;
    T alpha = T{1};

    std::vector<T> x = testinghelpers::get_random_vector<T>(0, 1, N, INC);
    std::vector<T> x_ref(x);

    trsv<T>( STORAGE, UPLO, TRANS, 'A', N, &alpha, nullptr, LDA, x.data(), INC);
    computediff<T>( "x", N, x.data(), x_ref.data(), INC );

#ifdef CAN_TEST_INFO_VALUE
    gtint_t info = bli_info_get_info_value();
    computediff<gtint_t>( "info", info, 3 );
#endif
}

/**
 * @brief Test TRSV when N is negative
 *        when info == 4
 */
TYPED_TEST(TRSV_IIT_ERS_Test, invalid_n)
{
    using T = TypeParam;
    T alpha = T{1};

    std::vector<T> x = testinghelpers::get_random_vector<T>(0, 1, N, INC);
    std::vector<T> x_ref(x);

    trsv<T>( STORAGE, UPLO, TRANS, DIAG, -1, &alpha, nullptr, LDA, x.data(), INC);
    computediff<T>( "x", N, x.data(), x_ref.data(), INC );

#ifdef CAN_TEST_INFO_VALUE
    gtint_t info = bli_info_get_info_value();
    computediff<gtint_t>( "info", info, 4 );
#endif
}


/**
 * @brief Test TRSV when lda < max(1, N)
 *        when info == 6
 */
TYPED_TEST(TRSV_IIT_ERS_Test, invalid_lda)
{
    using T = TypeParam;
    T alpha = T{1};

    std::vector<T> x = testinghelpers::get_random_vector<T>(0, 1, N, INC);
    std::vector<T> x_ref(x);

    trsv<T>( STORAGE, UPLO, TRANS, DIAG, N, &alpha, nullptr, LDA - 1, x.data(), INC);
    computediff<T>( "x", N, x.data(), x_ref.data(), INC );

#ifdef CAN_TEST_INFO_VALUE
    gtint_t info = bli_info_get_info_value();
    computediff<gtint_t>( "info", info, 6 );
#endif
}

/**
 * @brief Test TRSV when INCX == 0
 *        when info == 8
 */
TYPED_TEST(TRSV_IIT_ERS_Test, invalid_incx)
{
    using T = TypeParam;
    T alpha = T{1};

    std::vector<T> x = testinghelpers::get_random_vector<T>(0, 1, N, INC);
    std::vector<T> x_ref(x);

    trsv<T>( STORAGE, UPLO, TRANS, DIAG, N, &alpha, nullptr, LDA, x.data(), 0);
    computediff<T>( "x", N, x.data(), x_ref.data(), INC );

#ifdef CAN_TEST_INFO_VALUE
    gtint_t info = bli_info_get_info_value();
    computediff<gtint_t>( "info", info, 8 );
#endif
}


/*
    Early Return Scenarios(ERS) :

    The TRSV API is expected to return early in the following cases:

    1. When n == 0.

*/

/**
 * @brief Test TRSV when N is zero
 */
TYPED_TEST(TRSV_IIT_ERS_Test, n_eq_zero)
{
    using T = TypeParam;
    T alpha = T{1};

    std::vector<T> x = testinghelpers::get_random_vector<T>(0, 1, N, INC);
    std::vector<T> x_ref(x);

    trsv<T>( STORAGE, UPLO, TRANS, DIAG, 0, &alpha, nullptr, LDA, x.data(), INC);
    computediff<T>( "x", N, x.data(), x_ref.data(), INC );

#ifdef CAN_TEST_INFO_VALUE
    gtint_t info = bli_info_get_info_value();
    computediff<gtint_t>( "info", info, 0 );
#endif
}

#endif
