#ifndef COMPONENT_DESCRIPTION_H
#define COMPONENT_DESCRIPTION_H
#include <string>
#include <vector>

using namespace std;

enum Component { ComInput, ComStackInput, ComAlgorithm, ComTest, ComOutput };

struct ComponentDescription
{
	Component type;
	string name;
	vector<string> parameters;
};

#endif // COMMAND_LINE_HELPER_H
