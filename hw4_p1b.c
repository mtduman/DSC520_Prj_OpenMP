#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

double sample_interval(double a, double b) {
    double x = ((double) rand())/((double) RAND_MAX);
    return (b-a)*x + a;
}

double funcL(double array[], int rows) {
    double L = 0.0;
    for (int j=0; j < rows; ++j) {
        L += array[j];
    }
    return L;
}

int main (int argc, char **argv) {
    srand(time(NULL));
    int dim = atoi( argv[1] );                   // convert command-line input to dim = number of dimensions
    unsigned long long int N = atoll( argv[2] ); // convert command-line input to N   = number of points

    double xdim[dim];
    double a = -5.0;
    double b =  5.0;
    long int V = 1;
    for (int m=1; m <= dim; ++m) {
        V *= (b-a);
    }
    
    double integral = 0;
    unsigned long long int p_pow = 16;
    
    for (unsigned long long int i=0; i < N; ++i) {
        for (int k=0; k < dim; ++k) {
            xdim[k] =sample_interval( a,b );
        }
        integral += funcL( xdim, dim );

        if ( i == p_pow ) {
            printf("Dim= %i, iterations= %10llu, Z= % 3.12e\n",
                   dim, i, (double)V * integral/ ( (double)i * (double)V) );
            p_pow *= 4;
        }
    }
    
    printf("Dim= %i, iterations= %10llu, Z= % 3.12e\n",
           dim, N, (double)V * integral/ ( (double)N * (double)V) );
    return 0;
    
}
