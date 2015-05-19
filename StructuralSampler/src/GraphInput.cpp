// Todo: add nice file header

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

#include "GraphInput.h"

#define BUFFER_SIZE 4096

//------------------------------------------------------------------------------

template <char delim>
char *read_line(char *str, int num, FILE *fp)
{
	char *ret = fgets(str, num, fp);
	if (!ret)
		return NULL;
	
	size_t len = strlen(str);
	if (!feof(fp) && str[len - 1] != '\n')
		throw "Buffer overflow whilst reading graph file.";
	
	while (*str && isblank(*str))
		++str;
	
	if (*str == delim)
		return read_line<delim>(str, num, fp);
	return str;
}

//==============================================================================

struct MGraphFileInput::File
{
	FILE *fp;
	char *buffer;
	char *ptr;
	
	// Header info
	size_t num_nodes;
	size_t num_edges;
	unsigned short format;
	unsigned short num_cons;
	
	// Number of lines read
	size_t index;
};

//------------------------------------------------------------------------------

void MGraphFileInput::Open()
{
	if (file)
		Close();
	
	FILE *fp = fopen(filename.c_str(), "rt");
	if (!fp)
		throw "Unable to open graph file for reading.";
	
	file = new File;
	file->fp = fp;
	file->buffer = new char[BUFFER_SIZE];
	file->ptr = file->buffer;
	file->index = 0;
	
	if (!read_line<'%'>(file->buffer, BUFFER_SIZE, file->fp))
	{
		Close();
		return;
	}
	
	// Parse header
	file->num_nodes = strtoul(file->ptr, &file->ptr, 10);
	file->num_edges = strtoul(file->ptr, &file->ptr, 10);
	file->format = (unsigned short) strtoul(file->ptr, &file->ptr, 2);
	file->num_cons = (unsigned short) strtoul(file->ptr, &file->ptr, 10);
	if (!file->num_cons)
		file->num_cons = 1;
	
	if (file->format)
		throw "Graph files with weights are currently not supported.";
	
	// Reset buffer
	file->ptr = file->buffer;
	*file->ptr = 0;
}

//------------------------------------------------------------------------------

void MGraphFileInput::Close()
{
	if (!file)
		return;
	
	fclose(file->fp);
	delete[] file->buffer;
	delete file;
	file = NULL;
}

//------------------------------------------------------------------------------
// Todo: support multiple formats
void MGraphFileInput::ExecuteNextUpdate()
{
	if (!file)
		return;
	
	// Scan for next digit or end of line;
	while (isspace(*file->ptr))
		++file->ptr;
	
	// End of line, try the next one
	if (*file->ptr == '\0')
	{
		if (file->index == file->num_nodes)
		{
			// Number of nodes specified met
			Close();
			return;
		}
		if (!read_line<'%'>(file->buffer, BUFFER_SIZE, file->fp))
		{
			// End of input
			Close();
			return;
		}
		++file->index;
		file->ptr = file->buffer;
	}
	
	// Read next destination node
	size_t dest = strtoul(file->ptr, &file->ptr, 10);
	if (dest)
	{
		if (algorithm)
			algorithm->Add(Edge(file->index, dest));
		printf("%d %d\n", file->index, dest); // debug
	}
	else // Probably hit end of line, try again
		ExecuteNextUpdate();
}

//------------------------------------------------------------------------------

bool MGraphFileInput::IsEnd()
{
	return !file;
}

//==============================================================================

struct EdgeFileInput::File
{
	FILE *fp;
	char *buffer;
};

//------------------------------------------------------------------------------

void EdgeFileInput::Open()
{
	if (file)
		Close();
	
	FILE *fp = fopen(filename.c_str(), "rt");
	if (!fp)
		throw "Unable to open graph file for reading.";
	
	file = new File;
	file->fp = fp;
	file->buffer = new char[BUFFER_SIZE];
}

//------------------------------------------------------------------------------

void EdgeFileInput::Close()
{
	if (!file)
		return;
	
	fclose(file->fp);
	delete[] file->buffer;
	delete file;
	file = NULL;
}

//------------------------------------------------------------------------------

void EdgeFileInput::ExecuteNextUpdate()
{
	if (!file)
		return;
	
	if (!read_line<'#'>(file->buffer, BUFFER_SIZE, file->fp))
	{
		// End of input
		Close();
		return;
	}
	
	const char *ptr = file->buffer;
	while (isspace(*ptr))
		++ptr;
	
	if (*ptr == '\0')
	{
		// Empty line, try again
		ExecuteNextUpdate();
		return;
	}
	
	unsigned long src, dst;
	if (sscanf(ptr, "%lu %lu", &src, &dst) < 2)
	{
		// Error in input
		throw "Invalid input detected whilst reading graph file.";
	}
	
	if (algorithm)
		algorithm->Add(Edge(src, dst));
	printf("%lu %lu\n", src, dst); // debug
}

//------------------------------------------------------------------------------

bool EdgeFileInput::IsEnd()
{
	return !file;
}

//------------------------------------------------------------------------------

void StackInput::Add(const Edge& e)
{
	this->algorithm->Add(e);
}

//------------------------------------------------------------------------------


void StackInput::Remove(const Edge& e)
{
	this->algorithm->Remove(e);
}

//------------------------------------------------------------------------------

void StackInput::Open()
{
	this->input->Open();
}

//------------------------------------------------------------------------------

void StackInput::Close()
{
	this->input->Close();
}

//------------------------------------------------------------------------------

void StackInput::ExecuteNextUpdate()
{
	this->input->ExecuteNextUpdate();
}

//------------------------------------------------------------------------------

bool StackInput::IsEnd()
{
	return this->input->IsEnd();
}

//------------------------------------------------------------------------------

void StackInput::SetInternalInput(Input* input)
{
	this->input = input;
	input->SetAlgorithm(this);
}