#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>
struct drand48_data
{
    unsigned short int __x[3];  /* Current state.  */
    unsigned short int __old_x[3]; /* Old state.  */
    unsigned short int __c;     /* Additive const. in congruential formula.  */
    unsigned short int __init;  /* Flag for initializing.  */
    unsigned long long int __a; /* Factor in congruential formula.  */
};

void sample_drand48_r(const double a, const double b, const int dim, struct drand48_data *buffer, double *x) {
    double tmp;
    for(int i=0;i<dim;++i) {
        drand48_r(buffer,&tmp);
        x[i] = (b-a)*tmp+a;
    }
}

double sample_interval( double a, double b) {     // Random Numbers generation beteeen (a,b)
    double x = (double)rand() / RAND_MAX;
    return a + (b-a)*x ;
    }

double funcL(double array[], int rows ) {
    double L = 0.0;
    for (int j=0; j < 9; ++j) {
        L += exp( -1.0 *(1.0-array[j])*(1.0-array[j]) -100.0*(array[j+1]-array[j]*array[j]) * (array[j+1]-array[j]*array[j])  );
    }
    return L;
}

int main (int argc, char **argv) {
    srand(time(NULL));
    const int dim =  10;
    int threads_tot = atoi( argv[1] );
    int threads_check;

    bool working_loop = true;
    unsigned long long int N = atoll( argv[2] );  // atoll for long numbers: convert command-line input to N   = number of points
    if ( isinf(N) || N == 0 || N == 705032704) {      // n_stop*4 =0 when it is inf      // Check n_stop if it is bigger than N
        printf(" N is too big ... INFINITY \n");
        working_loop = false;                                     // exit from while loop end of the circle
    }
    
    
    double xdim[dim];
    double a = -5.0, b =  5.0 ;
    double V = 1.0;                           // For Volume,but ignored when applying Monte Carlo approximation
    for (int m=1; m <= dim; ++m) {
        V *= (b-a);
    }
    
    int pow_max = 0;
    do {
        pow_max += 1;
    } while (pow(4, pow_max)  < N);         // Find out closest power of 4 to input N value
    
    double integral      = 0.0;
    
    int pow_count = 2;                      // For first number  16 = 4^2
    unsigned long long int n_start = 1;
    unsigned long long int n_stop  = 16;
    
    unsigned long long int n;
    
    struct drand48_data drand_buf;
    memset(&drand_buf, '\0',sizeof(struct drand48_data));
    srand48_r(myseed, &drand_buf);
    
    while ( working_loop ) {
        if (n_stop > N || isinf(n_stop) || n_stop == 0 ) {      // n_stop*4 =0 when it is inf      // Check n_stop if it is bigger than N
            n_stop = N;                                               // change it with N for loop
            working_loop = false;                                     // exit from while loop end of the circle
        }
        
        #pragma omp parallel for private(xdim) reduction(+: integral) num_threads(threads_tot)  // PARALLEL COMPUTING START
        for ( n = n_start ; n <= n_stop ; n+=1) {                  // Iteration between each power of 4 , for-loop
            int i;
            for (unsigned int i=0; i<=dim; ++i) {
                sample_drand48_r(-5.,5., dim, &drand_buf, x);
            }
            for ( i=0; i < dim; ++i) {
                xdim[i] =sample_interval( a,b );                      // Genarate 'dim' number between (a,b)
            }
            printf(" x %u \n", x)
            integral += funcL( xdim, dim );                           // Apply func L and add it up integral veriable
        }                                                             // PARALLEL COMPUTING END
        printf(" %16llu,% 3.16e \n",
               n_stop,
               V * integral/ (double)n_stop);
        pow_count += 1;
        n_start    = n_stop+1;                                        // prepare n_start for next for loop
        n_stop     *= 4 ;                                             // prepare n_stop for next for loop by multipl 4

    }
    return 0;
}
