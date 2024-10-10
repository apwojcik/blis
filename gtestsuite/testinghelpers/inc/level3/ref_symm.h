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

#pragma once

#include "common/testing_helpers.h"

/*
 * ==========================================================================
 * For BLIS-typed interface SYMM performs one of the matrix-matrix operations
 *     C := alpha*conj( A )*trans( B ) + beta*C, if side is left
 *  or C := alpha*trans( B )*conj( A ) + beta*C, if side is right
 *  alpha and beta are scalars, and A is symmetric, B and C are matrices, with conj( A )
 *  an m by m matrix, and trans( B ) and C m by n matrices.
 *
 *  For BLAS/CBLAS interface SYMM performs one of the matrix-matrix operations
 *     C := alpha*A*B + beta*C, if side is left
 *  or C := alpha*B*A + beta*C, if side is right
 *  alpha and beta are scalars, and A is symmetric, B and C are matrices, with A
 *  an m by m matrix, and B and C m by n matrices.
 * ==========================================================================
**/

namespace testinghelpers {

template <typename T>
void ref_symm(
    char storage, char side, char uplo, char conja, char transb,
    gtint_t m, gtint_t n,
    T alpha,
    T* ap, gtint_t lda,
    T* bp, gtint_t ldb,
    T beta,
    T* cp, gtint_t ldc
);

} //end of namespace testinghelpers