// Todo: add nice file header

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>

#include "GraphInput.h"

#define BUFFER_SIZE 4096

//------------------------------------------------------------------------------

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
	
	if (*str == '%')
		return read_line(str, num, fp);
	return str;
}

//------------------------------------------------------------------------------

struct FileInput::File
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

void FileInput::Open()
{
	if (file)
		Close();
	
	file = new File;
	file->fp = fopen(filename.c_str(), "rt");
	file->buffer = new char[BUFFER_SIZE];
	file->ptr = file->buffer;
	file->index = 0;
	
	if (!read_line(file->buffer, BUFFER_SIZE, file->fp))
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
	
	// Reset buffer
	file->ptr = file->buffer;
	*file->ptr = 0;
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
// Todo: support multiple formats
void FileInput::ExecuteNextUpdate()
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
		if (!read_line(file->buffer, BUFFER_SIZE, file->fp))
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
		// Todo: output (file->index,dest) as an edge.
		printf("%d %d\n", file->index, dest); // debug
	}
	else // Probably hit end of line, try again
		FileInput::ExecuteNextUpdate();
}

//------------------------------------------------------------------------------

bool FileInput::IsEnd()
{
	return !file;
}

//------------------------------------------------------------------------------
void Input::SetAlgorithm(Algorithm* algoritm)
{
	this->Output = algoritm;
}