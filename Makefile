CXX = dpcpp
CXXFLAGS = -o
LDFLAGS = -lOpenCL -lsycl
EXE_NAME = integral
SOURCES = main.cpp library.cpp worker.cpp
BINDIR = bin

all: main

main:
	$(CXX) $(CXXFLAGS) $(EXE_NAME) $(SOURCES) $(LDFLAGS)

run: 
	./$(EXE_NAME)
    
queue: 
	echo 'cd $$PBS_O_WORKDIR; ./integral'| qsub -l nodes=1:gpu:ppn=2 -l walltime=00:03:00 -N numintegr

clean: 
	rm -rf $(EXE_NAME) numintegr.*
    
result:
	cat numintegr.o*
    