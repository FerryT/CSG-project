#include "../headers/Tests.h"

void QualityTest::RunTest(string outputFile)
{
	while (!this->input->IsEnd())
	{
		for (int i = 0; !this->input->IsEnd() && i < this->SnapshotSize; i++)
		{
			this->input->ExecuteNextUpdate();
		}


	}

}
