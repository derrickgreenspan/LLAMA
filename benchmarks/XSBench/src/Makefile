#===============================================================================
# User Options
#===============================================================================

COMPILER    = gnu
OPTIMIZE    = yes
DEBUG       = no
PROFILE     = no
MPI         = no
PAPI        = no
VEC_INFO    = no
VERIFY      = no
BINARY_DUMP = no
BINARY_READ = no

#===============================================================================
# Program name & source code list
#===============================================================================

program = XSBench

source = \
Main.c \
io.c \
Simulation.c \
CalculateXS.c \
GridInit.c \
XSutils.c \
Materials.c

obj = $(source:.c=.o)

#===============================================================================
# Sets Flags
#===============================================================================

# Standard Flags
CFLAGS := -std=gnu99 -Wall

# Linker Flags
LDFLAGS = -lm

# Regular gcc Compiler
ifeq ($(COMPILER),gnu)
  CC = clang 
  CFLAGS += -fopenmp 
endif

# Intel Compiler
ifeq ($(COMPILER),intel)
  CC = icc
  CFLAGS += -qopenmp 
endif

# BG/Q gcc Cross-Compiler
ifeq ($(MACHINE),bluegene)
  CC = mpicc
endif

# Debug Flags
ifeq ($(DEBUG),yes)
  CFLAGS += -g
  LDFLAGS  += -g
endif

# Profiling Flags
ifeq ($(PROFILE),yes)
  CFLAGS += -pg
  LDFLAGS  += -pg
endif

# Optimization Flags
ifeq ($(OPTIMIZE),yes)
  CFLAGS += -O3
endif

# Compiler Vectorization (needs -O3 flag) information
ifeq ($(VEC_INFO),yes)
  CFLAGS += -ftree-vectorizer-verbose=6
endif

# PAPI source (you may need to provide -I and -L pointing
# to PAPI depending on your installation
ifeq ($(PAPI),yes)
  source += papi.c
  CFLAGS += -DPAPI
  #CFLAGS += -I/soft/apps/packages/papi/papi-5.1.1/include
  #LDFLAGS += -L/soft/apps/packages/papi/papi-5.1.1/lib -lpapi
  LDFLAGS += -lpapi
endif

# MPI
ifeq ($(MPI),yes)
  CC = mpicc
  CFLAGS += -DMPI
endif

# Verification of results mode
ifeq ($(VERIFY),yes)
  CFLAGS += -DVERIFICATION
endif

# Binary dump for file I/O based initialization
ifeq ($(BINARY_DUMP),yes)
  CFLAGS += -DBINARY_DUMP
endif

# Binary read for file I/O based initialization
ifeq ($(BINARY_READ),yes)
  CFLAGS += -DBINARY_READ
endif


#===============================================================================
# Targets to Build
#===============================================================================

# 0.a  100000.a  10000.a  1000.a  100.a  10.a  20000.a  30000.a  40000.a
$(program): $(obj) XSbench_header.h Makefile 
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 0XSBench $(LDFLAGS) library/0.a
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 10XSBench $(LDFLAGS) library/10.a
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 100XSBench $(LDFLAGS) library/100.a
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 1000XSBench $(LDFLAGS) library/1000.a
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 10000XSBench $(LDFLAGS) library/10000.a
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 20000XSBench $(LDFLAGS) library/20000.a
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 30000XSBench $(LDFLAGS) library/30000.a
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) $(obj) -o 40000XSBench $(LDFLAGS) library/40000.a

%.o: %.c
	        clang -Xclang -load -Xclang build/libllamaPass.so $(CFLAGS) -c $< -o $@ 

clean:
	rm -rf $(program) $(obj)

edit:
	vim -p $(source) papi.c XSbench_header.h

run:
	./$(program)

bgqrun:
	qsub -t 10 -n 1 -O test XSBench
