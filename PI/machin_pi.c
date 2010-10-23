/*
 * Patrick Jennings
 *
 * A simple multithreaded high precision pi calculator.
 *
 * Uses a Maclaurin series to approximate arctan(1/5)
 * and arctan(1/239). Then uses Machin's formula to
 * approximate pi.
 *
 * Formulas:
 * pi/4 = 4*arctan(1/5) - arctan(1/239) (Machin's formula)
 * arctan(z) = z - z^3/3 + z^5/5 - z^7/7 + ... (Maclaurin series)
 *
 */

#include <stdlib.h>
#include <gmp.h>
#include <pthread.h>

#define ITERATIONS	2 << 15		// Number of iterations for each Maclaurin series
#define NUM_THREADS	8		// Number of threads to execute
#define PI_PRECISION	512		// Precision of pi in bits
#define PI_ERROR	-1

mpf_t thread_total[NUM_THREADS];

int thread_calc(void *func, mpf_t *total);
void *calc_arctan_one_five(void *i);
void *calc_arctan_one_twothreenine(void *i);
void calc_arctan(int threadid, int num, int den);

int main(int argc, char *argv[]) {
	mpf_t arctan_one_five;
	mpf_t arctan_one_twothreenine;
	mpf_t pi;
	int i;

	mpf_set_default_prec(PI_PRECISION);
	mpf_init(pi);

	// Get the approximations of arctan(1/5) and arctan(1/239) from
	// the Maclaurin series.
	if(thread_calc(calc_arctan_one_five, &arctan_one_five) == PI_ERROR)
		return PI_ERROR;
	if(thread_calc(calc_arctan_one_twothreenine, &arctan_one_twothreenine) == PI_ERROR)
		return PI_ERROR;

	//gmp_printf("arctan of 1/5: %.64Ff\n", arctan_one_five);
	//gmp_printf("arctan of 1/239: %.64Ff\n", arctan_one_twothreenine);
	
	// Calculate PI from the two arctan approximations and use Machin's
	// formula to get an approximation of pi.
	mpf_mul_ui(arctan_one_five, arctan_one_five, 4);
	mpf_sub(pi, arctan_one_five, arctan_one_twothreenine);
	mpf_mul_ui(pi, pi, 4);
	
	gmp_printf("%d iterations: %.128Ff\n", ITERATIONS, pi);
	
	mpf_clear(pi);
	mpf_clear(arctan_one_five);
	mpf_clear(arctan_one_twothreenine);
	return 0;
}

// Important: total will be initialized inside of thread_calc. This
// means that all information will be wiped inside of its struct.
//
// Creates NUM_THREADS threads and calls the passed in function
// for each thread.
int thread_calc(void *func, mpf_t *total) {
	pthread_t thread_pool[NUM_THREADS];
	int *thread_id[NUM_THREADS];
	int i;

	mpf_init2(*total, PI_PRECISION);

	// Create threads. Split Maclaurin series calculations
	// equally among the threads.
	for(i = 0; i < NUM_THREADS; i++) {
		thread_id[i] = (int *) malloc(sizeof(int));
		*thread_id[i] = i;
		mpf_init(thread_total[i]);
		if(pthread_create(&thread_pool[i], NULL, func, (void *)thread_id[i]))
			return PI_ERROR;
	}

	// Wait for thread execution
	for(i = 0; i < NUM_THREADS; i++) {
		pthread_join(thread_pool[i], NULL);
		mpf_add(*total, *total, thread_total[i]);
		mpf_clear(thread_total[i]);
		free(thread_id[i]);
	}
	return 0;
}

// Approximates arctan(1/5)
void *calc_arctan_one_five(void *i) {
	calc_arctan(*((int *) i), 1, 5);
}

// Approximates arctan(1/239)
void *calc_arctan_one_twothreenine(void *i) {
	calc_arctan(*((int *) i), 1, 239);
}

// A helper function that finds an approximation of
// arctan(num/den).
void calc_arctan(int threadid, int num, int den) {
	mpf_t z, each;
	int i;

	mpf_init2(z, PI_PRECISION);
	mpf_set_ui(z, num);
	mpf_div_ui(z, z, den);
	mpf_init2(each, PI_PRECISION);

	for(i = threadid; i < ITERATIONS; i += NUM_THREADS) {
		mpf_pow_ui(each, z, 2*i + 1);
		mpf_div_ui(each, each, 2*i + 1);
		if(i%2)
			mpf_sub(thread_total[threadid], thread_total[threadid], each);
		else
			mpf_add(thread_total[threadid], thread_total[threadid], each);
	}

	mpf_clear(z);
	mpf_clear(each);
	pthread_exit(NULL);
}

