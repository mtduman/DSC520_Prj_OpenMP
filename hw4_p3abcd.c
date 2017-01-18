#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>

double sample_interval( double a, double b) {     // Random Numbers generation beteeen (a,b)
    double x = (double)rand() / RAND_MAX;
    return a + (b-a)*x ;
    }

double funcL(double array[] ) {
    double L = 0.0;
    for (int j=0; j < 9; ++j) {
        L += ( 1.0 - array[j] )*( 1.0 - array[j] ) + 100.0 * ( array[j+1] - array[j] * array[j] ) * ( array[j+1] - array[j] * array[j] ) ;
    }
    return exp( -1.0 * L );
}

int main (int argc, char **argv) {
    double start_time = omp_get_wtime();
    srand(time(NULL) * omp_get_thread_num() );
    const int dim =  10;
    int threads_tot = atoi( argv[1] );
    int threads_check;

    bool working_loop = true;
    unsigned long long int N = atoll( argv[2] );  // atoll for long numbers: convert command-line input to N   = number of points
    if ( isinf(N) || N == 0 || N == 705032704 || N == 9223372036854775807) {   //05032704 for unsigned int , 9223372036854775807 ull int
        printf(" N is too big ... INFINITY \n");
        working_loop = false;                                     // exit from while loop end of the circle
    }
    
    double xdim[dim];
    double a = -0.5, b =  0.5 ;
    double V = 1.0;                           // For Volume
    for (int m=1; m <= dim; ++m) {
        V *= (b-a);
    }
  
    int pow_max = 0;
    do {
        pow_max += 1;
    } while (pow(2, pow_max)  < N);         // Find out closest power of 4 to input N value
    
    double funcL_sum      = 0.0;
    
    int pow_count = 1;
    unsigned long long int n_start = 1;
    unsigned long long int n_stop  = 2;
    unsigned long long int n;

    while ( working_loop ) {
        if (n_stop >= N || isinf(n_stop) || n_stop == 0 ) {      // n_stop*4 =0 when it is inf   // Check n_stop if it is bigger than N
            n_stop = N;                                               // change it with N for loop
            working_loop = false;                                     // exit from while loop end of the circle
        }
        
        #pragma omp parallel for private(xdim) reduction(+: funcL_sum) num_threads(threads_tot)  // PARALLEL COMPUTING START
        for ( n = n_start ; n <= n_stop ; n+=1) {                  // Iteration between each power of 4 , for-loop
            int i;
            for ( i=0; i < dim; ++i) {
                xdim[i] = sample_interval( a,b );                      // Genarate 'dim' number between (a,b)
            }
            funcL_sum += funcL( xdim );                        // Apply func L and add it up integral veriable
            threads_check = omp_get_num_threads();
        }                                                             // PARALLEL COMPUTING END

//        printf(" %16llu,% 3.16e \n",
//               n_stop,
//               V * funcL_sum/ (double)n_stop );
        
        pow_count += 1;
        n_start    = n_stop+1;                                        // prepare n_start for next for loop
        n_stop    *= 2 ;                                              // prepare n_stop for next for loop by multipl 4

    }
    double finish_time = omp_get_wtime();
    printf("%llu, %i, %f ,%f ,%f\n", N, threads_check, (finish_time - start_time), start_time, finish_time);

    return 0;
}
