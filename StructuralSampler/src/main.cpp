
#include <cstdlib>

#include "GraphInput.h" // debug

int main(int argc, char *argv[])
{
	// debug [
	{
		FileInput file("../../sampledata/fruits");
		file.Open();
		while (!file.IsEnd())
			file.ExecuteNextUpdate();
		file.Close();
	}
	// debug ]
	return EXIT_SUCCESS;
}
