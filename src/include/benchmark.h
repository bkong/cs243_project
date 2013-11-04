/***********************************
 * Benchmark.h                     *
 * Header which contains functions *
 * for use in benchmarking         *
 **********************************/

 /*
 * To add a new benchmark:
 * 1) Increment # of benchmarks
 * 2) Add function prototype of benchmark function
 * 3) Add benchmark to array of benchmarks
 */
 
typedef struct
{
    char *name;    // Name of the benchmark
    void (*run)(); // Function pointer to benchmark
} Benchmark;

/* The total # of benchmarks */
#define NUM_BENCHMARKS 1

/* List of benchmark function prototypes */
void sleep_benchmark();
// add more benchmark functions here

/* Array of the benchmarks */
static const Benchmark BENCHMARKS[] =
{
    { "Sleep", sleep_benchmark },
    // Add more benchmarks here
};
