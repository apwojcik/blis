/*

   BLISPP
   C++ test driver for BLIS CPP dotc routine and reference blis dotc routine.

   Copyright (C) 2019 - 2023, Advanced Micro Devices, Inc. All rights reserved.

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

#include <complex>
#include <iostream>
#include "blis.hh"
#include "test.hh"

using namespace blis;
using namespace std;
//#define PRINT
#define N 16

template< typename T >
void ref_dotc(int64_t n,
             T *X,
             T *Y,
             T *res_ref
            )
{
    obj_t obj_x;
    obj_t obj_y;
    obj_t obj_res;
    num_t dt;

    if(is_same<T, complex<float>>::value)
        dt = BLIS_SCOMPLEX;
    else if(is_same<T, complex<double>>::value)
        dt = BLIS_DCOMPLEX;

    bli_obj_create_with_attached_buffer( dt, n, 1, X, 1, n, &obj_x );
    bli_obj_create_with_attached_buffer( dt, n, 1, Y, 1, n, &obj_y );
    bli_obj_set_conj(BLIS_CONJUGATE,&obj_x);
    bli_obj_create_with_attached_buffer( dt, 1, 1, res_ref, 1, 1,&obj_res );

    bli_dotv(&obj_x,
             &obj_y,
             &obj_res );

}

template< typename T >
void test_dotc()
{
    T *X, *Y;
    int n;
    int incx, incy;
    T res = 0, res_ref = 0;

    n = N;

    incx = 1;
    incy = 1;
     
    srand (time(NULL));
    allocate_init_buffer(X , n , 1);
    allocate_init_buffer(Y , n , 1);

#ifdef PRINT
    printvector(X, n,(char *) "X");
    printvector(Y, n,(char *) "Y");
#endif

    res = blis::dotc<T>(
            n,
            X,
            incx,
            Y,
            incy
            );

#ifdef PRINT
    cout<< "Dot product \n" << res << "\n";
#endif
    ref_dotc(n, X, Y , &res_ref ); 

#ifdef PRINT
    cout<< "Dot product ref\n" << res_ref << "\n";;
#endif

    if(res != res_ref )
        printf("%s TEST FAIL\n" ,__PRETTY_FUNCTION__);
    else
        printf("%s TEST PASS\n" , __PRETTY_FUNCTION__);

    delete[]( X     );
    delete[]( Y     );
}

// -----------------------------------------------------------------------------
int main( int argc, char** argv )
{
    test_dotc<complex<float>>( );
    test_dotc<complex<double>>( );
    return 0;

}
