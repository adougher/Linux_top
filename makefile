# To build the shell (which is called mytop by default), simply type:
#   make
#
# To clean up and remove the compiled binary, type:
#   make clean
#
# To build and run the unit tests, type:
#   make test

OBJS = top_functions.cpp top.cpp sysinfo.cpp
NAME = mytop
CPP_FLAGS = -Wall -l ncurses

all: $(NAME)

$(NAME): $(OBJS)
	g++ $(OBJS) -o $(NAME) $(CPP_FLAGS)

test: sysinfo_tests
	./sysinfo_tests

debug: $(OBJS)
	g++ $(OBJS) -o $(NAME) $(CPP_FLAGS) -g

clean:
	rm -rf $(NAME)* $(TESTS) gtest_main.a *.o

# Our headers.
HEADERS = *.h types/*.h

# Test outputs.
TESTS = sysinfo_tests

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = gtest-1.7.0

# Flags passed to the preprocessor.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler for test targets.
CXXFLAGS += -g -DPROC_ROOT='"proc"'

# All Google Test headers.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# Builds gtest_main.a.

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o: $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest_main.a: gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds the unit tests.

sysinfo.o: sysinfo.cpp $(GTEST_HEADERS) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c sysinfo.cpp

sysinfo_tests.o: sysinfo_tests.cpp $(GTEST_HEADERS) $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c sysinfo_tests.cpp

sysinfo_tests: sysinfo.o sysinfo_tests.o gtest_main.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ -lpthread
