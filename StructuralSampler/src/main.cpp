
#include <cstdlib>

#include "GraphInput.h" // debug

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("Usage:\n\tStructuralSampler <graph file>\t");
		return EXIT_FAILURE;
	}
	
	try {
		// debug [
		FileInput file(argv[1]);
		file.Open();
		while (!file.IsEnd())
			file.ExecuteNextUpdate();
		file.Close();
		// debug ]
	} catch (const char *msg) {
		fprintf(stderr, "Fatal error:\n\t%s\n", msg);
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
