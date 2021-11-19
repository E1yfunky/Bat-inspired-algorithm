#include <stdio.h>
#include <cmath>

double Fifunction(double* x, int raz) {
  long double F=0, F1=0;
  double F2=0;
  int i=0;
  while (i < (raz)){
    F1+=x[i]*x[i]/raz;
    F2+=cos(2*3.1415*x[i]);
    //F = F + (1 - x[i]) * (1 - x[i]) + 100 * (x[i+1] - x[i] * x[i]) * (x[i+1] - x[i] * x[i]);
   i++; }
  F = 20-20*exp(-0.2*sqrt(F1))-exp(F2/raz)+exp(1);
  return F; }
