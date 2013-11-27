#Makefile for cs243 benchmarking project

BENCHMARKS = $(wildcard *.c)
PROGS = $(patsubst %.c,%,$(BENCHMARKS))
OPT = O0
EXTENSION = .bnch

#todo: different gcc flags will output different output names

all: $(PROGS)

%: %.c
	@echo gcc -$(OPT) -std=c99 -o $@_$(OPT)$(EXTENSION) $<
	@gcc -$(OPT) -std=c99 -o $@_$(OPT)$(EXTENSION) $<

clean:
	@echo rm -f $(PROGS)
	@rm -f $(PROGS)
