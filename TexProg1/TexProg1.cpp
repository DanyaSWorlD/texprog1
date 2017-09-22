// texprog1clr.cpp: главный файл проекта.

#include "stdafx.h"
#include <io.h>
#include <vector>

using namespace System;
using namespace System::IO;
using namespace System::Text;

class Item
{
public:
	Item() {};
	~Item() {};
	std::string src;
	std::string name;
};


class dir {
public:
	dir* Parent;
	dir* Child;
	bool created;
	std::string Path;
	std::vector<std::string> directories;
	std::vector<std::string> files;
	dir() {};
	dir(std::string path) {
		created = true;
		Path = path;
		using namespace Runtime::InteropServices;

		for each (String^ s in Directory::GetDirectories(gcnew String(GetPath().c_str())))
			directories.push_back(std::string((const char*)(Marshal::StringToHGlobalAnsi(s).ToPointer())));

		for each (String^ s in Directory::GetFiles(gcnew String(GetPath().c_str())))
			files.push_back(std::string((const char*)(Marshal::StringToHGlobalAnsi(s).ToPointer())));
	};

	Item getFile() {
		Item it;
		it.name = GetNext();
		it.src = GetPath() + "/" + it.name;
		return it;
	};

	bool HasNext()
	{
		if (files.size() > 0)
			return 1;

		if ((*Child).created)
			if ((*Child).HasNext())
				return 1;

		return 0;
	}
private:
	std::string GetPath(std::string path = "")
	{
		if ((*Parent).created)
			path = (*Parent).GetPath(path) + '/' + Path + path;
		return path;
	};

	std::string GetNext()
	{
		if ((*Child).created)
			return (*Child).GetNext();

		while (directories.size() > 0)
		{
			std::string s = directories.at(0);
			directories.erase(directories.begin());
			(*Child) = dir(GetPath() + "/" + s);
			(*Child).Parent = this;
			if ((*Child).HasNext())
				return (*Child).GetNext();
			else
				deleteChild();
		}

		if (files.size() > 0)
		{
			std::string s = files.at(0);
			files.erase(files.begin());
			return s;
		}

		return "";
	};

	void deleteChild()
	{
		(*Child).created = false;
		delete Child;
	}
};

/*class Iterator
{
	virtual bool HasNext();
	virtual Item Next();
};*/

class FileIterator //: Iterator
{
public:
	dir d;
	FileIterator()
	{
		d = dir("c:/");
	};
	//bool DataUsed = true;
	bool HasNext()
	{
		//if (DataUsed == false)
		//return true;
		return d.HasNext();
	}
	Item Next()
	{
		//DataUsed = true;
		return d.getFile();
	}
};

int main(array<System::String ^> ^args)
{
	FileIterator it = FileIterator();
	while (it.HasNext())
	{
		printf("%s\r\n", it.Next().src);
	}
	return 0;
}