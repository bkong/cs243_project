#Makefile for cs243 benchmarking project

INPUT_FOLDER = src
BENCHMARKS_FOLDER = src/benchmarks
OUTPUT_NAME = benchmark_suite


#todo: different gcc flags will output different output names

default:
	gcc -o $(OUTPUT_NAME) $(INPUT_FOLDER)/benchmark.c
	@cd $(BENCHMARKS_FOLDER) ; make -s all

clean:
	@if [ -a $(OUTPUT_NAME) ] ; \
	then \
		echo rm $(OUTPUT_NAME) ; \
		rm $(OUTPUT_NAME) ; \
	fi;
	@cd $(BENCHMARKS_FOLDER) ; make -s clean

