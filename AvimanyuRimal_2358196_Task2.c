// C-Programming Assignment Task-02(Calculating Pi using Leibniz formula and multithreading)
// Student Id : 2358196
// Name : Avimanyu Rimal

#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>  // Include the necessary header for dynamic memory allocation

#define NUM_THREADS 4                                                                                                                                                                                                                                                                                                                                                                                                            9

double *sum;  // Dynamically allocate the array to store partial sums calculated by each thread
int iterations_per_thread;

void *leibniz_series(void *threadid);

int main()
{
    int num_iterations;
    int num_threads;
    pthread_t *threads;  // Dynamically allocate the array to store thread IDs
    int error;
    int i;
    double pi;

    // Taking user input for the number of iterations
    printf("Enter the number of iterations: ");
    scanf("%d", &num_iterations);

    // Taking user input for the number of threads
    printf("Enter the number of threads: ");
    scanf("%d", &num_threads);

    iterations_per_thread = num_iterations / num_threads;

    // Dynamically allocate memory for threads and sum array
    threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    sum = (double *)malloc(num_threads * sizeof(double));

    // Check if memory allocation is successful
    if (threads == NULL || sum == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Creating threads
    for (i = 0; i < num_threads; i++)
    {
        // Creating threads with leibniz_series function and thread ID as argument
        error = pthread_create(&threads[i], NULL, leibniz_series, (void *)(intptr_t)i);
        if (error)
        {
            // Handling thread creation error
            printf("Error creating thread\n");
            return 1;
        }
    }

    // Waiting for threads to complete
    for (i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    pi = 0;

    // Accumulating the partial sums calculated by each thread
    for (i = 0; i < num_threads; i++)
        pi += sum[i];

    pi *= 4;

    // Printing the calculated value of Pi
    printf("The calculated value of Pi is: %f\n", pi);

    // Free dynamically allocated memory
    free(threads);
    free(sum);

    pthread_exit(NULL);

    return 0;
}

// Function for calculating the Leibniz series in each thread
void *leibniz_series(void *threadid)
{
    int start = (intptr_t)threadid * iterations_per_thread;
    int end = (intptr_t)threadid * iterations_per_thread + iterations_per_thread;

    int i;
    double result = 0;

    // Performing the Leibniz series calculation
    for (i = start; i <end; i++)
        result += pow(-1, i) / (2 * i + 1);

    // Storing the partial sum in the corresponding index of the sum array
    sum[(intptr_t)threadid] = result;

    pthread_exit(NULL);  // Exiting the thread
}
