SRC = $(shell find . -name "*.c")
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

CFLAGS  = -O2 -march=native -pipe -I../include
CFLAGS += -Wall -Wextra -Werror -pedantic -ansi

LDFLAGS = -lreadline ../libkpmalloc.a

# For now...
CFLAGS += -DDEBUG -g

KPMALLOC = ../libkpmalloc.a
TARGET = test.exe

.PHONY: test
test: $(TARGET)
	@echo "	EXECVP	$(TARGET)"
	@./$(TARGET)

$(TARGET): $(OBJ)
	@echo "	LD	$@"
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ 

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

