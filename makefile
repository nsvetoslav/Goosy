# Variables
CC = gcc
CFLAGS = -Wall -shared -fPIC
CEXE_FLAGS = -Wall
LDFLAGS = -ldbghelp -limagehlp # Link against these libraries
HACKGAJJO_SRC = src/hackjaggo.c src/logger.c
HACKGAJJO_BIN = bin/hackjaggo.dll
HACKGAJJO_EXPORTABLE_BIN = bin/ehackjaggo.dll

exe_src = src/test.c
exe_bin = bin/hackjaggo.exe

# Check if DEBUG is set (default is release)
DEBUG ?= 0
DLL_EXPORTABLE ?= 0

# Add debug flag if DEBUG is set
ifeq ($(DEBUG), 1)
    CFLAGS += -g -DDEBUG # Add debug flag and define DEBUG macro
else
    CFLAGS += -O2 # Add optimization flag for release
endif

all: main dlls exportable_dlls

main: $(HACKGAJJO_SRC)
	$(CC) $(CFLAGS) -o $(HACKGAJJO_BIN) $(HACKGAJJO_SRC) $(LDFLAGS)

# Executable target (build the test executable)
dlls: $(exe_src)
	$(CC) $(CEXE_FLAGS) -o $(exe_bin) $(exe_src) $(LDFLAGS)

# Target for exportable DLL with DLL_EXPORTABLE=1
exportable_dlls: $(HACKGAJJO_SRC)
	$(CC) $(CFLAGS) -DDLL_EXPORTABLE=1 -o $(HACKGAJJO_EXPORTABLE_BIN) $(HACKGAJJO_SRC) $(LDFLAGS)

clean:
	rm -f $(HACKGAJJO_BIN) $(HACKGAJJO_EXPORTABLE_BIN) $(exe_bin)
