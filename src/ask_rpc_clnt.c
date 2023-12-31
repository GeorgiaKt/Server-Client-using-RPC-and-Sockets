/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "ask_rpc.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
calc_inner_pr_1(inputs *argp, CLIENT *clnt)
{
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, CALC_INNER_PR,
		(xdrproc_t) xdr_inputs, (caddr_t) argp,
		(xdrproc_t) xdr_int, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

averages *
find_avgs_1(inputs *argp, CLIENT *clnt)
{
	static averages clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, FIND_AVGS,
		(xdrproc_t) xdr_inputs, (caddr_t) argp,
		(xdrproc_t) xdr_averages, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

mul_arr *
calc_mul_1(inputs *argp, CLIENT *clnt)
{
	static mul_arr clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, CALC_MUL,
		(xdrproc_t) xdr_inputs, (caddr_t) argp,
		(xdrproc_t) xdr_mul_arr, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
