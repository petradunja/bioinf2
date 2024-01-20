
#ifndef logUtils_h
#define logUtils_h

#include <cmath>  
#include <limits>  

using namespace std;

double LOGZERO = -INFINITY;

double compute_log(double x){
	if (x==0){
		return LOGZERO;
	} else if (x>0){
		return log(x);
	}  
}

double compute_exp(double x){
	if (x == -INFINITY){
		return 0.00000001;
	}
	return exp(x);
}

double log_sum(double x, double y){
	if (x==LOGZERO || y==LOGZERO){
		if (x==LOGZERO){
			return y;
		} 
		return x;
	} else if (x > y){
		return x + compute_log(1 + compute_exp(y-x));
	} else {
		return y + compute_log(1 + compute_exp(x-y));
	}
}

double log_product(double x, double y){
	if (x==LOGZERO || y==LOGZERO){
		return LOGZERO;
	}
	return x + y;
}
#endif 