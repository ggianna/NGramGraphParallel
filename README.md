
# NGramGraphParallel
A generic implementation of n-gram graphs, designed to be able to handle not only texts (or text strings in general) but any custom data format, without changes to the main structure of the code.\
Also, support for OpenCL is added to speed up workload-heavy graph operations with parallelization.

## Features
* **Flexibility**: The basic n-gram graph representation (see [George Giannakopoulos](http://users.iit.demokritos.gr/~ggianna/)' [thesis, Chapter 3](http://users.iit.demokritos.gr/~ggianna/thesis.pdf) for additional information) is expanded and defined generic, which results in data-agnostic and reusable code. In detail, the "text" and "n-gram" are replaced by "payload" and "atom"; In other words, an entity and the smallest pieces this entity can be split into. This enables custom represantation of other data types (e.g. DNA), which leads to clearer representation and maybe better performance than the equivalent string one.
* **Scalability**: Through parallelization, workload-heavy graph operations can speed up significantly. Real life scenarios may contain a large amount of data, and parallelizing specific operations (e.g. graph comparisons) can bring execution time for big datasets down to reasonable levels. Parallelization is only implemented for the construction of a class graph out of it's document components.

## Dependencies
* [Boost](https://www.boost.org/) : NGramGraphParallel uses [Boost's Graph library](https://www.boost.org/doc/libs/1_68_0/libs/graph/doc/index.html) for its graphs. To be able to run the code, you must have boost installed in your system.
You can download the boost libraries [here](https://www.boost.org/users/download/).
* [OpenCL](https://www.khronos.org/opencl/) : The parallelization of the workload is going to use OpenCL. [Compatible hardware](https://en.wikipedia.org/wiki/OpenCL#Version_support) and their appropriate driver(s) are required.

## Demo execution
Inside the project's top directory:\
`make all`\
`./demos/demo -h`\
`./demos/demo -f ./demos/demo_input.txt -c serial`

# Setting-up C wrapper
* append c\_wrapper directory to LD\_LIBRARY\_PATH   
	`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:full/path/to/c_wrapper`\
	or add above line  to ~/.bashrc for permanent use
* edit Makefile path variable `C_WRAPPER_FOLDER` and `OPENCL_HEADERS_FOLDER`
* run "make" on main directory
* test wrapper by running ./c\_wrapper/Request

## Version
v0.1 This version is the result of a 3-month internship. Only basic functionalities are implemented.
This version includes:
* The generic n-gram graphs representation. 
* Implementation for both generic and specialized (only for text) graphs.
* Basic graph operations and functionality.
* Support for [DOT](http://www.graphviz.org/doc/info/lang.html) language representation of n-Gram graphs.<br/><br/>

v0.2 This version is the result of a 2-month internship. It includes:
* The definition and implementation of some useful classes.
* Significant speedup of constructing a document class from it's text components using OpenCL.
* Some useful measurement programs and diagrams demonstrating the behavior of the implemented parallel method (class graph construction).
* A summarizing presentation and a demo.

## License
NGramGraphParallel is licenced under [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).









