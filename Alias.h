/*#pragma once

#include <fstream>
#include <iostream>
#include <experimental/filesystem>
#include <string>
#include <sstream>
#include <bitset>
#include <vector>
#include <filesystem>
#include "Header.h"
#include <unordered_map>
using namespace std;
using namespace std::experimental::filesystem;

typedef vector<char> chars;
typedef unordered_map<string, File*> mapsA;


class Aliases {
public:
	mapsA map;
	void addAlias(string alias, File* object) {if (object) map.insert({ alias, object }); }
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
};*/

