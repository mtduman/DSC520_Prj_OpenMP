#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <omp.h>

double sample_interval(double a, double b) {     // Random Numbers generation beteeen (a,b)
    double x = ((double) rand())/((double) RAND_MAX);
    return a + (b-a)*x ;
}

double funcL(double array[], int rows) {          // Function L(x1 +x2 +...+xj)
    double L = 0.0;
    for (int j=0; j < rows; ++j) {
        L += array[j];
    }
    return L;
}

int main (int argc, char **argv) {
    srand(time(NULL));
    const int dim = 10;
    int threads_tot = atoi( argv[1] );
    int threads_check;
    unsigned long long int N = atoll( argv[2] );  // atoll for long numbers: convert command-line input to N   = number of points

    double xdim[dim];
    double a = -5.0, b =  5.0 ;
//    long int V = 1;                           // For Volume,but ignored when applying Monte Carlo approximation
//    for (int m=1; m <= dim; ++m) { V *= (b-a);  }

    int pow_max = 0;
    do {
        pow_max += 1;
    } while (pow(4, pow_max)  < N);         // Find out closest power of 4 to input N value
    
    double approx_integral[pow_max]  ;      // Create array to save integral values of each power of 4 iterations
    double integral      = 0.0;
    
    int pow_count = 2;                      // For first number  16 = 4^2
    unsigned long long int n_start =  1;
    unsigned long long int n_stop  = 16;
    bool working_loop = true;
    
    long long int n;
    do {
        if (n_stop > N ) {                                            // Check n_stop if it is bigger than N
            n_stop = N;                                               // change it with N for loop
            working_loop = false;                                     // exit from while loop end of the circle
        }

        #pragma omp parallel for private(xdim) reduction(+: integral) num_threads(threads_tot)  // PARALLEL COMPUTING START
        for ( n = n_start ; n <= n_stop ; n+=1) {                  // Iteration between each power of 4 , for-loop
            int i;
            for ( i=0; i < dim; ++i) {
                xdim[i] =sample_interval( a,b );                      // Genarate 'dim' number between (a,b)
            }
            integral += funcL( xdim, dim );                           // Apply func L and add it up integral veriable
//            threads_check = omp_get_num_threads();
        }                                                             // PARALLEL COMPUTING END

        approx_integral[pow_count] = integral/ (double)n_stop ;       // Calculate approx integral for 'pow_count' iteration
        pow_count += 1;
        n_start    = n_stop+1;                                        // prepare n_start for next for loop
        n_stop     *= 4 ;                                             // prepare n_stop for next for loop by multipl 4
    } while ( working_loop );

//    printf("In a parallel region: %d threads has been used \n",threads_check, omp_get_num_threads() ,   omp_get_thread_num());

    n_stop = 4;

    for (int i=2; i < pow_max ; ++i) {                                // for loop to print approx MC integral and error for power of 4
        n_stop *= 4;
        printf(" %16llu,% 3.16e,%3.16e,%3.16e \n",
               n_stop,
               approx_integral[i],
               fabs(approx_integral[i] - approx_integral[i+1]),
               fabs(approx_integral[i] - 0.0) );
    }
    printf(" %16llu,% 3.16e,%3.16e,%3.16e  \n" ,                     // print approx MC integral and error for N iteration
           N,\
           approx_integral[pow_max],
           fabs(approx_integral[pow_max] - approx_integral[pow_max-1]),
           fabs(approx_integral[pow_max] - 0.0) );

    return 0;
    
}
