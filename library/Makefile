# Author: Derrick Greenspan
# Use make CC=gcc if you want to use gcc
# Since we're using the llvm compiler infrastructure for the llvm pass,
# We'll default to using clang


RMDIR = rmdir

CC = clang
GCC_CC = gcc
CFLAGS = -Wall -lnuma
DEPS = intercept.h llamalib.h 

SOURCE_1 = llama.c
SOURCE_2 = redefinitions.c

TARGET_1 = llama.o
TARGET_2 = redefinitions.o

INSTALL = build

clang: srcs/${SOURCES} clean 
	@mkdir -p ${INSTALL}
	@$(CC) -c srcs/${SOURCE_1} -o ${INSTALL}/$(TARGET_1)
	@$(CC) -c srcs/${SOURCE_2} -o ${INSTALL}/$(TARGET_2)
	@ar cq ${INSTALL}/llama.a ${INSTALL}/*.o 

gcc: srcs/${SOURCES}
	${GCC_CC} -lnuma -c srcs/${SOURCES} -o ${INSTALL}/${TARGET}

clean:
	@$(RM) $(INSTALL)/*
	@if [ -e $(INSTALL) ]; then \
		$(RMDIR) $(INSTALL); \
	fi

