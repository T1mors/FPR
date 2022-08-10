#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/** Calculates the mean of the input array.
 *
 *  \param data array of doubles to average
 *  \param n number of entries in data
 */
double mean(double *data, int n)
{
    double m = 0.0;
    for(int i=0; i<n; ++i)
    {
        m += data[i];
    }
    return m/n;
}

/** Calculates the variance of the input array.
 *
 *  \param data array of doubles to evaluate
 *  \param n number of entries in data
 */
double variance(double *data, int n)
{
    double var = 0.0;
    double m = mean(data, n);
    for(int i=0; i<n; ++i)
    {
        double diff = data[i] - m;
        var += diff * diff;
    }
    return var/n;
}

/** Calculates the standard deviation of the input array.
 *
 *  \param data array of doubles to evaluate
 *  \param n number of entries in data
 */
double sdev(double *data, int n)
{

    return sqrt(variance(data, n));
}

/** Generates a uniformly distributed random number from [0,1)
 */
double uniform()
{
    return (double) rand() / RAND_MAX;
}

/** Calculates the mean and error by bootstrap resampling.
 *
 *  \param data             array of doubles to average
 *  \param n                number of entries in data
 *  \param mean_p  [out]    resulting mean value
 *  \param error_p [out]    resulting error of the mean
 *  \param f                function pointer to the reduction function
 */
void bootstrap(double *data, int n, double *mean_p, double *error_p, double(*f)(double *, int))
{
    #define NUM_RESAMPLE 100
    double resampled[NUM_RESAMPLE];
    double *tmp_sample = (double*) malloc(n*sizeof(double));
    int i;
    int j;
    int idx;	
    //==================================================================
    /* YOUR CODE HERE */
    for (i=0; i<NUM_RESAMPLE; i++){
	    for (j=0; j<n; j++){
		idx = (int) n*uniform();
        	tmp_sample[j] = data[idx];
    	}
	resampled[i] = f(tmp_sample, n);
    }
    //==================================================================

    *mean_p = mean(resampled, NUM_RESAMPLE);
    *error_p = sdev(resampled, NUM_RESAMPLE);
    free(tmp_sample);
}

void bootstrap_mean(double *data, int n, double *mean_p, double *error_p)
{
    bootstrap(data, n, mean_p, error_p, &mean);
}
void bootstrap_var(double *data, int n, double *mean_p, double *error_p)
{
    bootstrap(data, n, mean_p, error_p, &variance);
}
