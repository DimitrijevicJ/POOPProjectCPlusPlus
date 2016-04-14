#pragma once
#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <Windows.h>
using namespace std;
using namespace std::experimental::filesystem;

typedef vector<char> chars;

class File {
	path directory;
	path fileName;
public:
	File(string ss) { path h = ss; directory = absolute(h.parent_path()); fileName = h.filename(); }
	File(path pathh) { directory = absolute(pathh.parent_path()); fileName = pathh.filename(); }
	File(const File& file) { directory = file.directory; fileName = file.fileName; }

	bool operator==(File file2) { return directory == file2.directory&&fileName == file2.directory; }
	path filePath() const { return directory / fileName; }
};


#define COLOR_BLACK   0
#define COLOR_GRAY    8

#define COLOR_MAROON  4 
#define COLOR_GREEN   2
#define COLOR_NAVY    1

#define COLOR_RED     12
#define COLOR_LIME    10
#define COLOR_BLUE    9

#define COLOR_TEAL    (COLOR_NAVY | COLOR_GREEN)
#define COLOR_PURPLE  (COLOR_MAROON | COLOR_NAVY)
#define COLOR_OLIVE   (COLOR_MAROON | COLOR_GREEN)

#define COLOR_AQUA    (COLOR_BLUE | COLOR_LIME)
#define COLOR_FUCHSIA (COLOR_RED | COLOR_BLUE)
#define COLOR_YELLOW  (COLOR_RED | COLOR_LIME)

#define COLOR_SILVER  7
#define COLOR_WHITE   15


inline void colorMe(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

class NonExistingOption : public exception {
public:
	void printEx() const { 
		colorMe(COLOR_FUCHSIA);
		cout << "The option you have chosen does not exist. Please consult Help or something.." << endl; }
};
class NonExistingCommand : public exception {
public:
	void printEx() const { colorMe(COLOR_FUCHSIA);  cout << "The command you wish to execute does not exist. Please consult Help or something.." << endl; }
};

typedef vector<char> chars;
typedef unordered_map<string, File*> mapsA;


class Aliases {
public:
	mapsA map;
	void addAlias(string alias, File* object) { if (object) map.insert({ alias, object }); }
	File* findObject(string al) { if (map.count(al) == 0) return nullptr; return map.at(al); }	//catch exception
	void removeAlias(string al) { map.erase(al); }
	void loadAlias(path pathh) {
		fstream file;
		file.open(pathh.string().c_str(), std::fstream::in);
		int i = 1;
		string s, s1, s2;
		while (file >> s) {
			if (i == 1) {
				s1 = s;
				i = 2;
			}
			else {
				s2 = s;
				i = 2;
				map.insert({ s1, new File(s2) });
			}
		}
		file.close();
	}
	void saveAlias(path pathh) {
		fstream file;
		file.open(pathh.string().c_str(), std::fstream::out);
		for (auto& a : map) {
			file << a.first << " " << a.second->filePath() << "\n";
		}
		file.close();
	}
};

class Selection {
public:
	struct SelElem {
		File* elem;
		SelElem* next;
		SelElem(File* e) { elem = e; next = nullptr; }
	};
	string name;
	SelElem* first, *last;
	Selection(string name) :name(name) { first = last = nullptr; }
	void operator+=(File* file);
	void operator-=(File* file);
	void deleteSelection();
	string fetchName() const { return name; }
	void copySelection();
};

typedef vector<char> chars;
typedef unordered_map<string, Selection*> mapsS;
class Simbolics {
public:
	mapsS map;
	void addSimb(string sim, Selection* sel) { if (sel) map.insert({ sim, sel }); }
	Selection* findObject(string al) { if (map.count(al) == 0) return nullptr; return map.at(al); } //catch exception
	void removeAlias(string al) { map.erase(al); }
	void loadSimbolic(path pathh) {
		fstream file;
		file.open(pathh.string().c_str(), std::fstream::in);
		int i = 1;
		string s, s1, s2;
		while (file >> s) {
			if (i == 1) {
				s1 = s;
				i = 2;
			}
			else {
				s2 = s;
				i = 2;
				map.insert({ s1, new Selection(s2) });
			}
		}
		file.close();
	}
	void saveASimbolic(path pathh) {
		fstream file;
		file.open(pathh.string().c_str(), std::fstream::out);
		for (auto& a : map) {
			file << a.first << " " << a.second->fetchName() << "\n";
		}
		file.close();
	}
};

class Selections {
public:
	struct selectionsList {
		Selection* selection;
		selectionsList* next;
		selectionsList(Selection* s) { selection = s; next = nullptr; }
	};
	static selectionsList* selections;
	static void addSelection(Selection* a) { selectionsList* elem = new selectionsList(a); elem->next = selections; selections = elem; }
	static Selection* findSelection(string sl);
	static void removeSelection(string sl);
	static void saveSelections(path pathh);
	static void loadSelections(path pathh);
	static bool isSelection(string sel);
};
