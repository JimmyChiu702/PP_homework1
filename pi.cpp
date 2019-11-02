#include <iostream>
#include <random>

using namespace std;

unsigned long long  number_of_cpu, number_of_tosses, number_in_circle;
pthread_mutex_t mutex;

void* toss(void*);

int main(int argc, char* argv[]) {
    clock_t start, end;
    start = clock();

    number_of_cpu = atoi(argv[1]);
    number_of_tosses = atoi(argv[2]);
    unsigned long long number_of_tosses_each = number_of_tosses/number_of_cpu;

    pthread_t* threads = (pthread_t*) malloc(number_of_cpu*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    long thread = 0;
    for (thread = 0; thread < number_of_cpu; thread++) {
        pthread_create(&threads[thread], NULL, toss, (void*)number_of_tosses_each);
    }
    for (thread = 0; thread < number_of_cpu; thread++) {
        pthread_join(threads[thread], NULL);
    }
    pthread_mutex_destroy(&mutex);
    free(threads);

    double pi_estimate = (4.0*number_in_circle)/number_of_tosses;
    end = clock();
    cout << "Estimate of pi: " << pi_estimate << endl;
    cout << "Elapsed time: " << (end-start) << endl;

    return 0;
}

void* toss(void* num_of_tosses_each) {
    unsigned long tosses = (unsigned long) num_of_tosses_each;
    double x, y;
    unsigned long sum;

    unsigned int seed = time(NULL);
    for (unsigned long t=0; t<tosses; t++) {
        x = (1.0*rand_r(&seed))/RAND_MAX;
        y = (1.0*rand_r(&seed))/RAND_MAX;
        if (x*x+y*y <= 1) {
            sum++;
        }
    }
    pthread_mutex_lock(&mutex);
    number_in_circle += sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}