#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "plib.h"
#include "fitf.h"
#define EPS 0.0000001

int main(int argc, char** argv) {
 int i = 1, j = 0, im=0, cl001 = 0, cl1 = 0;
 double eps = 1, last_min, z, minimum = 100000000;

 Parametrs par(argv[1]);
 n=par.bat_number;
 srand(par.ran);

 Gplot Files;

 Fitfunction Fit(&Fifunction, par.x_min, par.x_max);

 Population A;
 raz = 1;
 while (raz<32) {
  raz*=2;
  minimum = 100000000;
  cl001 = 0;
  cl1 = 0;
  for (int g=0; g<100; g++) {
   i=1; j=0; eps=1;
   A.init(Fit);
   A.FindMinMax();
   A.GSearch();
   last_min = A.get_min(A.b[m]);

   while ((j < par.iteration_number) || (eps > EPS)) {
    A.Migration();

    if ((((double) (rand() % 1000)) / 1000) > A.get_r(A.b[m]))
      A.Search(A.b[m], m);

    for (int t=0; t<n; t++) {
      if (t!=m)
        A.Search(A.b[t], t);
    }

    A.GSearch();

    A.RandSol(A.b[m]);
    z = (((double) (rand() % 1000)) / 10000);
    if ((z < A.get_a(A.b[m])) && (f_p<minf)) {
      A.accept(A.b[m]);
      A.Evolution(i);
    }


    Files.results(A.b[m], i);
    i++;

    eps =last_min - A.get_min(A.b[m]);

    last_min = A.get_min(A.b[m]);
    if (eps <= 0)
      j++;
    else
      j = 0;
    A.FindMinMax();
   }
   if (A.get_min(A.b[m])<minimum) {
     minimum = A.get_min(A.b[m]);
     im = i-1;
     Files.Tminimum(im);
   }
   if (minimum<= 0.001){
       cl001 ++;
   }
   if (minimum<= 0.1){
       cl1 ++;
   }
  }
  cout << "In razmernost' " << raz;
  A.print(im, minimum);
  cout << "Where "<< cl001 << " < 0.001 & " << cl1 << " < 0.1" << endl;
  Files.Sresults(minimum,raz);
  Files.minimum(im);
 }
 delete[] x_p;
 return 0;
}
