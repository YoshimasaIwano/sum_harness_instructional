//
// (C) 2022-2023, E. Wes Bethel
// benchmark-* harness for running different versions of the sum study
//    over different problem sizes
//
// usage: no command line arguments
// set problem sizes, block sizes in the code below

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <string.h>

#define FLOP_PER_ELEM 2 

extern void setup(int64_t N, uint64_t A[]);
extern int64_t sum(int64_t N, uint64_t A[]);

/* The benchmarking program */
int main(int argc, char** argv) 
{
   std::cout << std::fixed << std::setprecision(2);

#define MAX_PROBLEM_SIZE 1 << 28  //  256M
   std::vector<int64_t> problem_sizes{ MAX_PROBLEM_SIZE >> 5, MAX_PROBLEM_SIZE >> 4, MAX_PROBLEM_SIZE >> 3, MAX_PROBLEM_SIZE >> 2, MAX_PROBLEM_SIZE >> 1, MAX_PROBLEM_SIZE};
   
   std::vector<uint64_t> A(MAX_PROBLEM_SIZE);

   int64_t t;
   int n_problems = problem_sizes.size();

   /* For each test size */
   for (int64_t n : problem_sizes) 
   {
      printf("Working on problem size N=%lld \n", n);

      // invoke user code to set up the problem
      setup(n, &A[0]);

      // insert your timer code here
      std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
      // invoke method to perform the sum
      t = sum(n, &A[0]);

      // insert your end timer code here, and print out elapsed time for this problem size
      std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();

      std::chrono::duration<double> elapsed = end_time - start_time;

      double time_sec = elapsed.count();
      double flops = n * FLOP_PER_ELEM / time_sec;
      double bandwidth = n * sizeof(uint64_t) / time_sec;
      double latency = time_sec / n;

      printf(" Elapsed time = %f seconds \n", elapsed.count());
      printf(" MFLOP/s: %.3lf\n", flops/1e6);
      printf(" Bandwidth utilization (GB/s): %.3lf\n", bandwidth/1e9);
      printf(" Estimated memory latency (sec): %.3lfe-9\n", latency*1e9);

      printf(" Sum result = %lld \n",t);

   } // end loop over problem sizes
}

// EOF
