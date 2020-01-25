# Architektury výpočetních systémů (AVS 2019)
# Projekt c. 2 (PMC)
# Login: xdusek27

# run:
# $ make run-{ref|loop|tree}


THREADS=16
GRID=64
INPUT=data/bun_zipper_res4.pts
ISO=0.15

.PHONY: all clean build run-ref run-loop run-tree

all:

clean:
	@rm -rf build/ xdusek27.zip

build:
	@mkdir -p build/
	@cd build/
	@cmake ../src -DCMAKE_BUILD_TYPE=Release
	@echo
	@make -j`nproc`

.ONESHELL:
run-ref:
	@mkdir -p build/
	@cd build/
	@cmake ../src -DCMAKE_BUILD_TYPE=Release
	@echo
	@make -j`nproc`
	@echo
	@cd ../
	./build/PMC -b ref -t $(THREADS) -g $(GRID) -l $(ISO) $(INPUT) build/ref.obj
	@echo

.ONESHELL:
run-loop:
	@mkdir -p build/
	@cd build/
	@cmake ../src -DCMAKE_BUILD_TYPE=Release
	@echo
	@make -j`nproc`
	@echo
	@cd ../
	./build/PMC -b loop -t $(THREADS) -g $(GRID) -l $(ISO) $(INPUT) build/loop.obj
	@python scripts/compare_outputs.py build/ref.obj build/loop.obj
	@echo

.ONESHELL:
run-tree:
	@mkdir -p build/
	@cd build/
	@cmake ../src -DCMAKE_BUILD_TYPE=Release
	@echo
	@make -j`nproc`
	@echo
	@cd ../
	./build/PMC -b tree -t $(THREADS) -g $(GRID) -l $(ISO) $(INPUT) build/tree.obj
	@python scripts/compare_outputs.py build/ref.obj build/tree.obj
	@echo

.ONESHELL:
zip:
	@cp src/parallel_builder/* ./
	@cp results/*.png ./
	@zip xdusek27 PMC-xdusek27.txt *.cpp *.h *.png
	@rm *.cpp *.h *.png
