/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "poly_extra.h"

void _fmpz_poly_bisection_step_arf(arf_t l, arf_t r, const fmpz * pol, slong len, int sl, int sr, slong prec)
{
    arb_t b, c;
    arf_t m;

    arf_init(m);
    arb_init(b);
    arb_init(c);

    do
    {
        /* TODO: we should do the mean operation faster */
        /* it is likely that l and r shares a lot       */
        arf_add(m, l, r, prec, ARF_RND_NEAR);
        arf_div_ui(m, m, 2, prec, ARF_RND_NEAR);
        if ((arf_cmp(m, l) <= 0) || (arf_cmp(r, m) <= 0))
        {
            arf_clear(m);
            arb_clear(b);
            arb_clear(c);
            break;
        }

        arb_set_arf(b, m);
        _fmpz_poly_evaluate_arb(c, pol, len, b, prec);
        if (arb_contains_zero(c))
        {
            arf_clear(m);
            arb_clear(b);
            arb_clear(c);
            break;
        }
        if (arf_sgn(arb_midref(c)) == sl)
            arf_set(l, m);
        else
            arf_set(r, m);

    } while (1);
}

int fmpz_poly_bisection_step_arb(arb_t res, const fmpz_poly_t pol, arb_t a, slong prec)
{
    int sl, sr, ans;
    arb_t b, c, rres;
    arf_t l, r;

    arf_init(l);
    arf_init(r);
    arb_init(b);
    arb_init(c);
    arb_init(rres);

    /* set l, r as the left and right endpoint of the ball */
    /* compute the signs of pol(l) and pol(r)              */
    arb_get_interval_arf(l, r, a, prec);

    arb_set_arf(b, l);
    fmpz_poly_evaluate_arb(c, pol, b, prec);
    if (arb_contains_zero(c))
    {
        arf_clear(l);
        arf_clear(r);
        arb_clear(b);
        arb_clear(c);
        arb_clear(rres);
        return 0;
    }
    sl = arf_sgn(arb_midref(c));

    arb_set_arf(b, r);
    fmpz_poly_evaluate_arb(c, pol, b, prec);
    if (arb_contains_zero(c))
    {
        arf_clear(l);
        arf_clear(r);
        arb_clear(b);
        arb_clear(c);
        arb_clear(rres);
        return 0;
    }
    sr = arf_sgn(arb_midref(c));

    _fmpz_poly_bisection_step_arf(l, r, pol->coeffs, fmpz_poly_length(pol), sl, sr, prec);
    arb_set_interval_arf(rres, l, r, prec);
    ans = arb_contains(a, rres) && !arb_equal(rres, a);
    if (ans)
        arb_swap(res, rres);

    arb_clear(b);
    arb_clear(c);
    arb_clear(rres);
    arf_clear(l);
    arf_clear(r);

    return ans;
}
