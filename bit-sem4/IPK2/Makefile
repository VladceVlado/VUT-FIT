# Usage:
#   $ make
#   $ make clean
#   $ make run
#   $ make doxygen
#   $ make pack

EXEC = ipk-lookup
DOC = ipk2-doc
TASK = ipk2-task

.PHONY: all src src_clean src_clean-all doc doc_clean doc_clean-all task task_clean task_clean-all

# ToDo
all:
	@echo "ToDo, echo funguje"

# application

src:
	cd src/ && make

src_clean:
	cd src/ && make clean

src_clean-all:
	cd src/ && make clean-all


# documentation

doc:
	cd doc/ && make

doc_clean:
	cd doc/ && make clean

doc_clean-all:
	cd doc/ && make clean-all


# task

task:
	cd task/ && make

task_clean:
	cd task/ && make clean

task_clean-all:
	cd task/ && make clean-all

# test

test: src_clean-all src
	cd test/ && ./run-test.sh
