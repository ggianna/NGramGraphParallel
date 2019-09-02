SOURCES_FOLDER			= src
HEADERS_FOLDER			= include
TEMPLATES_FOLDER		= include/templateImp
TEST_PROGRAMS_FOLDER		= test_programs
DEMOS_FOLDER			= demos
MEASUREMENT_PROGRAMS_FOLDER	= measurement_programs

OBJECTS	= $(SOURCES_FOLDER)/GraphSimilarity.o $(SOURCES_FOLDER)/NGramGraph.o $(SOURCES_FOLDER)/StringAtom.o $(SOURCES_FOLDER)/StringPayload.o $(SOURCES_FOLDER)/StringSplitter.o $(SOURCES_FOLDER)/ProximityApproach.o $(SOURCES_FOLDER)/NGGUpdateOperator.o $(SOURCES_FOLDER)/NGGMergeOperator.o $(SOURCES_FOLDER)/DocumentClass.o $(SOURCES_FOLDER)/OpenclUpdateComputation.o $(SOURCES_FOLDER)/FileUtils.o $(SOURCES_FOLDER)/OclUpdatableClass.o $(SOURCES_FOLDER)/InputParser.o

OUT = $(TEST_PROGRAMS_FOLDER)/createClassGraphs $(TEST_PROGRAMS_FOLDER)/testOpenclUpdate $(TEST_PROGRAMS_FOLDER)/test $(MEASUREMENT_PROGRAMS_FOLDER)/profile_update_kernel $(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_exec_time $(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_value_similarity $(MEASUREMENT_PROGRAMS_FOLDER)/profile_parallel_class_graph_construction $(MEASUREMENT_PROGRAMS_FOLDER)/updates_number_vs_time $(DEMOS_FOLDER)/demo

CC		= g++
FLAGS		= -c -std=c++11 -Wall -I$(HEADERS_FOLDER)
OPENCL_LIB	= -lOpenCL

all: $(OUT)


$(DEMOS_FOLDER)/demo: $(OBJECTS) $(SOURCES_FOLDER)/demo.o $(DEMOS_FOLDER)/demo_input.txt
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/demo.o $(OPENCL_LIB)

$(MEASUREMENT_PROGRAMS_FOLDER)/updates_number_vs_time: $(OBJECTS) $(SOURCES_FOLDER)/updates_number_vs_time.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/updates_number_vs_time.o $(OPENCL_LIB)

$(MEASUREMENT_PROGRAMS_FOLDER)/profile_parallel_class_graph_construction: $(OBJECTS) $(SOURCES_FOLDER)/profile_parallel_class_graph_construction.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/profile_parallel_class_graph_construction.o $(OPENCL_LIB)

$(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_value_similarity: $(OBJECTS) $(SOURCES_FOLDER)/ht_size_vs_value_similarity.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/ht_size_vs_value_similarity.o $(OPENCL_LIB)

$(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_exec_time: $(OBJECTS) $(SOURCES_FOLDER)/ht_size_vs_exec_time.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/ht_size_vs_exec_time.o $(OPENCL_LIB)

$(MEASUREMENT_PROGRAMS_FOLDER)/profile_update_kernel: $(SOURCES_FOLDER)/profile_update_kernel.o
	$(CC) -o $@ $(SOURCES_FOLDER)/profile_update_kernel.o $(OPENCL_LIB)

$(TEST_PROGRAMS_FOLDER)/testOpenclUpdate: $(OBJECTS) $(SOURCES_FOLDER)/testOpenclUpdate.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/testOpenclUpdate.o $(OPENCL_LIB)

$(TEST_PROGRAMS_FOLDER)/createClassGraphs: $(OBJECTS) $(SOURCES_FOLDER)/createClassGraphs.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/createClassGraphs.o $(OPENCL_LIB)

$(TEST_PROGRAMS_FOLDER)/test: $(OBJECTS) $(SOURCES_FOLDER)/test.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/test.o $(OPENCL_LIB)


$(SOURCES_FOLDER)/demo.o: $(DEMOS_FOLDER)/demo.cpp
	$(CC) $(FLAGS) $(DEMOS_FOLDER)/demo.cpp -o $@

$(SOURCES_FOLDER)/updates_number_vs_time.o: $(MEASUREMENT_PROGRAMS_FOLDER)/updates_number_vs_time.cpp
	$(CC) $(FLAGS) $(MEASUREMENT_PROGRAMS_FOLDER)/updates_number_vs_time.cpp -o $@

$(SOURCES_FOLDER)/profile_parallel_class_graph_construction.o: $(MEASUREMENT_PROGRAMS_FOLDER)/profile_parallel_class_graph_construction.cpp
	$(CC) $(FLAGS) $(MEASUREMENT_PROGRAMS_FOLDER)/profile_parallel_class_graph_construction.cpp -o $@

$(SOURCES_FOLDER)/ht_size_vs_value_similarity.o: $(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_value_similarity.cpp
	$(CC) $(FLAGS) $(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_value_similarity.cpp -o $@

$(SOURCES_FOLDER)/ht_size_vs_exec_time.o: $(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_exec_time.cpp
	$(CC) $(FLAGS) $(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_exec_time.cpp -o $@

$(SOURCES_FOLDER)/profile_update_kernel.o: $(MEASUREMENT_PROGRAMS_FOLDER)/profile_update_kernel.cpp
	$(CC) $(FLAGS) $(MEASUREMENT_PROGRAMS_FOLDER)/profile_update_kernel.cpp -o $@

$(SOURCES_FOLDER)/testOpenclUpdate.o: $(TEST_PROGRAMS_FOLDER)/testOpenclUpdate.cpp $(HEADERS_FOLDER)/DocumentClassComponent.hpp $(HEADERS_FOLDER)/FileUtils.hpp
	$(CC) $(FLAGS) $(TEST_PROGRAMS_FOLDER)/testOpenclUpdate.cpp -o $@

$(SOURCES_FOLDER)/createClassGraphs.o: $(TEST_PROGRAMS_FOLDER)/createClassGraphs.cpp $(HEADERS_FOLDER)/FileUtils.hpp
	$(CC) $(FLAGS) $(TEST_PROGRAMS_FOLDER)/createClassGraphs.cpp -o $@

$(SOURCES_FOLDER)/test.o: $(TEST_PROGRAMS_FOLDER)/test.cpp
	$(CC) $(FLAGS) $(TEST_PROGRAMS_FOLDER)/test.cpp -o $@


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

$(SOURCES_FOLDER)/OclUpdatableClass.o: $(SOURCES_FOLDER)/OclUpdatableClass.cpp $(HEADERS_FOLDER)/OclUpdatableClass.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/OclUpdatableClass.cpp -o $@

$(SOURCES_FOLDER)/InputParser.o: $(SOURCES_FOLDER)/InputParser.cpp $(HEADERS_FOLDER)/InputParser.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/InputParser.cpp -o $@


clean:
	rm -f $(OBJECTS) $(OUT)
	rm -f $(DEMOS_FOLDER)/demo_input.txt
	rm -f $(DEMOS_FOLDER)/generate_demo_input

no_exe_clean:
	rm -f $(OBJECTS)

count:
	wc -l $(SOURCES_FOLDER)/*.cpp $(HEADERS_FOLDER)/*.hpp $(TEMPLATES_FOLDER)/*.tcc


#
# Automagically generated demo input
#
$(DEMOS_FOLDER)/demo_input.txt: $(DEMOS_FOLDER)/generate_demo_input
	$(DEMOS_FOLDER)/generate_demo_input > $(DEMOS_FOLDER)/demo_input.txt

$(DEMOS_FOLDER)/generate_demo_input: $(DEMOS_FOLDER)/generate_demo_input.cpp
	$(CC) -Wall $(DEMOS_FOLDER)/generate_demo_input.cpp -o $@

