#include "header.h"

static double *results = nullptr;


void reduce_sum(int p){
	static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
	static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
	static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
	static int t_in = 0, t_out = 0;
	if(p <= 1){
		return;
	}
	pthread_mutex_lock(&m);
	t_in++;
	if(t_in >= p){
		t_out = 0;
		pthread_cond_broadcast(&c_in);
	}
	else{
		while(t_in < p){
			pthread_cond_wait(&c_in, &m);
		}
	}
	t_out++;
	if(t_out >= p){
		t_in = 0;
		pthread_cond_broadcast(&c_out);
	}
	else{
		while(t_out < p){
			pthread_cond_wait(&c_out, &m);
		}
	}
	pthread_mutex_unlock(&m);
	return;
}


double reduce_sum(int p, double val){
	static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
	static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
	static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
	static int t_in = 0, t_out = 0;
	static double ans = 0;
	if(p <= 1){
		return val;
	}
	pthread_mutex_lock(&mu);
	t_in++;
	if(val > ans){
		ans += val;
	}
	if(t_in >= p){
		t_out = 0;
		pthread_cond_broadcast(&c_in);
	}
	else{
		while(t_in < p){
			pthread_cond_wait(&c_in, &mu);
		}
	}
	val = ans;
	t_out++;
	if(t_out >= p){
		t_in = 0;
		ans = 0;
		pthread_cond_broadcast(&c_out);
	}
	else{
		while(t_out < p){
			pthread_cond_wait(&c_out, &mu);
		}
	}
	pthread_mutex_unlock(&mu);
	return val;
}


double reduce_sum_mx(int p, double val){
	static pthread_mutex_t mu = PTHREAD_MUTEX_INITIALIZER;
	static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
	static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
	static int t_in = 0, t_out = 0;
	static double ans = 0;
	if(p <= 1){
		return val;
	}
	pthread_mutex_lock(&mu);
	t_in++;
	if(val > ans){
		ans = val;
	}
	if(t_in >= p){
		t_out = 0;
		pthread_cond_broadcast(&c_in);
	}
	else{
		while(t_in < p){
			pthread_cond_wait(&c_in, &mu);
		}
	}
	val = ans;
	t_out++;
	if(t_out >= p){
		t_in = 0;
		ans = 0;
		pthread_cond_broadcast(&c_out);
	}
	else{
		while(t_out < p){
			pthread_cond_wait(&c_out, &mu);
		}
	}
	pthread_mutex_unlock(&mu);
	return val;
}


int init_reduce_sum(int p){
	results = new double[p];
	if(results == nullptr) return -1;
	return 0;
}


int delete_reduce_sum(){
	if(results == nullptr){return -1;}
	delete[] results;
	return 0;
}


double reduce_sum_det(int p, int k, double s){
	double sum = 0; int l;
	results[k] = s;
	reduce_sum(p);
	for(l = 0; l < p; ++l){
		sum+=results[l];
	}
	reduce_sum(p);
	return sum;
}
