#pragma once

//Top level Functions
float my_sqrt(float x);
float taylor_expansion(float x);
int find_companion(float n);
void tweak(float n, float &sqrt_n_approx);

//Primary Helper Functions
int get_digits(float n);
float taylor_coeff(int n, int sqrt_a);
bool is_infinite(float x);

//Secondary Helper Functions
float my_abs(float x);
int power(int base, int exp);
float power(float base, int exp);


