SOURCES_FOLDER			= src
HEADERS_FOLDER			= include
TEMPLATES_FOLDER		= include/templateImp
TEST_PROGRAMS_FOLDER		= test_programs
DEMOS_FOLDER			= demos
MEASUREMENT_PROGRAMS_FOLDER	= measurement_programs
OPENCL_HEADERS_FOLDER		=/home/ggianna/Documents/CApplications/OpenCL-SDK/include/cpp
C_WRAPPER_FOLDER		= /home/ngialitsis/search/NGramGraphParallel/c_wrapper
###############

OBJECTS	= $(SOURCES_FOLDER)/GraphSimilarity.o $(SOURCES_FOLDER)/NGramGraph.o $(SOURCES_FOLDER)/StringAtom.o $(SOURCES_FOLDER)/StringPayload.o $(SOURCES_FOLDER)/StringSplitter.o $(SOURCES_FOLDER)/ProximityApproach.o $(SOURCES_FOLDER)/NGGUpdateOperator.o $(SOURCES_FOLDER)/NGGMergeOperator.o $(SOURCES_FOLDER)/DocumentClass.o $(SOURCES_FOLDER)/OpenclUpdateComputation.o $(SOURCES_FOLDER)/FileUtils.o $(SOURCES_FOLDER)/OclUpdatableClass.o $(SOURCES_FOLDER)/InputParser.o $(C_WRAPPER_FOLDER)/libC_HandleGraphRequest.so   $(C_WRAPPER_FOLDER)/libC_Interface.so  $(C_WRAPPER_FOLDER)/libStringSplitter.so $(C_WRAPPER_FOLDER)/libNGramGraph.so $(C_WRAPPER_FOLDER)/libProximityApproach.so

OUT = $(TEST_PROGRAMS_FOLDER)/createClassGraphs $(TEST_PROGRAMS_FOLDER)/testOpenclUpdate $(TEST_PROGRAMS_FOLDER)/test $(MEASUREMENT_PROGRAMS_FOLDER)/profile_update_kernel $(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_exec_time $(MEASUREMENT_PROGRAMS_FOLDER)/ht_size_vs_value_similarity $(MEASUREMENT_PROGRAMS_FOLDER)/profile_parallel_class_graph_construction $(MEASUREMENT_PROGRAMS_FOLDER)/updates_number_vs_time $(DEMOS_FOLDER)/demo $(TEST_PROGRAMS_FOLDER)/NGramGraphTransformSaveString $(C_WRAPPER_FOLDER)/Request


CC		= g++
C		= gcc
FLAGS		= -c -std=c++11 -Wall -I$(HEADERS_FOLDER) -I$(OPENCL_HEADERS_FOLDER)
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

$(TEST_PROGRAMS_FOLDER)/NGramGraphTransformSaveString: $(OBJECTS) $(SOURCES_FOLDER)/NGramGraphTransformSaveString.o
	$(CC) -o $@ $(OBJECTS) $(SOURCES_FOLDER)/NGramGraphTransformSaveString.o $(OPENCL_LIB)

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

$(SOURCES_FOLDER)/NGramGraphTransformSaveString.o: $(TEST_PROGRAMS_FOLDER)/NGramGraphTransformSaveString.cpp
	$(CC) $(FLAGS) $(TEST_PROGRAMS_FOLDER)/NGramGraphTransformSaveString.cpp -o $@

$(C_WRAPPER_FOLDER)/libStringSplitter.so: $(SOURCES_FOLDER)/StringSplitter.cpp $(HEADERS_FOLDER)/StringSplitter.hpp
	$(CC) -I$(HEADERS_FOLDER) -fpic -shared $(SOURCES_FOLDER)/StringSplitter.cpp -o $@

$(C_WRAPPER_FOLDER)/libC_HandleGraphRequest.so: $(C_WRAPPER_FOLDER)/C_HandleGraphRequest.cpp $(C_WRAPPER_FOLDER)/C_HandleGraphRequest.h $(C_WRAPPER_FOLDER)/libStringSplitter.so $(C_WRAPPER_FOLDER)/libProximityApproach.so $(C_WRAPPER_FOLDER)/libNGramGraph.so
	$(CC) -I$(HEADERS_FOLDER) -fpic -shared $(C_WRAPPER_FOLDER)/C_HandleGraphRequest.cpp -L$(C_WRAPPER_FOLDER) -lStringSplitter -lNGramGraph -lProximityApproach -o $@

$(C_WRAPPER_FOLDER)/libC_Interface.so: $(C_WRAPPER_FOLDER)/C_Interface.cpp $(C_WRAPPER_FOLDER)/libC_HandleGraphRequest.so  
	$(CC) -I$(HEADERS_FOLDER) -fpic -shared $(C_WRAPPER_FOLDER)/C_Interface.cpp -L$(C_WRAPPER_FOLDER) -lC_HandleGraphRequest -lStringSplitter -lStringAtom -lStringPayload -o $@

$(C_WRAPPER_FOLDER)/Request: $(C_WRAPPER_FOLDER)/Request.c $(C_WRAPPER_FOLDER)/libC_Interface.so  $(C_WRAPPER_FOLDER)/C_HandleGraphRequest.h $(C_WRAPPER_FOLDER)/libStringSplitter.so $(C_WRAPPER_FOLDER)/libProximityApproach.so $(C_WRAPPER_FOLDER)/libNGramGraph.so $(C_WRAPPER_FOLDER)/libGraphSimilarity.so
	$(C) $(C_WRAPPER_FOLDER)/Request.c -L$(C_WRAPPER_FOLDER) -lC_Interface  -lStringSplitter -lNGramGraph -lProximityApproach -lGraphSimilarity  -o $@

$(SOURCES_FOLDER)/GraphSimilarity.o: $(SOURCES_FOLDER)/GraphSimilarity.cpp $(HEADERS_FOLDER)/GraphSimilarity.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/GraphSimilarity.cpp -o $@

$(C_WRAPPER_FOLDER)/libGraphSimilarity.so: $(SOURCES_FOLDER)/GraphSimilarity.cpp $(HEADERS_FOLDER)/GraphSimilarity.hpp
	$(CC) -I$(HEADERS_FOLDER) -fpic -shared $(SOURCES_FOLDER)/GraphSimilarity.cpp -o $@



$(SOURCES_FOLDER)/NGramGraph.o: $(SOURCES_FOLDER)/NGramGraph.cpp $(HEADERS_FOLDER)/NGramGraph.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/NGramGraph.cpp -o $@

$(C_WRAPPER_FOLDER)/libNGramGraph.so: $(SOURCES_FOLDER)/NGramGraph.cpp $(HEADERS_FOLDER)/NGramGraph.hpp
	$(CC) -I$(HEADERS_FOLDER) -fpic -shared $(SOURCES_FOLDER)/NGramGraph.cpp -o $@


$(SOURCES_FOLDER)/StringAtom.o: $(SOURCES_FOLDER)/StringAtom.cpp $(HEADERS_FOLDER)/StringAtom.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/StringAtom.cpp -o $@

$(C_WRAPPER_FOLDER)/libStringAtom.so: $(SOURCES_FOLDER)/StringAtom.cpp $(HEADERS_FOLDER)/StringAtom.hpp
	$(CC) -I$(HEADERS_FOLDER) -fpic -shared $(SOURCES_FOLDER)/StringAtom.cpp -o $@



$(SOURCES_FOLDER)/StringPayload.o: $(SOURCES_FOLDER)/StringPayload.cpp $(HEADERS_FOLDER)/StringPayload.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/StringPayload.cpp -o $@

$(C_WRAPPER_FOLDER)/libStringPayload.so: $(SOURCES_FOLDER)/StringPayload.cpp $(HEADERS_FOLDER)/StringPayload.hpp $(C_WRAPPER_FOLDER)/libStringAtom.so
	$(CC) -I$(HEADERS_FOLDER) -fpic -shared $(SOURCES_FOLDER)/StringPayload.cpp -L$(C_WRAPPER_FOLDER) -lStringAtom -o $@


$(SOURCES_FOLDER)/StringSplitter.o: $(SOURCES_FOLDER)/StringSplitter.cpp $(HEADERS_FOLDER)/StringSplitter.hpp $(C_WRAPPER_FOLDER)/libStringPayload.so $(C_WRAPPER_FOLDER)/libStringAtom.so
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/StringSplitter.cpp -L$(C_WRAPPER_FOLDER) -lStringPayload -lStringAtom -o $@

$(SOURCES_FOLDER)/ProximityApproach.o: $(SOURCES_FOLDER)/ProximityApproach.cpp $(HEADERS_FOLDER)/ProximityApproach.hpp
	$(CC) $(FLAGS) $(SOURCES_FOLDER)/ProximityApproach.cpp -o $@

$(C_WRAPPER_FOLDER)/libProximityApproach.so: $(SOURCES_FOLDER)/ProximityApproach.cpp $(HEADERS_FOLDER)/ProximityApproach.hpp
	$(CC) -I$(HEADERS_FOLDER) -fpic -shared $(SOURCES_FOLDER)/ProximityApproach.cpp -o $@


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

wrap: $(WRAPS)
clean:
	rm -f $(OBJECTS) $(OUT)
	rm -f $(DEMOS_FOLDER)/demo_input.txt
	rm -f $(DEMOS_FOLDER)/generate_demo_input
	rm -f $(TEST_PROGRAMS_FOLDER)/NGramGraphTransformString

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

