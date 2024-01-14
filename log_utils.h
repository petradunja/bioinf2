
#ifndef logUtils_h
#define logUtils_h

#include <cmath>  

using namespace std;

double compute_exp(double x){
	if (x == - INFINITY){
		return 0;
	}
	return exp(x);
	
}

double log_sum(double x, double y){
	if (x == -INFINITY || y == -INFINITY){
		if (x == -INFINITY){
			return y;
		} 
		return x;
	} else if (x > y){
		return x + log(1 + compute_exp(y-x));
	} else {
		return y + log(1 + compute_exp(x-y));
	}
}

double log_product(double x, double y){
	if (x == -INFINITY || y == -INFINITY){
		return -INFINITY;
	}
	return x + y;
	
}
#endif 