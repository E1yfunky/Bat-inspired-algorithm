#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <vector>

using namespace std;

class Population;
class Gplot;
int m=0, m_p=0, n, raz;
double minf, f_p, *x_p;
double  wmin, wmax;

struct Parametrs {
  int razmernost;
  int bat_number;
  int iteration_number;
  double x_min;
  double x_max;
  int ran;

  Parametrs(std::string ba): razmernost(2), bat_number(10), iteration_number(20) {
   std::fstream config(ba);
   while (!config.eof()) {
    string parametr;
    config >> parametr;
    if (parametr == "razmernost:") {
     config >> razmernost;
    } else if (parametr == "bat_number:") {
     config >> bat_number;
    } else if (parametr == "iteration_number:") {
     config >> iteration_number;
    } else if (parametr == "x:") {
     config >> x_min >> x_max;
    } else if (parametr == "srand:") {
     config >> ran;
    } 
   }
  }
};

class Fitfunction {
 public:
  Fitfunction () {};
  
  Fitfunction (double (*f)(double* x, int raz), int x_min, int x_max) {
   fitness = f;
   lim.push_back(x_min);
   lim.push_back(x_max);
  };
  double calculate(double *x, int) {
   return (*fitness)(x, raz);
  };
  int getxmin() { return lim[0]; };
  int getxmax() { return lim[1]; };
 
 protected:
  double (*fitness)(double*, int);
  std::vector<int> lim;
};

class Bat {
  double *x = new double [32];
  double *v = new double [32];
  double f;
  double w;
  double r;
  double a;
 public:
  Bat () {};
  ~Bat() { delete[] x; delete[] v; };
  void init(Fitfunction fi) { ffunction=fi;  
                              //cout << endl;
       for (int i=0; i<raz; i++) {
        x[i] = ((double) (rand() % ((ffunction.getxmax() - ffunction.getxmin()) * 10000))) / 10000 + ffunction.getxmin();
            v[i] = 0;
            //cout << x[i] << endl;
           }
       w=((double) (rand() % 1000)) / 1000;
           r=((double) (rand() % 500)) / 1000 + 0.5;
       a=((double) (rand() % 999)) / 1000 + 0.01;
       f=ffunction.calculate(x, raz);};
  void MigrationB(Bat&);
  void EvolutionB(int);
  friend class Population;
  friend class Gplot;

 protected:
  Fitfunction ffunction;
};

class Population {
 public:
  Bat *b = new Bat[n];
  Population () {};
  ~Population() { delete[] b; };
  void init (Fitfunction f) {
   x_p = new double [32];
   ffunction=f;
   for (int i=0; i<n; i++) {
     b[i].init(ffunction);
   }
  }
  double get_r(Bat& b) { return b.r; };
  double get_a(Bat& b) { return b.a; };
  double get_min(Bat& b) { return b.f; };
  void RandSol(Bat& b) { for (int i = 0; i < raz; i++)
                 x_p[i] = b.x[i] + (((double) (rand() % 100)) / 1000);
             f_p = ffunction.calculate(x_p, raz); };
  void accept(Bat& b) { for (int i = 0; i < raz; i++) 
			 b.x[i] = x_p[i];
			b.f = f_p;
			minf = f_p; };
  void print(int t, double minimum) { cout << " F(X)=" << minimum << " t=" << t << "\n"; };
  void FindMinMax();
  void Migration();
  void Evolution(int);
  void GSearch();
  void Search(Bat&, int);

 protected:
  Fitfunction ffunction;
};

class Gplot {
 public:
  fstream pf;
  fstream pfp;
  double tem[2][3000];
  double temp[2][5000];
  Gplot () {  pf.open("ba.txt", fstream::out | fstream::trunc);
              pfp.open("bas.txt", fstream::out | fstream::trunc);
  };
  ~Gplot() {
    pf.close();
    pfp.close();
  }
  void results(const Bat& Z, int i) {
      temp[0][i-1]=i;
      temp[1][i-1]=Z.f;
  };


  void Tminimum(int n){
      int i=0;
      while(i != n){
          tem[0][i] = temp[0][i];
          tem[1][i] = temp[1][i];
          i++;
      }
  }

  void minimum(int n) {
      pfp << "\n\n";
      int i=0;
      while(i != n){
          pfp << tem[0][i] << " " << tem[1][i] << endl;
          i++;
      }
  }

  void Sresults (double minimum, int i) {
   pf << i << " " << minimum << endl;
  };
};

void Bat::MigrationB(Bat& B) {
 w = wmin + (((double) (rand() % 1000) / 1000)) * (wmax - wmin);
 for (int i = 0; i < raz; i++) {
  v[i] = v[i] + w * (x[i] - B.x[i]);
  x[i] = x[i] - v[i];
  if (x[i]> ffunction.getxmax()) {
      for (int j = 0; j < raz; j++) {
          x[j] = ((double) (rand() % ((ffunction.getxmax() - ffunction.getxmin()) * 1000))) / 1000 + ffunction.getxmin();
              v[j] = 0;
      }
      break;
  }
 }
 f = ffunction.calculate(x, raz);
}

void Bat::EvolutionB(int t) {
  a=0.9 * a;
  r=r*(1 - exp(-0.9*t));
}

void Population::FindMinMax() {
 wmin = wmax = b[0].w;
 for (int i=1; i<n;i++) {
  if (b[i].w<wmin)
   wmin = b[i].w;
  if (b[i].w>wmax)
   wmax = b[i].w;
 }
}

void Population::Migration() {
 for (int i=0; i<n; i++) {
  if (i!=m)
   b[i].MigrationB(b[m]);
 }
}

void Population::Evolution(int t) {
   b[m].EvolutionB(t);
}

void Population::GSearch() {
 minf=b[0].f;
 m=0;

 for (int i=1; i<n; i++) {
  if (b[i].f<minf) {
    minf = b[i].f;
    m = i;
   } 
}
}

void Population::Search(Bat& Z, int t) {
 double fi = Z.f, x_s[raz], Ao=0;
 double e;
 int i, j;

 for (i=0;i<n; i++)
  Ao += Z.a;
 Ao /= n;

 i = 0;

 while ((fi >= Z.f) && (i++ < 50)) {
  e = (((double) (rand() % 2000) / 1000)) - 1;

  for (j=0; j<raz; j++) {
   x_s[j] = Z.x[j] - e * Ao;
  }

  fi = ffunction.calculate(x_s, raz);
 } 

 if (i<50) {
  Z.f = fi;
  for (j=0; j<raz; j++) 
   Z.x[j] = x_s[j]; 
 }
}
