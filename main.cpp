#include <cmath>
#include <cstdio>
#include <omp.h>
#include "library.h"
#include <iostream>
#include <time.h>
#include <iomanip>
using namespace std;

const int nTrials = 10;
const int skipTrials = 3; // Skip first iteration as warm-up

double ComputeIntegral(const int n, const double a, const double b);

void Stats(double & x, double & dx) {
  x  /= (double)(nTrials - skipTrials);
  dx  = sqrt(dx/double(nTrials - skipTrials) - x*x);
}

double Accuracy(const double I, const double a, const double b) {
  const double I0 = InverseDerivative(b) - InverseDerivative(a);
  const double error = (I - I0)*(I - I0);
  const double norm  = (I + I0)*(I + I0);
  return error/norm;
}
double wtime ( )
{
  double value;

  value = ( double ) clock ( ) 
        / ( double ) CLOCKS_PER_SEC;

  return value;
}
int main(int argc, char** argv) {

  //const int n = 1000*1000*1000;
  constexpr long kIteration = 1024*1024;
  constexpr long kScale = 500;
  constexpr int n = kIteration * kScale;
  cout << "\n\033[1mNumerical integration with n="<<n<<"\033[0m\n";
  cout <<"\033[1mStep\t Time, ms\t GSteps/s\t Accuracy\033[0m\n";  
    fflush(stdout);

  double t, dt, f, df;

  for (int iTrial = 1; iTrial <= nTrials; iTrial++) {

    const double a = double(iTrial - 1);
    const double b = double(iTrial + 1);

    const double t0 = wtime();
    const double I = ComputeIntegral(n, a, b);
    const double t1 = wtime();
    const double ts   = t1-t0; // time in seconds
    const double tms  = ts*1.0e3; // time in milliseconds
    const double fpps = double(n)*1e-9/ts; // performance in steps/s

    if (iTrial > skipTrials) { // Collect statistics
      t  += tms; 
      dt += tms*tms;
      f  += fpps;
      df += fpps*fpps;
    }

    const double acc = Accuracy(I, a, b);

    // Output performance
    cout <<iTrial <<"\t" << setprecision(3) << tms <<"\t\t" << setprecision(3)<< fpps <<"\t\t" 
         <<setprecision(3)<<acc<<"" <<(iTrial<=skipTrials?"*":"")<<"\n";
	      fflush(stdout);
  }

  Stats(t, dt);  
  Stats(f, df);  
  cout<<"-----------------------------------------------------\n";
  cout<<"Average performance: Time ";
  cout<<"\033[42m"<<fixed<<setprecision(1) <<t<<"ms +-"<<setprecision(1)       
      <<dt<<"\033[0m\tAccuracy\033[42m  "<<setprecision(1) <<f<<"+-"<< setprecision(1)            
      <<df<<"\033[0m\n";
  cout<<"-----------------------------------------------------\n";
  cout<<"* - warm-up, not included in average\n\n";

}
