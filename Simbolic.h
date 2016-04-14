#pragma once
#include "Header.h"
using namespace std;
using namespace std::experimental::filesystem;

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
	void addSimb(string sim, Selection* sel) {if(sel) map.insert({sim, sel });}
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
			file << a.first << " " << a.second->fetchName()<< "\n";
		}
		file.close();
	}
};

/*class Selection {
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
};*/

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
};

