#ifndef COMMAND_LINE_HELPER_H
#define COMMAND_LINE_HELPER_H
#include <vector>

#include "GraphInput.h"
#include "Tests.h"
#include "Algorithm.h"
#include "ComponentDescription.h"

using namespace std;

vector<ComponentDescription> GetDescriptions(int argc, char *argv[]);
vector<ComponentDescription> GetDescriptions(vector<string> argv);

StackInput* CreateStackInput(ComponentDescription desc);
vector<StackInput*> CreateStackInputs(vector<ComponentDescription> arguments);
Test* CreateTest(ComponentDescription desc);
Algorithm* CreateAlgorithm(ComponentDescription desc);
vector<Algorithm*> CreateAlgorithms(vector<ComponentDescription> arguments);
Input* CreateInput(ComponentDescription desc);
string GetOutputFilename(ComponentDescription desc);

template<Component c>
bool IsType(ComponentDescription desc) { return desc.type == c; }

#endif // COMMAND_LINE_HELPER_H
