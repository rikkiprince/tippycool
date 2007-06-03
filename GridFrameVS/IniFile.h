
#ifndef __INI_FILE_H__
#define __INI_FILE_H__

#include "headers.h"
#include <ctype.h>


class IniFile
{
protected:
	#define MAX_FILENAME	(512)
	char filename[MAX_FILENAME];

public:
	IniFile(const char *fn);
	~IniFile();

	void getString(const char *section, const char *key, char *buffer, int size);
	int getInt(const char *section, const char *key);
	Orientation getOrientation(const char *section);
	Orientation getOrientation(const char *section, const char *key);
};

#endif // __INI_FILE_H__
