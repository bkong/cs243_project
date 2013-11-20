/***********************************************
 * Benchmark.c                                 *
 * Benchmarking suite for CS 241               *
 * Group members:                              *
 *   Tyler Howe, Jacob Heller, & Bailey Kong   *
 *                                             *
 * Use: Each benchmark file uses the           *
 *   reg_benchmark() to register that file's   *
 *   benchmark for use in the suite.  The      *
 *   user chooses a suite to run either on the *
 *   command line or at runtime via the        *
 *   terminal.                                 *
 **********************************************/
 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>

#define MAX_BENCHMARKS 10
#define BENCHMARK_FOLDER "./src/benchmarks/"

static const char* EXT = ".bnch";

typedef struct {
    int num;
    char *names[10];
} benchmark_set;

static void __run_benchmark(char *benchmark, int iterations);
static int __is_benchmark_file(const char *name);
static benchmark_set *__get_benchmarks();

extern int errno;

int main(int argc, char *argv[])
{
    benchmark_set *benchmarks = __get_benchmarks();
    char bench_str[20];
    int iterations = 0;
    const char *SEP = ",";
    
    if (argc <= 1)
    {
        // User needs to select which benchmark to run
        printf("Choose which benchmark to run:\n");
        int i;
        for (i=0; i<MAX_BENCHMARKS; i++)
        {
            if (benchmarks[i].num)
            {
                printf("%d. %s\n", i+1, benchmarks[i].names[0]);
            }
        }
        printf("\nEnter number(s): ");
        scanf("%s", bench_str);
    }
    else if (argc == 2)
    {
        // User entered which benchmark as command line arg
        strcpy(bench_str, argv[1]);
    }
    else
    {
        // User entered which benchmark as command line arg
        strcpy(bench_str, argv[1]);
        sscanf(argv[2], "%d", &iterations);
    }
    
    if (!iterations)
    {
        printf("\nEnter number of iterations: ");
        scanf("%d", &iterations);
    }
    
    char* bench;
    char* benches = bench_str; // change to pointer to avoid warning
    // loop through all of the benchmarks, separated by commas
    while((bench = strsep(&benches, SEP)) != NULL) 
    {
        int benchnum = atoi(bench)-1;
        int i;
        for (i=0; i<benchmarks[benchnum].num; i++)
        {
            printf("\n");
            __run_benchmark(benchmarks[benchnum].names[i], iterations);
        }
    }
    return 0;
}

static benchmark_set *__get_benchmarks()
{
    int i;
    benchmark_set *benchmarks = malloc(sizeof(benchmark_set) * MAX_BENCHMARKS);
    for (i=0; i<MAX_BENCHMARKS; i++)
    {
        benchmarks[i].num = 0;
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(BENCHMARK_FOLDER);

    if (d > 0)
    {
        while ((dir = readdir(d)) != NULL)
        {
            size_t ext_len = strlen(EXT);
            char *name = dir->d_name; // get the filename
            if (__is_benchmark_file(name))
            {
                size_t len = strlen(name);
                char bench_name[len -ext_len+1];
                strncpy(bench_name, name, len-ext_len); // remove the '.bnch' part
                bench_name[len-ext_len] = '\0'; // null terminate the string

                char *sep = strstr(name, "_");
                char bench_num[sep-name+1];
                strncpy(bench_num, name, sep-name); // grab only the benchmark number
                bench_num[sep-name] = '\0'; // null terminate the string
                int num = atoi(bench_num)-1;

                if (num >= MAX_BENCHMARKS) continue;

                benchmarks[num].names[benchmarks[num].num] = malloc(sizeof(char)*strlen(bench_name)+1);
                strcpy(benchmarks[num].names[benchmarks[num].num], bench_name); // add the file to the benchmark set
                benchmarks[num].num++;
            }
        }
        closedir(d);
    }
    return benchmarks;
}

static int __is_benchmark_file(const char *name)
{
    if (name != NULL)
    {
        size_t size = strlen(name); // size of filename
        size_t ext_size = strlen(EXT); // size of extension
        int i;
        
        if (size >= ext_size)
        {
            for(i=0; i<ext_size; i++)
            {
                // does current char == extension?
                if (name[size-ext_size+i] != EXT[i])
                {
                    return 0;
                }
            }
            return 1;
        }
    }
    
    return 0;
}


/* private function to get clock cycle count */
static uint64_t __get_clk()
{
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

/* private function to run a benchmark */
static void __run_benchmark(char *benchmark, int iterations)
{
    int i;
    uint64_t start, end;
    unsigned long long int total_clk = 0L; // Total # of clock cycles
    static char *argv[] = {"", NULL};
    char path[strlen(BENCHMARK_FOLDER) + strlen(benchmark) + strlen(EXT) + 1];
    strcpy(path, BENCHMARK_FOLDER);
    strcat(path, benchmark);
    strcat(path, EXT);
    printf("Running benchmark \"%s\" for %d iterations...\n", benchmark, iterations);
    
    for (i=0;i<iterations;i++)
    {
        start = __get_clk();
        // Spawn child process
        pid_t pid = fork();
        if (pid == 0)   // Child process
        {
            if(execv(path, argv)<0);{
                    printf("Errno is %d\n", errno);
            }
            exit(-1);
        }
        else // Parent Process
        {
            waitpid(pid, 0, 0); // Wait for child
        }
        end = __get_clk();
        
        total_clk += ((end - start) / 1000);
    }
    printf("Benchmark Complete.\n");
    printf("Average number of clock cycles for %d iterations: %f million\n", 
    iterations, 
    (total_clk / iterations)/1000.0);
}


