# Name of the final executable of the project
EXEC = ifj17
LOGIN=xhosal00
EXTENSIONS=rozsireni rozdeleni dokumentace.pdf


# Files specified in here are going to be compiled into final
# executable. Files that are going to be used to build the
# project must be specified manually.
SRC = ifj17.c error_ifj.c generator.c ilist.c symtable.c scanner.c \
symtable_stack.c keywords.c precedent.c precedent_rules.c precedent_tools.c \
precedent_stack.c precedent_table.c parser.c parser_essentials.c \
semantic_rules.c syntax_rules.c semantic_essentials.c garbage_cleaner.c \
terms.c prefix_tool.c

# Build specifying variables
DEPS = dep.list
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g

# Transforms *.c into *.o
OBJ = $(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

all: $(DEPS) $(EXEC)

# Dependencies of every file in the project.
# File dependencies are generated whenever
# a file of the project is modified.
$(DEPS): $(SRC)
	$(CC) -MM $(SRC) > $(DEPS)

# Generated dependencies are inserted into Makefile.
-include $(DEPS)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm $(OBJ) $(EXEC)

zip:
	zip $(LOGIN).zip *.c *.h Makefile $(EXTENSIONS)
