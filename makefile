#Makefile for cs243 benchmarking project

INPUT_FOLDER = ./src
BENCHMARKS_FOLDER = ./src/benchmarks
OUTPUT_NAME = benchmark_suite
OPT = O0
RESULTS_FILE = ./benchmark_results.csv

#todo: different gcc flags will output different output names
 
all:
	gcc -o $(OUTPUT_NAME) $(INPUT_FOLDER)/benchmark.c
	gcc -o timeIt $(INPUT_FOLDER)/timeIt.c;
	@export OPT
	@cd $(BENCHMARKS_FOLDER) ; make -s all

clean:
	@if [ -a $(OUTPUT_NAME) ] ; \
	then \
		echo rm $(OUTPUT_NAME) ; \
		rm $(OUTPUT_NAME) ; \
	fi;

	@if [ -a timeIt ] ; \
	then \
		echo rm timeIt ; \
		rm timeIt ; \
	fi;

	@if [ -a $(RESULTS_FILE) ] ; \
	then \
		echo rm $(RESULTS_FILE) ; \
		rm $(RESULTS_FILE) ; \
	fi;

	@cd $(BENCHMARKS_FOLDER) ; make -s clean
