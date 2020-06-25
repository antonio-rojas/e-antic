/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 William Hart

******************************************************************************/

#include <stdio.h>
#include <antic/nf.h>
#include "../../upstream/patched/nf.h"
#include "e-antic/nf_elem.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("set/equal....");
    fflush(stdout);

    flint_randinit(state);

    /* set a = b, check a == b */
    for (i = 0; i < 100; i++)
    {
        nf_t nf;
        nf_elem_t a, b;

        nf_init_randtest(nf, state, 40, 200);

        nf_elem_init(a, nf);
        nf_elem_init(b, nf);
        
        nf_elem_randtest(b, state, 200, nf);
        
        nf_elem_set(a, b, nf);

        result = (nf_elem_equal(a, b, nf));
        if (!result)
        {
           printf("FAIL:\n");
           printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
           printf("b = "); nf_elem_print_pretty(b, nf, "x"); printf("\n");
           abort();
        }

        nf_elem_clear(a, nf);
        nf_elem_clear(b, nf);
         
        nf_clear(nf);
    }

    /* test aliasing a and b */
    for (i = 0; i < 100; i++)
    {
        nf_t nf;
        nf_elem_t a;

        nf_init_randtest(nf, state, 40, 200);

        nf_elem_init(a, nf);
        
        nf_elem_randtest(a, state, 200, nf);
        
        nf_elem_set(a, a, nf);

        result = (nf_elem_equal(a, a, nf));
        if (!result)
        {
           printf("FAIL:\n");
           printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
           abort();
        }

        nf_elem_clear(a, nf);
         
        nf_clear(nf);
    }

    flint_randclear(state);
    flint_printf("PASS\n");
    return 0;
}
