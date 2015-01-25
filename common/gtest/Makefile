# make gtest lib
GTEST_DIR = ../../
LIB_DIR = $(GTEST_DIR)/lib
OBJ_DIR = $(GTEST_DIR)/.obj/common
OBJS = libgtest.a libgtest_main.a 

# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -g -Wall -Wextra -lpthread

GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/common/gtest/*.cc $(GTEST_DIR)/common/gtest/*.h $(GTEST_HEADERS)

all : $(OBJS)
	mkdir -p $(OBJ_DIR)
	mv *.o $(OBJ_DIR)
	mv $^ $(LIB_DIR)

clean :
	rm -rf $(OBJ_DIR)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/common/gtest/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/common/gtest/gtest_main.cc

libgtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

libgtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^
