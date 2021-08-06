#include <CL/sycl.hpp>
#include "library.h"
#include <cmath>
using namespace sycl;

double ComputeIntegral(const int n, const double a, const double b) {
  
    static constexpr size_t B = 128; // work-group size
    queue Q{property::queue::in_order{}};
    const double dx = (b - a)/n;
    double sum = 0.0;
    //# USM allocation using malloc_shared
    double *I = malloc_shared<double>(n, Q);
    Q.parallel_for(n, [=](id<1> i){
        double xip12 = a + dx*(double(i) + 0.5);
        double yip12 = 1.0/sqrt(xip12);
        double dI = yip12*dx;
        I[i]= dI;
    }).wait();

    for (int i=0; i<n; i++){
        sum += I[i]; 
    }
    //std::cout << "sum: " << sum << " ";
    free(I, Q);
    return sum;
}
