
#include "IniFile.h"

IniFile::IniFile(const char *fn)
{
	strcpy(this->filename, fn);
}

IniFile::~IniFile()
{
}

void IniFile::getString(const char *section, const char *key, char *buffer, int size)
{
	GetPrivateProfileString(section, key, NULL, buffer, size, this->filename);
}

int IniFile::getInt(const char *section, const char *key)
{
	return GetPrivateProfileInt(section, key, NULL, this->filename);
}

char* lcase(char s[])
{
	for(int i=0; i<strlen(s); i++)
		s[i] = tolower(s[i]);
	return s;
}

Orientation IniFile::getOrientation(const char *section)
{
	return getOrientation(section, "orientation");
}

Orientation IniFile::getOrientation(const char *section, const char *key)
{
	char buf[10];
	getString(section, key, buf, 10);
	lcase(buf);
	if(strcmp(buf, "top") == 0)
		return TOP;
	else if(strcmp(buf, "bottom") == 0)
		return BOTTOM;
	else if(strcmp(buf, "left") == 0)
		return LEFT;
	else if(strcmp(buf, "right") == 0)
		return RIGHT;
	else if(strcmp(buf, "front") == 0)
		return FRONT;
	else if(strcmp(buf, "back") == 0)
		return BACK;
	else return none;
}
