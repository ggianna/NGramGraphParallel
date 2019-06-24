#ifndef HASH_FUNCTION_H
#define HASH_FUNCTION_H

template <typename ArgumentType, typename ValueType>
class HashFunction {
protected:
	ArgumentType argument;
	std::string name;

public:
	HashFunction();

	HashFunction(std::string n) : name(n) {}

	HashFunction(ArgumentType arg, std::string n) : argument(arg), name(n) {}

	void setArgument(ArgumentType arg) { argument = arg; }

	std::string getName() { return name; }

	virtual ValueType apply() = 0;
};

class DJB2 : public HashFunction<std::string, unsigned long> {
public:
	DJB2() : HashFunction("DJB2") {}

	DJB2(std::string arg) : HashFunction(arg, "DJB2") {}

	unsigned long apply() override {
		unsigned long hash = 5381;
		for (const char& c: argument) {
			hash = ((hash << 5) + hash) + (int)c; // hash*33 + c
		}

		return hash;
	}
};

class JenkinsHash : public HashFunction<std::string, unsigned long> {
public:
	JenkinsHash() : HashFunction("Jenkins Hash") {}

	JenkinsHash(std::string arg) : HashFunction(arg, "Jenkins Hash") {}

	unsigned long apply() override {
		unsigned long hash = 0;
		for (const char& c: argument) {
			hash += (int)c;
			hash += hash << 10;
			hash ^= hash >> 6;
		}
		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		return hash;
	}
};

#endif // HASH_FUNCTION_H
