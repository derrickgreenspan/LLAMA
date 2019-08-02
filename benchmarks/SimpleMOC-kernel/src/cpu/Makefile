#===============================================================================
# User Options
#===============================================================================

COMPILER    = gnu 
OPENMP      = yes
OPTIMIZE    = yes 
DEBUG       = no
PROFILE     = no
PAPI        = no
MIC         = no
TABLE       = no

#===============================================================================
# Program name & source code list
#===============================================================================

program = SimpleMOC-kernel

source = \
main.c \
kernel.c \
init.c \
io.c \
papi.c

obj = $(source:.c=.o)

#===============================================================================
# Sets Flags
#===============================================================================

# Regular gcc Compiler
ifeq ($(COMPILER),gnu)
  CC = clang 
endif

# intel Compiler
ifeq ($(COMPILER),intel)
  CC = icc
endif

# IBM XL Compiler
ifeq ($(COMPILER),ibm)
  CC = mpicc
endif

# BG/Q gcc Cross-Compiler
ifeq ($(MACHINE),bluegene)
  CC = mpicc
endif

# Standard Flags
#CFLAGS := -std=gnu99

# Linker Flags
LDFLAGS = -lm

# Debug Flags
ifeq ($(DEBUG),yes)
  CFLAGS += -g
endif

# Profiling Flags
ifeq ($(PROFILE),yes)
  CFLAGS += -pg
endif

# MIC Flags
ifeq ($(MIC),yes)
  CFLAGS += -mmic
endif

# Optimization Flags
ifeq ($(OPTIMIZE),yes)
ifeq ($(COMPILER),gnu)
  CFLAGS += -Ofast -ffast-math -ftree-vectorize -msse2
endif
ifeq ($(COMPILER),intel)
ifeq ($(MIC), yes)
  CFLAGS += -O3 -ansi-alias -no-prec-div -DINTEL -fp-model fast=2
else
  CFLAGS += -O3 -xhost -ansi-alias -no-prec-div -DINTEL
endif
endif
ifeq ($(COMPILER),ibm)
  CFLAGS += -O5 -qhot -qsimd=auto -qalias=ansi:allptrs -qarch=qp -DIBM
endif
endif

# PAPI source (you may need to provide -I and -L pointing
# to PAPI depending on your installation
ifeq ($(PAPI),yes)
  CFLAGS += -DPAPI -I/soft/libraries/papi-5.3.0/include
  LDFLAGS += -L/soft/libraries/papi-5.3.0/lib -lpapi
  OPENMP = yes
endif

# OpenMP
ifeq ($(OPENMP),yes)
ifeq ($(COMPILER), gnu)
	CFLAGS += -fopenmp
endif
ifeq ($(COMPILER), intel)
	CFLAGS += -openmp
endif
ifeq ($(COMPILER), ibm)
	CFLAGS += -qsmp
endif
   	CFLAGS += -fopenmp
endif

# Exponential Table Build
ifeq ($(TABLE), yes)
	CFLAGS += -DTABLE
endif

#===============================================================================
# Targets to Build
#===============================================================================

$(program): $(obj) SimpleMOC-kernel_header.h
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 0SimpleMOC $(LDFLAGS) library/0.a
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 10SimpleMOC $(LDFLAGS) library/10.a
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 100SimpleMOC $(LDFLAGS) library/100.a
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 1000SimpleMOC $(LDFLAGS) library/1000.a
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 10000SimpleMOC $(LDFLAGS) library/10000.a
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 20000SimpleMOC $(LDFLAGS) library/20000.a
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 30000SimpleMOC $(LDFLAGS) library/30000.a
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 40000SimpleMOC $(LDFLAGS) library/40000.a


%.o: %.c
	clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(program) $(obj)

edit:
	vim -p $(source) SimpleMOC-kernel_header.h

run:
	./$(program)
