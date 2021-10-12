#ifndef SERVER
#define SERVER

class Handler {
	public:
		int WINDOWSIZE;
		int NGRAMSIZE_VALUE;
		Handler( double(*similarityFunction)(const char*,const char*),  int ngram_size = 3,  int window_size = 2);
		double dissimilarity(const char* msg1, const char* msg2 );
		// int getGraph(const char* msg);		
};

#endif

