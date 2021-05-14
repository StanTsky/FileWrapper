#ifdef __linux__
#include <unistd.h> // read, write
#include <errno.h> // ENOENT
#include <fcntl.h> // O_RDONLY and O_WRONLY
#include <stdio.h>
#else
// assume any other platform is Windows
#include <Windows.h>
#include <stdio.h>
#endif

class FileWrapper
{
#ifdef __linux__
private:
	// ... Linux FileWrapper code goes here
	int _handle;

public:
	FileWrapper(const char * file, int flags, mode_t mode = 0700)
	{
		this->_handle = open(file, flags, mode);
	}

	~FileWrapper()
	{
		close(this->_handle);
	}

	int getHandle() { return this->_handle; }
#else
private:
	// ... Windows FileWrapper here
	HANDLE _handle;

public:
	FileWrapper(const char * file, DWORD access, DWORD creation)
	{
		this->_handle = CreateFileA(file, access, 0, NULL, creation, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	~FileWrapper()
	{
		CloseHandle(this->_handle);
	} // http://msdn.microsoft.com/en-us/library/windows/desktop/ms724211(v=vs.85).aspx

	HANDLE getHandle() { return this->_handle; }
#endif
};

void cp(const char * src, const char * dest)
{
	char buffer[20];
#ifdef __linux__
	// Linux code goes here
	// int infile, outfile;
	int numberOfBytesRead;

	FileWrapper infile(src, O_RDONLY);

	if (infile.getHandle() == ENOENT)
	{
		printf("Could not find file!");
		// return 1;
	}

	FileWrapper outfile(dest, O_WRONLY | O_CREAT);

	while ((numberOfBytesRead = read(infile.getHandle(), buffer, 20)))
		write(outfile.getHandle(), buffer, numberOfBytesRead);

	// close(infile);
	// close(outfile);
#else
	// Windows code goes here
	//HANDLE infile;
	//HANDLE outfile;
	DWORD numberOfBytesRead;
	DWORD numberOfBytesWritten;

	// args[0] is the name of the program (historically)


	FileWrapper infile(src, GENERIC_READ, OPEN_EXISTING);
	if (GetLastError() == ERROR_FILE_NOT_FOUND)
	{
		printf("Could not find file!\n");
		//system("pause");
		exit(EXIT_FAILURE);							// exit program
		// return 1;
	}

	// Open destination file
	FileWrapper outfile(dest, GENERIC_WRITE, CREATE_ALWAYS);

	//infile = CreateFileA(outfile,
	//	GENERIC_READ, 0, NULL, OPEN_EXISTING,
	//	FILE_ATTRIBUTE_NORMAL, NULL);
	//if (GetLastError() == ERROR_FILE_NOT_FOUND)
	//{
	//	printf("Could not find file");
	//	return 1;
	//}
	//outfile = CreateFileA(dest, GENERIC_WRITE, 0,
	//	NULL, CREATE_ALWAYS,
	//	FILE_ATTRIBUTE_NORMAL, NULL);

	while (true)
	{
		ReadFile(infile.getHandle(), buffer, 20, &numberOfBytesRead, NULL);
		if (numberOfBytesRead == 0)
			break;

		WriteFile(outfile.getHandle(), buffer, numberOfBytesRead, &numberOfBytesWritten, NULL);
	}

	// CloseHandle(infile);
	// CloseHandle(outfile);
#endif
}

int main()
{
	// Don't ask user for file
	cp("test.txt", "test2.txt");

	//system("pause");
	return 0;
}