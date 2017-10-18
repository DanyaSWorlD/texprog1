// texprog1clr.cpp: главный файл проекта.

#include "stdafx.h"
#include <iostream>
#include <io.h>
#include <vector>
#include <locale>

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
		if (Child != 0)
			if (Child->HasNext())
				return Child->HasNext();

		if (files.size() > 0)
			return 1;
		else
			while (directories.size() > 0) {
				std::string s = directories.at(0);
				directories.erase(directories.begin());
				Child = new dir(s);
				Child->Parent = this;
				if (Child->HasNext())
					return true;
			}
		return false;
	}

private:
	dir* Parent = 0;
	dir* Child = 0;
	bool created;
	std::string Path;
	std::vector<std::string> directories;
	std::vector<std::string> files;
	std::string GetPath(std::string path = "") { return Path; };

	bool hasNext()
	{
		if (files.size() > 0)
			return 1;

		if (Child != 0)
			if ((*Child).hasNext())
				return 1;

		return 0;
	}

	std::string GetNext()
	{
		if (Child != 0)
			if (Child->hasNext())
				return Child->GetNext();

		while (directories.size() > 0)
		{
			std::string s = directories.at(0);
			directories.erase(directories.begin());
			//Child = (dir*)malloc(sizeof(dir));
			Child = new dir(s);
			Child->Parent = this;
			if (Child->hasNext())
				return Child->GetNext();
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
		delete Child;
		Child = 0;
	}
};

class FileIterator
{
public:
	dir d;
	FileIterator()
	{
		std::string src = "P:\\картинки";
		d = dir(src);
	};
	bool HasNext()
	{
		return d.HasNext();
	}
	Item Next()
	{
		return d.getFile();
	}
};

int main(array<System::String ^> ^args)
{
	setlocale(LC_ALL, "RUS");
	FileIterator it = FileIterator();
	while (it.HasNext())
	{
		Item i = it.Next();
		std::cout << i.name.c_str() << "\r\n" << i.src.c_str() << "\r\n\r\n";
	}
	system("pause");
	return 0;
}