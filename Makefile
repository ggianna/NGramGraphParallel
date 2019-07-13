SOURCES_FOLDER		= src
HEADERS_FOLDER		= include
TEMPLATES_FOLDER	= include/templateImp
OBJECTS			= $(SOURCES_FOLDER)/GraphSimilarity.o $(SOURCES_FOLDER)/NGramGraph.o $(SOURCES_FOLDER)/StringAtom.o $(SOURCES_FOLDER)/StringPayload.o $(SOURCES_FOLDER)/StringSplitter.o $(SOURCES_FOLDER)/ProximityApproach.o $(SOURCES_FOLDER)/NGGUpdateOperator.o $(SOURCES_FOLDER)/NGGMergeOperator.o $(SOURCES_FOLDER)/DocumentClass.o $(SOURCES_FOLDER)/OpenclUpdateComputation.o $(SOURCES_FOLDER)/FileUtils.o
OUT		= createClassGraphs testOpenclUpdate serial_vs_parallel_update profile_update_kernel
CC		= g++
FLAGS		= -c -std=c++11 -Wall -I$(HEADERS_FOLDER)
OPENCL_LIB	= -lOpenCL

all: $(OUT)


profile_update_kernel: $(SOURCES_FOLDER)/profile_update_kernel.o
	$(CC) -o $@ $(SOURCES_FOLDER)/profile_update_kernel.o $(OPENCL_LIB)

serial_vs_parallel_update: $(OBJECTS) $(SOURCES_FOLDER)/serial_vs_parallel_update.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/serial_vs_parallel_update.o $(OPENCL_LIB)

testOpenclUpdate: $(OBJECTS) $(SOURCES_FOLDER)/testOpenclUpdate.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/testOpenclUpdate.o $(OPENCL_LIB)

createClassGraphs: $(OBJECTS) $(SOURCES_FOLDER)/createClassGraphs.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/createClassGraphs.o $(OPENCL_LIB)

#test: $(OBJECTS) $(SOURCES_FOLDER)/test.o
#	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/test.o $(OPENCL_LIB)

$(SOURCES_FOLDER)/profile_update_kernel.o: profile_update_kernel.cpp
	$(CC) $(FLAGS) profile_update_kernel.cpp -o $@

$(SOURCES_FOLDER)/serial_vs_parallel_update.o: serial_vs_parallel_update.cpp
	$(CC) $(FLAGS) serial_vs_parallel_update.cpp -o $@

$(SOURCES_FOLDER)/testOpenclUpdate.o: testOpenclUpdate.cpp $(HEADERS_FOLDER)/DocumentClassComponent.hpp $(HEADERS_FOLDER)/FileUtils.hpp
	$(CC) $(FLAGS) testOpenclUpdate.cpp -o $@

$(SOURCES_FOLDER)/createClassGraphs.o: createClassGraphs.cpp $(HEADERS_FOLDER)/FileUtils.hpp
	$(CC) $(FLAGS) createClassGraphs.cpp -o $@

#$(SOURCES_FOLDER)/test.o: test.cpp
#	$(CC) $(FLAGS) test.cpp -o $@

$(SOURCES_FOLDER)/GraphSimilarity.o: $(SOURCES_FOLDER)/GraphSimilarity.cpp $(HEADERS_FOLDER)/GraphSimilarity.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/GraphSimilarity.cpp -o $@

$(SOURCES_FOLDER)/NGramGraph.o: $(SOURCES_FOLDER)/NGramGraph.cpp $(HEADERS_FOLDER)/NGramGraph.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/NGramGraph.cpp -o $@

$(SOURCES_FOLDER)/StringAtom.o: $(SOURCES_FOLDER)/StringAtom.cpp $(HEADERS_FOLDER)/StringAtom.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/StringAtom.cpp -o $@

$(SOURCES_FOLDER)/StringPayload.o: $(SOURCES_FOLDER)/StringPayload.cpp $(HEADERS_FOLDER)/StringPayload.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/StringPayload.cpp -o $@

$(SOURCES_FOLDER)/StringSplitter.o: $(SOURCES_FOLDER)/StringSplitter.cpp $(HEADERS_FOLDER)/StringSplitter.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/StringSplitter.cpp -o $@

$(SOURCES_FOLDER)/ProximityApproach.o: $(SOURCES_FOLDER)/ProximityApproach.cpp $(HEADERS_FOLDER)/ProximityApproach.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/ProximityApproach.cpp -o $@

$(SOURCES_FOLDER)/NGGUpdateOperator.o: $(SOURCES_FOLDER)/NGGUpdateOperator.cpp $(HEADERS_FOLDER)/NGGUpdateOperator.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/NGGUpdateOperator.cpp -o $@

$(SOURCES_FOLDER)/NGGMergeOperator.o: $(SOURCES_FOLDER)/NGGMergeOperator.cpp $(HEADERS_FOLDER)/NGGMergeOperator.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/NGGMergeOperator.cpp -o $@

$(SOURCES_FOLDER)/DocumentClass.o: $(SOURCES_FOLDER)/DocumentClass.cpp $(HEADERS_FOLDER)/DocumentClass.hpp $(HEADERS_FOLDER)/DocumentClassComponent.hpp $(HEADERS_FOLDER)/FileUtils.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/DocumentClass.cpp -o $@

$(SOURCES_FOLDER)/OpenclUpdateComputation.o: $(SOURCES_FOLDER)/OpenclUpdateComputation.cpp $(HEADERS_FOLDER)/OpenclUpdateComputation.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/OpenclUpdateComputation.cpp -o $@

$(SOURCES_FOLDER)/FileUtils.o: $(SOURCES_FOLDER)/FileUtils.cpp $(HEADERS_FOLDER)/FileUtils.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/FileUtils.cpp -o $@


clean:
	rm -f $(OBJECTS) $(OUT)

no_exe_clean:
	rm -f $(OBJECTS)

count:
	wc -l $(SOURCES_FOLDER)/*.cpp $(HEADERS_FOLDER)/*.hpp $(TEMPLATES_FOLDER)/*.tcc
