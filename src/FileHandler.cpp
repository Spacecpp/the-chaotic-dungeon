#include "FileHandler.h"

FileHandler::FileHandler(): m_file(NULL), binary_mode(false)
{
	
}


FileHandler::~FileHandler()
{
	close();
}



bool FileHandler::open(const char * path, bool binary_mode)
{
	
	close();
	
	m_file = fopen(path, binary_mode ? "rb" : "r");
	this->binary_mode = binary_mode;
	
	return m_file != NULL;
	
}



bool FileHandler::create(const char * path, bool binary_mode, bool no_overwriting, bool secure_mode)
{
	
	close();
	
	/*
	if (no_overwriting)
	{
		if (file_exists(path) )
			return false;
	}*/
	
	m_file = fopen(path, binary_mode ? "wb" : "w");
	this->binary_mode = binary_mode;
	
	return m_file != NULL;
	
}



bool FileHandler::close()
{
	
	if (m_file)
	{
		bool ok = (fclose(m_file) == 0);
		m_file = NULL;
		return ok;
	}
	return true;
	
}



// write
bool FileHandler::write_variable(int var, int bytes)
{
	if (binary_mode)
		return (fwrite(&var, bytes, 1, m_file) > 0);
	return (fprintf(m_file, "%d ", var) > 0);
}


bool FileHandler::write_variable(unsigned int var, int bytes)
{
	if (binary_mode)
		return (fwrite(&var, bytes, 1, m_file) > 0);
	return (fprintf(m_file, "%u ", var) > 0);
}


bool FileHandler::write_variable(short int var)
{
	if (binary_mode)
		return (fwrite(&var, sizeof(short int), 1, m_file) > 0);
	return (fprintf(m_file, "%d ", var) > 0);
}


bool FileHandler::write_variable(unsigned short int var)
{
	if (binary_mode)
		return (fwrite(&var, sizeof(short int), 1, m_file) > 0);
	return (fprintf(m_file, "%u ", var) > 0);
}


bool FileHandler::write_variable(char var)
{
	if (binary_mode)
		return (fwrite(&var, sizeof(char), 1, m_file) > 0);
	return (fprintf(m_file, "%d ", var) > 0);
}


bool FileHandler::write_variable(unsigned char var)
{
	if (binary_mode)
		return (fwrite(&var, sizeof(char), 1, m_file) > 0);
	return (fprintf(m_file, "%d ", var) > 0);
}


bool FileHandler::write_variable(float var)
{
	if (binary_mode)
		return (fwrite(&var, sizeof(float), 1, m_file) > 0);
	return (fprintf(m_file, "%f ", var) > 0);
}


bool FileHandler::write_variable(double var)
{
	if (binary_mode)
		return (fwrite(&var, sizeof(double), 1, m_file) > 0);
	return (fprintf(m_file, "%f ", var) > 0);
}


bool FileHandler::write_variable(bool var)
{
	if (binary_mode)
		return (fwrite(&var, sizeof(bool), 1, m_file) > 0);
	return (fprintf(m_file, "%d ", (int)var) > 0);
}



// textmode only
bool FileHandler::write_endline()
{
	if (binary_mode)
	{	/*
		char var = '\n';
		return (fwrite(&var, sizeof(char), 1, m_file) > 0);*/
		// do nothing
		return true;
	}
	return (fprintf(m_file, "\n") > 0);
}


bool FileHandler::write_text(const char * text)
{
	if (binary_mode)
	{
		// do nothing
		return false;
	}
	return (fprintf(m_file, text) > 0);
}

bool FileHandler::write_text(const std::string & text)
{
	if (binary_mode)
	{
		// do nothing
		return false;
	}
	return (fprintf(m_file, text.c_str() ) > 0);
}



// read
bool FileHandler::read_variable(int &var, int bytes)
{
	if (binary_mode)
		return (fread(&var, bytes, 1, m_file) > 0);
	return (fscanf(m_file, "%d", &var) > 0);
}


bool FileHandler::read_variable(unsigned int &var, int bytes)
{
	if (binary_mode)
		return (fread(&var, bytes, 1, m_file) > 0);
	return (fscanf(m_file, "%u", &var) > 0);
}


bool FileHandler::read_variable(short int &var)
{
	if (binary_mode)
		return (fread(&var, sizeof(short int), 1, m_file) > 0);
	
	int dummy = 0;
	int items_read = fscanf(m_file, "%d", &dummy);
	var = (short int)dummy;
	return items_read > 0;
}


bool FileHandler::read_variable(unsigned short int &var)
{
	if (binary_mode)
		return (fread(&var, sizeof(unsigned short int), 1, m_file) > 0);
	
	unsigned int dummy = 0;
	int items_read = fscanf(m_file, "%u", &dummy);
	var = (unsigned short int)dummy;
	return items_read > 0;
}


bool FileHandler::read_variable(char &var)
{
	if (binary_mode)
		return (fread(&var, sizeof(char), 1, m_file) > 0);
	
	int dummy = 0;
	int items_read = fscanf(m_file, "%d", &dummy);
	var = (char)dummy;
	return items_read > 0;
}


bool FileHandler::read_variable(unsigned char &var)
{
	if (binary_mode)
		return (fread(&var, sizeof(unsigned char), 1, m_file) > 0);
	
	unsigned int dummy = 0;
	int items_read = fscanf(m_file, "%u", &dummy);
	var = (unsigned char)dummy;
	return items_read > 0;
}


bool FileHandler::read_variable(float &var)
{
	if (binary_mode)
		return (fread(&var, sizeof(float), 1, m_file) > 0);
	return (fscanf(m_file, "%f", &var) > 0);
}


bool FileHandler::read_variable(double &var)
{
	if (binary_mode)
		return (fread(&var, sizeof(double), 1, m_file) > 0);
	return (fscanf(m_file, "%lf", &var) > 0);
}


bool FileHandler::read_variable(bool &var)
{
	if (binary_mode)
		return (fread(&var, sizeof(bool), 1, m_file) > 0);
	
	int dummy = 0;	
	int items_read = fscanf(m_file, "%d", &dummy);
	var = (bool)dummy;
	return items_read > 0;
}



// outros
size_t FileHandler::get_file_size()
{
	
	size_t current_position = ftell(m_file);
	
	fseek(m_file, 0, SEEK_END); // caution
	size_t file_size = ftell(m_file);
	fseek(m_file, current_position, SEEK_SET);
	
	return file_size;
	
}
