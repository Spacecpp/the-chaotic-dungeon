#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include <string>

// class to easily write and read variables from a file, both in text and binary mode
class FileHandler
{
	
	private:
		FILE * m_file;
		bool binary_mode;
	
	public:
		FileHandler();
		~FileHandler();
		
		bool open(const char * path, bool binary_mode);
		bool create(const char * path, bool binary_mode, bool no_overwriting = false, bool secure_mode = false);
		bool close();
		
		// write
		bool write_variable(int var, int bytes = sizeof(int) );
		bool write_variable(unsigned int var, int bytes = sizeof(int) );
		bool write_variable(short int var);
		bool write_variable(unsigned short int var);
		bool write_variable(char var);
		bool write_variable(unsigned char var);
		bool write_variable(float var);
		bool write_variable(double var);
		bool write_variable(bool var);
		
		// textmode only
		bool write_endline();
		bool write_text(const char * text);
		bool write_text(const std::string & text);
		
		// read
		bool read_variable(int &var, int bytes = sizeof(int) );
		bool read_variable(unsigned int &var, int bytes = sizeof(int) );
		bool read_variable(short int &var);
		bool read_variable(unsigned short int &var);
		bool read_variable(char &var);
		bool read_variable(unsigned char &var);
		bool read_variable(float &var);
		bool read_variable(double &var);
		bool read_variable(bool &var);
		
		// outros
		size_t get_file_size();
		
		// inline
		inline bool is_binary_mode() { return binary_mode; }
		inline FILE * get_FILE() { return m_file; }
		
};
#endif
