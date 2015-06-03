#ifndef COMPONENT_DESCRIPTION_H
#define COMPONENT_DESCRIPTION_H

#include "Graph.h"

enum Component { ComInput, ComStackInput, ComAlgorithm, ComTest, ComOutput };

struct ComponentDescription
{
	Component type;
	String name;
	Strings parameters;
};

typedef std::vector<ComponentDescription> ComponentDescriptions;

#endif // COMMAND_LINE_HELPER_H
