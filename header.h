#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <pthread.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
using namespace std;
struct Args{
	double a, b, c, d, eps, t1 = 0, t2 = 0;
	int nx, ny, kk, k, m, p, it = 0;
	double* A, *B, *x, *u, *v, *r, *temp;
	size_t* I;
	bool stat;
	double r1 = -1, r2 = -1, r3 = -1, r4 = -1;
	Args() = default;
	~Args() = default;
};

void err_output(char* str);
double get_cpu_time();
double get_full_time();
int read_input(int* temp, double* ttemp, char** argv);
double f(int k, double x, double y);
void* thread_f(void* arg);
double scalar_product(size_t n, double *x, double *y, int p, int kk);
int min_err_matr(size_t n, double *a, size_t *I, double *b, double *x, double *r, double *u, double *v, double *temp, double eps, int max_it, int p, int kk);
int min_msr_solve(size_t n, double *a, size_t *I, double *b, double *x, double *r, double *u, double *v, double *temp, double eps, int max_it, int max_steps, int p, int kk);
void mult_sub_vector(size_t n, double *x, double *y, double t, int p, int kk);
void matr_mult_vector_msr(size_t n, double *a, size_t *I, double *x, double *y, int p, int kk);
void apply_precondition(size_t n, double* a, size_t* I, double *v, double *r, double *temp, int p, int kk);
int check_sym(int nx, int ny, size_t *I, double *a, double eps, int p, int kk);
double F_ij(int nx, int ny, double hx, double hy, double x0, double y0, int i, int j, int k);
int init_B(double* B, int nx, int ny, double hx, double hy, double x0, double y0, int k, int kk, int p);
int min_msr_solve1(size_t n, double *a, size_t *I, double *b, double *x, double *r, double *u, double *v, double* temp, double eps, int max_it, int max_steps, int p, int kk);


void ij2l(int nx, int i, int j, size_t &l);
void l2ij(int nx, int &i, int &j, size_t l);
void fill_a(int nx, int ny, double hx, double hy, size_t *I, double *a, int p, int kk);
size_t get_len_msr(int nx, int ny);
void fill_I(int nx, int ny, size_t *I, int kk, int p);
void reduce_sum(int p);
double reduce_sum_mx(int p, double val);
double reduce_sum(int p, double val);
int init_reduce_sum(int p);
int delete_reduce_sum();
double reduce_sum_det(int p, int k, double s);


double f_z(double x_, double y_, double a, double c, int nx, double hx, double hy, double *x);
double r1(size_t N, int nx, int ny, double a, double c, double hx, double hy, double *x, int k, int kk, int p);
double r2(size_t N, int nx, int ny, double a, double c, double hx, double hy, double *x, int k, int kk, int p);
double r3(size_t N, int nx, double a, double c, double hx, double hy, double *x, int k, int kk, int p);
double r4(size_t N, int nx, double a, double c, double hx, double hy, double *x, int k, int kk, int p);

#endif // HEADER_H_INCLUDED
