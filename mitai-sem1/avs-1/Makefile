# Architektury výpočetních systémů (AVS 2019)
# Projekt c. 1 (ANN)
# Login: xdusek27

# run:
# $ make step{N}-{version}
# N in {0, 1, 2, 3, 4}
# version in {dbg, cache, perform}


PAPI_EVENTS_CACHE=PAPI_L1_DCM|PAPI_SR_INS|PAPI_LD_INS|PAPI_L2_DCA|PAPI_L2_DCM|PAPI_L3_TCA|PAPI_L3_TCM

PAPI_EVENTS_PERFORM=PAPI_FP_OPS|PAPI_SP_OPS|PAPI_DP_OPS|PAPI_VEC_SP|PAPI_VEC_DP


all:


pack:
	zip -r xdusek27.zip Step0/ Step1/ Step2/ Step3/ Step4/ ANN-xdusek27.txt


clean:
	rm -rf build/ xdusek27.zip


# module load doesn't work :(
ml: SHELL:=/bin/bash
ml:
	module load intel PAPI HDF5 CMake Python/3.6.1


build:
	mkdir -p build


.ONESHELL:
step0-dbg: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=0 -DSTEPS='0'
	@echo
	@make -j`nproc`
	@time ./Step0/ANN ../Data/network.h5 ../Data/testData.h5 \
		./Step0/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log
	@echo
	@python3 ../Scripts/compareOutputs.py ./Step0/output.h5 ../Data/testRefOutput.h5 2> /dev/null


.ONESHELL:
step0-cache: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='0'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_CACHE)' \
		./Step0/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step0/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step0-perform: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='0'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_PERFORM)' \
		./Step0/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step0/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step1-dbg: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=0 -DSTEPS='1'
	@echo
	@make -j`nproc`
	@time ./Step1/ANN ../Data/network.h5 ../Data/testData.h5 \
		./Step1/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log
	@echo
	@python3 ../Scripts/compareOutputs.py ./Step1/output.h5 ../Data/testRefOutput.h5 2> /dev/null


.ONESHELL:
step1-cache: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='1'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_CACHE)' \
		./Step1/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step1/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step1-perform: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='1'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_PERFORM)' \
		./Step1/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step1/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step2-dbg: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=0 -DSTEPS='2'
	@echo
	@make -j`nproc`
	@time ./Step2/ANN ../Data/network.h5 ../Data/testData.h5 \
		./Step2/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log
	@echo
	@python3 ../Scripts/compareOutputs.py ./Step2/output.h5 ../Data/testRefOutput.h5 2> /dev/null


.ONESHELL:
step2-cache: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='2'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_CACHE)' \
		./Step2/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step2/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step2-perform: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='2'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_PERFORM)' \
		./Step2/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step2/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step3-dbg: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=0 -DSTEPS='3'
	@echo
	@make -j`nproc`
	@time ./Step3/ANN ../Data/network.h5 ../Data/testData.h5 \
		./Step3/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log
	@echo
	@python3 ../Scripts/compareOutputs.py ./Step3/output.h5 ../Data/testRefOutput.h5 2> /dev/null


.ONESHELL:
step3-cache: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='3'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_CACHE)' \
		./Step3/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step3/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step3-perform: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='3'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_PERFORM)' \
		./Step3/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step3/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step4-dbg: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=0 -DSTEPS='4'
	@echo
	@make -j`nproc`
	@time ./Step4/ANN ../Data/network.h5 ../Data/testData.h5 \
		./Step4/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log
	@echo
	@python3 ../Scripts/compareOutputs.py ./Step4/output.h5 ../Data/testRefOutput.h5 2> /dev/null


.ONESHELL:
step4-cache: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='4'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_CACHE)' \
		./Step4/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step4/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log


.ONESHELL:
step4-perform: build
	@cd build
	@cmake .. -DCMAKE_BUILD_TYPE=Release -DWITH_PAPI=1 -DSTEPS='4'
	@make -j`nproc`
	PAPI_EVENTS='$(PAPI_EVENTS_PERFORM)' \
		./Step4/ANN ../Data/network.h5 ../Data/bigDataset.h5 \
		./Step4/output.h5 > $@-`date +%d-%m_%H:%M:%S`.log
