#Makefile for cs243 benchmarking project

BENCHMARKS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(BENCHMARKS))

#todo: different gcc flags will output different output names

all: $(PROGS)

%: %.c
	@echo gcc -O0 -std=c99 -o $@ $<
	@gcc -O0 -std=c99 -o $@ $<

clean:
	@if [ -a $(OUTPUT_NAME) ] ; \
	then \
		echo rm -f $(PROGS) ; \
		rm -f $(PROGS) ; \
	fi;
