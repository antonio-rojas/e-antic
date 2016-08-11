/*
    Copyright (C) 2016 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/


#include "flint.h"
#include "nf_emb_elem.h"
#include "arb.h"

void check_floor(nf_emb_elem_t a, nf_emb_t nf, int ans, const char * s)
{
	fmpz_t n;
	int test;

	fmpz_init(n);
	nf_emb_elem_floor(n, a, nf);
	test = fmpz_cmp_si(n, ans);
	fmpz_clear(n);

	if(test)
	{
		printf("FAIL:\n");
		printf("nf = "); nf_print(nf->nf); printf("\n");
		printf("emb = "); arb_printd(NF_REMB_REF(nf), 10); printf("\n");
		printf("a = "); nf_emb_elem_print_pretty(a, nf, s, 10); printf("\n");
		printf("got n = "); fmpz_print(n); printf(" but expected %d\n", ans);
		abort();
	}
}

#define TEST_FLOOR_CLEANUP \
	nf_emb_elem_clear(a, nf); \
	nf_emb_clear(nf); \
	fmpq_clear(k); \
	fmpq_poly_clear(p);
 
void test_field1()
{
	/* tests in QQ[sqrt(5)] */
	int iter;

	fmpq_t k;
	fmpq_poly_t p;
	arb_t emb;
	nf_emb_t nf;
	nf_emb_elem_t a;
    FLINT_TEST_INIT(state);

	fmpq_init(k);
	fmpq_poly_init(p);

	fmpq_poly_set_coeff_si(p, 2, 1);
	fmpq_poly_set_coeff_si(p, 1, -1);
	fmpq_poly_set_coeff_si(p, 0, -1);

	arb_init(emb);
	arb_set_d(emb, 1.61803398874989);
	arb_add_error_2exp_si(emb, -20);
	nf_emb_real_init(nf, p, emb, 20 + n_randint(state, 100));
	arb_clear(emb);

	nf_emb_elem_init(a, nf);


	/* (1+sqrt(5))/2 vs Fibonacci */
	fmpq_poly_zero(p);
	fmpq_poly_set_coeff_si(p, 1, -1);
	for(iter=1; iter<2000; iter++)
	{
		fmpz_fib_ui(fmpq_numref(k), iter+1);
		fmpz_fib_ui(fmpq_denref(k), iter);
		fmpq_poly_set_coeff_fmpq(p, 0, k);
		nf_emb_elem_set_fmpq_poly(a, p, nf);
		check_floor(a,nf,-iter%2,"sqrt(5)");
	}

    FLINT_TEST_CLEANUP(state);
	TEST_FLOOR_CLEANUP;
}

void test_field2()
{
	/* tests in QQ[3^(1/4)] */
	nf_emb_t nf;
	nf_emb_elem_t a;
	fmpq_t d,k;
	fmpq_poly_t p;
	FLINT_TEST_INIT(state);

	fmpq_init(d);
	fmpq_poly_init(p);

	fmpq_set_si(d, 3, 1);
	nf_emb_init_nth_root_fmpq(nf, d, 4, 20 + n_randint(state,100));

	fmpq_clear(d);


	fmpq_init(k);
	nf_emb_elem_init(a, nf);

	/* test rationals */

	/* --> 3^(1/4)  */
	fmpq_poly_set_coeff_si(p, 1, 1);
	nf_emb_elem_set_fmpq_poly(a, p, nf);


	check_floor(a, nf, 1, "3^(1/4)");

	/* --> 3^(1/4) - p_34 / q_34 */
	/*     floor = 0 */
	fmpz_set_str(fmpq_numref(k), "3871793620206447926", 10);
	fmpz_set_str(fmpq_denref(k), "2941926960111028069", 10);
	fmpq_neg(k, k);
	fmpq_poly_set_coeff_fmpq(p, 0, k);
	nf_emb_elem_set_fmpq_poly(a, p, nf);

	check_floor(a, nf, 0, "3^(1/4)");

	/* --> 3^(1/4) - p_35 / q_35 */
	/*     floor = -1            */
	fmpz_set_str(fmpq_numref(k), "4393442218385055959", 10);
	fmpz_set_str(fmpq_denref(k), "3338294180377262795", 10);
	fmpq_neg(k, k);
	fmpq_poly_set_coeff_fmpq(p, 0, k);
	nf_emb_elem_set_fmpq_poly(a, p, nf);

	check_floor(a, nf, -1, "3^(1/4)");

	/* --> 3^(1/4) - p_200 / q_200 */
	fmpz_set_str(fmpq_numref(k), "51566086581654990699052199424489069476470199719930170996263916596162993841059250500042162091", 10);
	fmpz_set_str(fmpq_denref(k), "39181752754141206003124111890355840072199542360218864430892618765033598468868752146602163065", 10);
	fmpq_neg(k, k);
	fmpq_poly_set_coeff_fmpq(p, 0, k);
	nf_emb_elem_set_fmpq_poly(a, p, nf);

	check_floor(a, nf, 0, "3^(1/4)");

	/* --> 3^(1/4) - p_201 / q_201 */
	fmpz_set_str(fmpq_numref(k), "80796322887694335717970676356641716096406222234122724217891106756946083353628876437327250032", 10);
	fmpz_set_str(fmpq_denref(k), "61391929399498685496270115285641595325756438975454257165479021482386018841773493669624721869", 10);
	fmpq_neg(k, k);
	fmpq_poly_set_coeff_fmpq(p, 0, k);
	nf_emb_elem_set_fmpq_poly(a, p, nf);

	check_floor(a, nf, -1, "3^(1/4)");

	/* */
	fmpz_set_str(fmpq_numref(k), "13231942875843754343234", 10);
	fmpz_set_str(fmpq_denref(k), "14321431341231112121", 10);
	fmpq_poly_set_coeff_fmpq(p, 3, k);
	fmpz_set_str(fmpq_numref(k), "148589873455543948591", 10);
	fmpz_set_str(fmpq_denref(k), "12332111221111", 10);
	fmpq_neg(k, k);
	fmpq_poly_set_coeff_fmpq(p, 2, k);
	fmpz_set_str(fmpq_numref(k), "1233321469998711012392391", 10);
	fmpz_set_str(fmpq_denref(k), "11814121556810191", 10);
	fmpq_poly_set_coeff_fmpq(p, 1, k);
	fmpz_set_str(fmpq_numref(k), "1249152314425433983202991363672458443993964487436329478959287771807457205881969983777233465754608376177969464841", 10);
	fmpz_set_str(fmpq_denref(k), "10720278662399817731713810382544982753044312944075797382817281426908463944866446042500978893159281330135", 10);
	fmpq_neg(k, k);
	fmpq_poly_set_coeff_fmpq(p, 0, k);
	nf_emb_elem_set_fmpq_poly(a, p, nf);

	check_floor(a, nf, 230, "3^(1/4)");

    FLINT_TEST_CLEANUP(state);
	TEST_FLOOR_CLEANUP;
}

int main()
{

	printf("floor....");
	fflush(stdout);

	test_field1();
	test_field2();

	printf("PASS\n");
	return 0;
}

