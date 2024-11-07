#include "header.h"

int main(int argc, char* argv[]){
	double a, b, c, d, eps;
	int nx, ny, k, m, p;
	if(argc < 11){
		err_output(argv[0]);
		return -1;
	}
	int* temp = (int*)malloc (sizeof (int) * 5);
	double* ttemp = (double*) malloc(sizeof(double)*5);
	if(read_input(temp, ttemp, argv)){
		free(temp); free(ttemp);
		err_output(argv[0]);
		cout << "-1" << endl;
		return -1;
	}
	a = ttemp[0]; b = ttemp[1]; c = ttemp[2]; d = ttemp[3]; eps = ttemp[4];
	nx = temp[0]; ny = temp[1]; k = temp[2]; m = temp[3]; p = temp[4];
	free(temp); free(ttemp);
	if((a > b) || (c > d) || (eps <= 0) || (nx <= 0) || (ny <= 0) || (k < 0) || (k > 7) || (m <=0) || (p <= 0) || init_reduce_sum(p)){
		err_output(argv[0]);
		cout << "-2" << endl;
		return -2;
	}
	
	
	size_t N = (size_t)(nx+1)*(ny+1);
	static double *A = nullptr, *B = nullptr, *x = nullptr, *u = nullptr, *v =nullptr, *r = nullptr, *temp1 = nullptr;
	size_t *I = nullptr;
	Args *args = nullptr;
	pthread_t *tid = nullptr;
	try{
		args = new Args[p];
		tid = new pthread_t[p];
		A = (double*)malloc (sizeof (double) * (get_len_msr(nx,ny)+1));
		I = (size_t*)malloc(sizeof(size_t)*(get_len_msr(nx,ny)+1));
		B = (double*)malloc(sizeof(double) * N);
		x = (double*)malloc(sizeof(double) * N);
		r = (double*)malloc(sizeof(double) * N);
		u = (double*)malloc(sizeof(double) * N);
		v = (double*)malloc(sizeof(double) * N);
		temp1 = (double*) malloc(sizeof(double)*N);
	}
	catch(std::bad_alloc&){
		err_output(argv[0]);
		delete_reduce_sum();
		return -3;
	}
	for(int i = 0; i < p; ++i){
		args[i].p = p;
		args[i].k = k;
		args[i].kk = i;
		args[i].a = a;
		args[i].b = b;
		args[i].c = c;
		args[i].d = d;
		args[i].nx = nx;
		args[i].ny = ny;
		args[i].m = m;
		args[i].eps = eps;
		args[i].stat = true;
		args[i].A = A;
		args[i].I = I;
		args[i].B = B;
		args[i].x = x;
		args[i].u = u;
		args[i].v = v;
		args[i].r = r;
		args[i].temp = temp1;
	}
	for(int i = 1; i < p; ++i){
		if(pthread_create(tid+i, 0, thread_f, args+i)){
			abort();
		}
	}
	thread_f(args+0);
	for(int i  = 1; i <p; ++i){
		pthread_join(tid[i], 0);
	}
	for(int i = 0; i < p; ++i){
		if(!args[i].stat){
			free(I); free(A); free(B); free(x); free(r); free(u); free(v); free(temp1);
			delete_reduce_sum();
			delete[] args; delete[] tid;
			err_output(argv[0]);
			return -5;
		}
	}
	printf ("%s : Task = %d R1 = %e R2 = %e R3 = %e R4 = %e T1 = %.2f T2 = %.2f It = %d E = %e K = %d Nx = %d Ny = %d P = %d\n",
	argv[0], 6, args[0].r1, args[0].r2, args[0].r3, args[0].r4, args[0].t1, args[0].t2, args[0].it, eps, k, nx, ny, p);
	delete_reduce_sum();
	free(I); free(A); free(B); free(x); free(r); free(u); free(v); free(temp1);
	delete[] args; delete[] tid;
	return 0;
}
