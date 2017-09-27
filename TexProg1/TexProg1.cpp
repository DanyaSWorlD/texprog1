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
	dir* Parent = 0;
	dir* Child = 0;
	bool created;
	std::string Path;
	std::vector<std::string> directories;
	std::vector<std::string> files;
	dir() {};
	dir(std::string path) {
		created = true;
		Path = path;
		using namespace Runtime::InteropServices;

		std::string str = GetPath();
		for each (String^ s in Directory::GetDirectories(gcnew String(GetPath().c_str())))
			directories.push_back(std::string((const char*)(Marshal::StringToHGlobalAnsi(s).ToPointer())));

		for each (String^ s in Directory::GetFiles(gcnew String(GetPath().c_str())))
			files.push_back(std::string((const char*)(Marshal::StringToHGlobalAnsi(s).ToPointer())));
	};

	Item getFile() {
		Item it;
		it.src = GetNext();
		it.name = it.src.substr(it.src.find_last_of("\\") + 1, std::string::npos);
		return it;
	};

	bool HasNext()
	{
		if (files.size() > 0)
			return 1;

		if (Child != 0)
			if ((*Child).HasNext())
				return 1;

		return 0;
	}
private:
	std::string GetPath(std::string path = "")
	{
		//if (Parent != 0)
			//path = (*Parent).GetPath(path) + '/' + Path + path;
		//else
			//return Path;
		//return path;
		return Path;
	};

	std::string GetNext()
	{
		if (Child != 0)
			return (*Child).GetNext();

		while (directories.size() > 0)
		{
			std::string s = directories.at(0);
			directories.erase(directories.begin());
			//Child = (dir*)malloc(sizeof(dir));
			dir d = dir(s);
			Child = &d;
			(*Child) = d;
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
		Child = 0;
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
		std::string src = "E:\\torrent\\Prey  by xatab";

		if (src.find("//") != -1)
			src.replace(src.find("//"), 2, "\\");
		if (src.find("/") != -1)
			src.replace(src.find("/"), 1, "\\");
		d = dir(src);
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
		std::string name = it.Next().name;
		printf("%s\r\n", name);
	}
	return 0;
}