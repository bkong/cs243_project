#Makefile for cs243 benchmarking project

INPUT_FOLDER = src
OUTPUT_NAME = benchmark_suite.exe

#todo: different gcc flags will output different output names

all:
    gcc -o $(OUTPUT_NAME) $(INPUT_FOLDER)/benchmark.c $(INPUT_FOLDER)/benchmarks/*.c $(INPUT_FOLDER)/include/*

clean:
    rm $(OUTPUT_NAME)
