/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

/*
	RPC Server code
	The file contains 3 functions, one for each calculation of the menu
*/

#include "ask_rpc.h"

int *calc_inner_pr_1_svc(inputs *argp, struct svc_req *rqstp) // function for option 1, calculating the inner product of 2 given vectors (X,Y) and returning it
{
	static int result = 0;
	result = 0;
	for (int i = 0; i < argp->n; i++) // calculating inner product
		result += (argp->X.X_val[i]) * (argp->Y.Y_val[i]); // result = result + (X[i]*Y[i])

	return &result; // return inner product
}

averages *
find_avgs_1_svc(inputs *argp, struct svc_req *rqstp) // function for option 2, finding the average value of each vector and returning it in a 2-element array
{
	static averages result;

	result.avg.avg_len = 2;														// initialize the length of avg array
	result.avg.avg_val = (double *)malloc(result.avg.avg_len * sizeof(double)); // dynamic allocation for avg array

	// initializing sums
	double sumX = 0;
	double sumY = 0;

	for (int i = 0; i < argp->n; i++) // calculating sums of each vector
	{
		sumX = sumX + argp->X.X_val[i];
		sumY = sumY + argp->Y.Y_val[i];
	}

	// calculating average values and storing them in avg array
	result.avg.avg_val[0] = sumX / argp->n;
	result.avg.avg_val[1] = sumY / argp->n;

	return &result; // return struct with stored values
}

mul_arr *
calc_mul_1_svc(inputs *argp, struct svc_req *rqstp) // function for option 3, calculating the product of r*(X+Y) and returning the results a n-element array
{
	static mul_arr result;

	result.array.array_len = argp->n;													// initialize the length of array
	result.array.array_val = (double *)malloc(result.array.array_len * sizeof(double)); // dynamic allocation for array

	for (int i = 0; i < argp->n; i++) // calculating the product r with (X+Y) for every element and storing the result to array
		result.array.array_val[i] = argp->r * (argp->X.X_val[i] + argp->Y.Y_val[i]);

	return &result; // return struct with stored values
}