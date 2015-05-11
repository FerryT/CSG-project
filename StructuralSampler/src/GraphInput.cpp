// Todo: add nice file header

#include <cstdio>
#include <cstdlib>

#include "../headers/GraphInput.h"

#define BUFFER_SIZE 4096

//------------------------------------------------------------------------------

struct FileInput::File
{
	FILE *fp;
	char *buffer;
	char *ptr;
};

//------------------------------------------------------------------------------

void FileInput::Open()
{
	if (file)
		Close();
	
	file = new File;
	file->fp = fopen(filename.c_str(), "rt");
	file->buffer = new char[BUFFER_SIZE];
	file->ptr = file->buffer;
}

//------------------------------------------------------------------------------

void FileInput::Close()
{
	if (!file)
		return;
	
	fclose(file->fp);
	delete[] file->buffer;
	delete file;
	file = NULL;
}

//------------------------------------------------------------------------------

void FileInput::ExecuteNextUpdate()
{
}

//------------------------------------------------------------------------------
bool FileInput::IsEnd()
{
}

//------------------------------------------------------------------------------
