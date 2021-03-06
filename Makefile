SRC = $(shell find src/ -name "*.c")
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

CFLAGS  = -O2 -march=native -pipe -Iinclude
CFLAGS += -Wall -Wextra -Werror -pedantic -ansi

# For now...
CFLAGS += -DDEBUG -g
MAKEFLAGS = --no-print-directory
ARFLAGS = rcs
TARGET  = libkpmalloc.a

$(TARGET): $(OBJ)
	@echo "	AR	$@"
	@$(AR) $(ARFLAGS) $@ $^ 

.PHONY: test
test: $(TARGET)
	@cd test/ && $(MAKE) $(MAKEFLAGS) test

%.o: %.c
	@echo "	CC	$@"
	@$(CC) $(CFLAGS) -o $@ -c $<

-include $(DEP)

%.d: %.c
	@$(CC) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

.PHONY: clean
clean:
	@echo "	CLEAN	$(TARGET)"
	@rm -rf $(OBJ) $(DEP) $(TARGET)
	@cd test/ && $(MAKE) $(MAKEFLAGS) clean
