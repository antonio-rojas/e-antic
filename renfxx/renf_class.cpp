/*
    Copyright (C) 2018 Vincent Delecroix
    Copyright (C) 2019 Julian Rüth

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include <e-antic/renfxx.h>

namespace {
// An index returned from std::ios_base::xalloc() that we use to store a
// pointer back to this number field.  We store a pointer to this number
// field at that index in a stream when we want to parse renf_elem_class
// elements from it.
static int xalloc = std::ios_base::xalloc();
} // end of anonymous namespace 

namespace eantic {

renf_class& renf_class::operator = (const renf_class& k) noexcept
{
    renf_clear(nf);
    renf_init_set(nf, k.nf);
    return *this;
}

/*****************/
/* I/O operators */
/*****************/

std::istream& renf_class::set_pword(std::istream& is) noexcept
{
    is.pword(xalloc) = this;
    return is;
}

std::ostream& operator << (std::ostream& os, const renf_elem_class& a)
{
    if (a.is_integer())
        os << a.str(EANTIC_STR_ALG);
    else
        os << a.str(EANTIC_STR_ALG | EANTIC_STR_D);
    return os;
}

std::istream& operator >> (std::istream& is, renf_elem_class& a)
{
    renf_class * nf = (renf_class *) is.pword(xalloc);

    throw std::logic_error("not implemented >>renf_elem_class - parse into string, assign to new element, then use operator= instead");

    /*
    if (nf != nullptr)
        // reset the number field with the one from the stream
        a.reset_parent(nf);

    a.assign_stream(is);
    return is;
    */
}

/*****************************/
/* renf_class implementation */
/*****************************/

renf_class::renf_class() noexcept
{
    fmpq_poly_t minpoly;
    arb_t emb;

    fmpq_poly_init(minpoly);
    fmpq_poly_set_coeff_si(minpoly, 0, -1);
    fmpq_poly_set_coeff_si(minpoly, 1, 1);

    arb_init(emb);
    arb_set_si(emb, 1);

    renf_init(nf, minpoly, emb, 64);

    fmpq_poly_clear(minpoly);
    arb_clear(emb);

    name = "a";
}

renf_class::~renf_class() noexcept
{
    renf_clear(nf);
}

renf_class::renf_class(const ::renf_t& k, const std::string& gen_name) noexcept
{
    renf_init_set(nf, k);
    this->name = gen_name;
}

renf_class::renf_class(const char * minpoly, const char * gen, const char * emb, const slong prec)
{
    arb_t e;
    fmpq_poly_t p;

    fmpq_poly_init(p);
    if (fmpq_poly_set_str_pretty(p, minpoly, gen))
    {
        fmpq_poly_clear(p);
        std::invalid_argument("renf_class: can not read polynomial from string");
    }
    name = gen;

    arb_init(e);
    if (arb_set_str(e, emb, prec))
    {
        arb_clear(e);
        std::invalid_argument("renf_class: can not read ball from string");
    }

    renf_init(nf, p, e, prec);
    fmpq_poly_clear(p);
    arb_clear(e);
}

renf_class::renf_class(const std::string& minpoly, const std::string& gen, const std::string emb, const slong prec)
{
    arb_t e;
    fmpq_poly_t p;

    fmpq_poly_init(p);
    if (fmpq_poly_set_str_pretty(p, minpoly.c_str(), gen.c_str()))
    {
        fmpq_poly_clear(p);
        std::invalid_argument("renf_class: can not read polynomial from string");
    }
    name = gen;

    arb_init(e);
    if (arb_set_str(e, emb.c_str(), prec))
    {
        arb_clear(e);
        std::invalid_argument("renf_class: can not read ball from string");
    }

    renf_init(nf, p, e, prec);

    fmpq_poly_clear(p);
    arb_clear(e);
}

bool renf_class::operator == (const renf_class& other) const noexcept
{
    return renf_equal(this->nf, other.nf);
}

renf_elem_class renf_class::zero() const noexcept
{
    renf_elem_class a(*this, 0);
    return a;
}

renf_elem_class renf_class::one() const noexcept
{
    renf_elem_class a(*this, 1);
    return a;
}

renf_elem_class renf_class::gen() const noexcept
{
    renf_elem_class a(*this);
    renf_elem_gen(a.renf_elem_t(), this->renf_t());
    return a;
}

slong renf_class::degree() const noexcept
{
    return nf_degree(nf->nf);
}

} // end of namespace eantic