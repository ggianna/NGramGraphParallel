#include "OclUpdatableClass.hpp"
#include "FileUtils.hpp"
#include "Atom.hpp"
#include "StringSplitter.hpp"
#include "StringPayload.hpp"
#include "HashFunction.hpp"

OclUpdatableClass::OclUpdatableClass(std::size_t ts) : DocumentClass(),
			hash_table_size(ts), edge_labels(ts), hash_values(ts),
			oclUpdateComp(nullptr, nullptr, nullptr, this, nullptr)
{
	// Allocate C-style array and initialize to 0
	hash_table  = new EDGE_WEIGHT_TYPE[hash_table_size]();
}


void OclUpdatableClass::getNewEdges(DocumentClassComponent *c) {
	unsigned char *flags = c->getFlags();
	std::vector<std::string>& component_edges = c->getEdgeLabels();
	std::vector<unsigned long>& component_hash_values = c->getHashValues();
	for (std::size_t i=0; i<hash_table_size; i++) {
		if (flags[i] == 1) {
			edge_labels[i] = component_edges[i];
			hash_values[i] = component_hash_values[i];
		}
	}
}


void OclUpdatableClass::buildGraph() {
	std::string source, target;
	Atom<std::string> aSource, aTarget;

	for (std::size_t i=0; i<hash_table_size; i++) {
		if (edge_labels[i].length() > 0) {
			std::tie(source, target) = updateOp.extractHeadAndTailFromEdgeLabel(edge_labels[i]);
			aSource.setData(source);
			aTarget.setData(target);
			addEdge(aSource, aTarget, hash_table[i]);
		}
	}
}


void OclUpdatableClass::singleUpdate(DocumentClassComponent *component, Context *context, CommandQueue *queue, Program *program) {
	/*
	// Assert that hash table sizes of this object and component object are equal
	// TODO Throw exception
	if (hash_table_size != component->getHashTableSize()) {
		cout << "DocumentClass::updateWithOpenCL(): hash table sizes of "
		     <<	"class and component are not equal.";
		exit(1);
	}
	
	// Compute learing factor
	++numberOfConstituents;
	float learning_factor = computeLearningFactor();
	EDGE_WEIGHT_TYPE *component_hash_table = component->getHashTable();
	unsigned char *flags = component->getFlags();
	vector<std::string>& component_edges = component->getEdgeLabels();
	vector<unsigned long>& component_hash_values = component->getHashValues();

	oclUpdateComp.setOpenclFields(context, queue, program);
	oclUpdateComp.setSampleTable(component_hash_table);
	oclUpdateComp.setFlags(flags);
	oclUpdateComp.setLearningFactor(learning_factor);

	oclUpdateComp.apply();
	
	// The hash_table array is now updated and the flags array indicates the new edges
	Atom<std::string> aSource, aTarget;
	std::string source, target;
	for (std::size_t i = 0; i < hash_table_size; ++i) {
		if (flags[i] == 1) {
			std::tie(source, target) = updateOp.extractHeadAndTailFromEdgeLabel(component_edges[i]);
			aSource.setData(source);
			aTarget.setData(target);
			addEdge(aSource, aTarget, hash_table[i]);
			edge_labels[i] = component_edges[i];
			hash_values[i] = component_hash_values[i];
		}
	}
	*/
}


void OclUpdatableClass::buildClass(std::string componentsDir, Context *context, CommandQueue *queue, Program *program) {
	StringSplitter splitter;
	StringPayload payload;
	JenkinsHash hashFunction;
	std::vector<std::string> componentFiles;
	DocumentClassComponent *component = new DocumentClassComponent(hash_table_size,
			CorrelationWindow, &splitter, &payload, &hashFunction);
	float l;

	if (componentsDir.back() != '/') {
		componentsDir += "/";
	}

	FileUtils::read_directory(componentsDir, componentFiles);

	oclUpdateComp.setCurrentUpdatesNo(0);
	oclUpdateComp.setTotalUpdatesNo(componentFiles.size());
	oclUpdateComp.setOpenclFields(context, queue, program);
	oclUpdateComp.setComponent(component);

	for (auto& file : componentFiles) {
		++numberOfConstituents;
		l = computeLearningFactor();
		oclUpdateComp.setLearningFactor(l);
		payload.setPayload(FileUtils::read_file_to_string(componentsDir + file));
		component->fillTables();
		oclUpdateComp.apply();
		getNewEdges(component);
		component->reset();
	}

	delete component;

	buildGraph();
}
