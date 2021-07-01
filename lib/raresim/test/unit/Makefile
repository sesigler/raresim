UNITY_ROOT=./unity
CC=gcc
SRC=../../src/
TEST_SRC=$(wildcard *.c)
TEST_NAMES=$(TEST_SRC:.c=)
PREFIX=Runner_
RUNNERS=$(addprefix $(PREFIX),$(TESTS))

NO_PRE_SRC_FILES= lists.c 


SRC_FILES=$(addprefix $(SRC),$(NO_PRE_SRC_FILES))

all: check-env clean $(TEST_NAMES) test

check-env:
#ifdef TRAVIS_BUILD_DIR
#endif



%: %.c
	#ruby $(UNITY_ROOT)/auto/generate_test_runner.rb $@.c
	ruby $(UNITY_ROOT)/generate_test_runner.rb $@.c
	$(CC) -Werror -g -o $@ \
	    -I$(UNITY_ROOT) \
	    -I$(SRC) \
	    -DTEST $(UNITY_ROOT)/unity.c \
	    -DSAMTOOLS=1 \
	    -pthread \
	    $(SRC_FILES) \
	    $@.c $@_Runner.c \
	    -lz -lm -lcurl
	#rm $@_Runner.c

test:
	$(foreach var,$(TEST_NAMES),./$(var);)

clean:
	rm -f $(TEST_NAMES)
	rm -f *_Runner.c
	rm -rf *dSYM/
