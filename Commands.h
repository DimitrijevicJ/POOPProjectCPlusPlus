#pragma once
#include <iomanip>
#include "Header.h"


extern Aliases* AliasesMap;
extern Simbolics* SimbolicsMap;

void end() {
	colorMe(COLOR_SILVER);
	cout << "Would you like to save your simbolics and selections?" << endl;
	string answer;
	cin >> answer;
	if (answer == "yes") {
		cout << "Where would you like to save your simbolics?" << endl;
		cin >> answer;
		path pathSelections = answer;
		SimbolicsMap->saveASimbolic(pathSelections);
		cout << "Where would you like to save your selections?" << endl;
		cin >> answer;
		pathSelections = answer;
		Selections::saveSelections(pathSelections);
	}
	cout << "Would you like to save your Aliases?" << endl;
	cin >> answer;
	if (answer == "yes") {
		cout << "Where would you like to save your aliases?" << endl;
		cin >> answer;
		path pathSelections = answer;
		AliasesMap->saveAlias(pathSelections);
	}

	exit(0);
}

class MakeDirectory {
public:
	void run(chars params, string param1, string param2) {
		//mkdir directoryName

		//check if the parameters is an alias
		//if alias it will return a file with path
		//if not alias it will return nullptr then we use param1
		path pathh;
		if (AliasesMap->map.count(param1))
		{	
			File* file = AliasesMap->findObject(param1);
			pathh = file->filePath();
		}
		else pathh = param1;
		if (exists(absolute((new File(pathh))->filePath()))) {
			cout << "That directory already exists" << endl;
			return;
		}
		if (!pathh.is_absolute()) pathh = current_path() /= pathh;
		if (create_directories(pathh)) { cout << "Well done! You are a young prodigy. You have succesfuly created a directory!" << endl; }
		else { cout << "You are a failure. You can't even create a simple directory." << endl; }
	}
};

class MakeFile {
public:
	void run(chars params, string param1, string param2) {
		//mkdir directoryName
		path pathh;
		if (AliasesMap->map.count(param1))
		{
			File* file = AliasesMap->findObject(param1);
			pathh = file->filePath();
		} 
		else pathh = param1;
		if (!exists(absolute((new File(pathh.parent_path()))->filePath()))) {
			(new MakeDirectory())->run(params, pathh.parent_path().string(), param2);
		}
		if (exists(pathh)) {
			cout << "There already exists a file with such name. Please choose another" << endl; return;
		}
		ofstream ofstr(pathh);
		cout << "You created a file! You are amazing!" << endl;
	}
};

class ChangeDirectory {
public:
	void run(chars params, string param1, string param2) {
		path pathh;
		if (AliasesMap->map.count(param1))
		{
			File* file = AliasesMap->findObject(param1);
			pathh = file->filePath();
		}
		else pathh = param1;
		if (param1 == "C:/") { current_path("C:/"); return; }
		if (!exists(absolute((new File(pathh.parent_path()))->filePath()))) { cout << "You are lost. No such directory." << endl; return; }
		if (!is_directory(pathh)) { cout << "Not cool bro. That's not a directory." << endl; return; }
		if (param1 == "..") pathh = current_path().parent_path();
		current_path(pathh);
		cout << "You are much smarter than Hansel and Grettel. You found your way to : " << current_path() << endl;

	}
};

class PrintDirectoryContents {
public:
	void run(chars params, string param1, string param2) {
		//dir -p ->permissions
		//dir -a ->all
		//dir -s ->size		//space(directoryname)
		//dir -d -> date
		//dir -ps ->permissions and size ... 
		int options = 0;
		for (char c : params) {
			switch (c)
			{
			case 'a': {options |= 0x7; break; }
			case 'p': {options |= 0x1; break; }
			case 's': {options |= 0x2; break; }
			case 'd': {options |= 0x4; break; }
			default: {throw new NonExistingOption(); }
			}
		}

		for (auto& p : directory_iterator(current_path()))
		{
			fileType(p.status());
			if (options & 1) {// permissions
				permissions(p.status());
			}
			if (options & 2) {//size
				struct stat filestatus;
				stat(p.path().string().c_str(), &filestatus);
				cout << setw(13);
				cout << filestatus.st_size << " B	";
			}
			if (options & 4) {//date
				auto ftime = last_write_time(p);
				time_t cftime = decltype(ftime)::clock::to_time_t(ftime);
				string stream;
				stream = std::asctime(std::localtime(&cftime));
				stream.erase(stream.length() - 1, 1);
				cout << stream<<"	";
			}
			cout << p.path().filename() << endl;
		}
	}
private:
	void permissions(file_status stat) {
		perms per = stat.permissions();

		cout << ((per & perms::owner_read) == perms::owner_read ? "r" : "-");
		cout << ((per & perms::owner_write) == perms::owner_write ? "w" : "-");
		cout << ((per & perms::owner_exec) == perms::owner_exec ? "x" : "-");
		cout << ((per & perms::group_read) == perms::group_read ? "r" : "-");
		cout << ((per & perms::group_write) == perms::group_write ? "w" : "-");
		cout << ((per & perms::group_exec) == perms::group_exec ? "x" : "-");
		cout << ((per & perms::others_read) == perms::others_read ? "r" : "-");
		cout << ((per & perms::others_write) == perms::others_write ? "w" : "-");
		cout << ((per & perms::others_exec) == perms::others_exec ? "x" : "-");
		cout << "	";
	}
	void fileType(file_status stat) {
		cout << left << setw(7);
		if (is_regular_file(stat)) std::cout << "     ";
		if (is_directory(stat)) std::cout << "<DIR>";
		//if (is_symlink(stat)) std::cout << " is a symlink\n";
	}
};

extern int movee;

class Copy {
public:
	void run(chars params, string param1, string param2) {
		path path1, path2;
		int is_simbolic = 0;
		Selection* select=nullptr;
		if (AliasesMap->map.count(param1))
			{
				File* file = AliasesMap->findObject(param1);
				path1 = file->filePath();
			}
		else path1 = param1;
		if (SimbolicsMap->map.count(param1)) {
			//param1 is a simbolic name, we need to copy a selection
			is_simbolic = 1; 
			select = SimbolicsMap->findObject(param1);
		}
		if (AliasesMap->map.count(param2))
			{
				File* file = AliasesMap->findObject(param2);
				path2 = file->filePath();
			}
		else path2 = param2;
		if (is_simbolic || Selections::isSelection(param1)) {
			if (select == nullptr) select = Selections::findSelection(param1);
			directory_entry oldDir(current_path());
			current_path(param2);
			select->copySelection();
			current_path(oldDir);
			return;
		}
		directory_entry fileWhat(path1), fileWhere(path2);

		if(!exists(fileWhat)) {
			cout << "File you are trying to copy doesn't exist" << endl; return;
		}
		int i = 1;
		//destination directory exists
		if (exists(fileWhere)) {
			if(is_directory(fileWhere)){
				string par = absolute(fileWhat.path()).string();
				directory_entry oldDir(current_path());
				current_path(fileWhere);
				if (!is_directory(par)) {
					(new Copy())->run(params, par, fileWhat.path().filename().string());
				}
				else {
					for (auto& p : directory_iterator(par)) {
						if (p != fileWhat) (new Copy())->run(params, p.path().string(), p.path().filename().string());
					}
				}
				current_path(oldDir);
				return;
			}
			else {
				while (exists(fileWhere))
				{
					char buffer[10];
					_itoa(i++, buffer, 10);
					param2.insert(param2.find_last_of('.'), buffer);
					if (AliasesMap->map.count(param2))
					{
						File* file = AliasesMap->findObject(param2);
						path2 = file->filePath();
					}
					else path2 = param2;
					fileWhere.assign(path2);
				}
			}
		}
		if (!exists(absolute(fileWhere.path().parent_path()))) {
			(new MakeDirectory())->run(params, fileWhere.path().parent_path().string(),param2);
		}
		if (params.size()>0&&params[0] == 'd') {
			std::experimental::filesystem::copy(fileWhat, fileWhere, copy_options::directories_only);
			int countCopied = 1;
			for (auto p : directory_iterator(fileWhere)) { countCopied++; }
			cout << "Bravo! You have successfully copied those files. Total copied files : " << countCopied << endl;
			return;
		}
		else if (params.size()==0 || params[0] == 'r') {
			std::experimental::filesystem::copy(fileWhat, fileWhere, copy_options::recursive);
			int countCopied = 1;
			for (auto p : recursive_directory_iterator(fileWhere)) { countCopied++; }
			if (movee) cout << "Those files are now in a better place. Total files moved : " << countCopied << endl;
			else cout << "Bravo! You have successfully copied dthose files. Total copied files : " << countCopied << endl;
			return;
		}
		else throw new NonExistingOption();
	}
};

class Delete {
public:
	void run(chars params, string param1, string param2) {

		directory_entry file(param1);
		if (!exists(file)) {
			cout << "File doesn't exist" << endl; return;
		}

		if (file.path() == current_path()) {
			colorMe(COLOR_FUCHSIA);
			cout << "You are cutting the branch you are standing on. No."<<endl; 
			colorMe(COLOR_SILVER);
			return;
		}
		if (!movee) {
			if (!(param2 == "WhosYourDaddy")) {
				cout << "Are you sure you want to delete those files?" << endl;
				string answer;
				cin >> answer;
				if (answer == "no") return;
				cout << "Are you absolutely sure?" << endl;
				answer;
				cin >> answer;
				if (answer == "no") return;
				cout << "You know there's no comming back from this, right?" << endl;
				answer;
				cin >> answer;
				if (answer == "no") return;
			}
		}
		std::uintmax_t deletedFiles = 1;
		if (params.size()>0 && params[0] == 'a') deletedFiles = remove_all(file);
		else
			if (params.size()==0 || params[0] == 'f') { 
				try {
					deletedFiles = remove(file);
				}
				catch (...) {
					string answer;
					cout << "You can't delete that file, it's full of stuff. Type a secret word to delete it" << endl;
					cin >> answer;
					if (answer == "please" || answer == "Please") remove_all(file);
				}
			}
			else throw NonExistingOption();
			if (!movee) cout << "It's not nice, deleting other people's stuff, but you did it." << endl;
	}
};

class Move {
public:
	void run(chars params, string param1, string param2) {
		movee = 1;
		params.resize(1);
		params[0] = 'r';
		param2.append("/"); string fname = param1.substr(param1.find_last_of("/") + 1);
		param2.append(fname);
		(new Copy())->run(params, param1, param2);
		params[0] = 'a';
		(new Delete())->run(params, param1, param2);
		movee = 0;
	}
};

class Select {
public:
	void run(chars params, string param1, string param2) {
		Selection* select = new Selection(param1);
		for (auto& f : recursive_directory_iterator(current_path())) {
			string ss;
			if (params.size()>0&&params[0] == 'p') {
				ss = absolute(f.path()).string();
			}
			else if (params.size()==0 || params[0] == 'f') {
				ss = f.path().filename().string();
			}
			else throw NonExistingOption();
			if (match(const_cast<char*>(param2.c_str()), const_cast<char*>(ss.c_str()))) 
				(*select) += new File(absolute(f.path()));
		}
		Selections::addSelection(select);
	}
	//Reference : This awesome function has been donated by some brilliant internet user, and I will use this opportunity to thank him
	//http://www.geeksforgeeks.org/wildcard-character-matching/
	bool match(char *first, char * second)
	{
		// If we reach at the end of both strings, we are done
		if (*first == '\0' && *second == '\0')
			return true;

		// Make sure that the characters after '*' are present
		// in second string. This function assumes that the first
		// string will not contain two consecutive '*'
		if (*first == '*' && *(first + 1) != '\0' && *second == '\0')
			return false;

		// If the first string contains '?', or current characters
		// of both strings match
		if (*first == '?' || *first == *second)
			return match(first + 1, second + 1);

		// If there is *, then there are two possibilities
		// a) We consider current character of second string
		// b) We ignore current character of second string.
		if (*first == '*')
			return match(first + 1, second) || match(first, second + 1);
		return false;
	}

};

class Assign {
public:
	void run(chars params, string param1, string param2) {
		if (params.size()==0 || params[0] == 'f') {
			AliasesMap->addAlias(param1, new File(param2));
			cout << "Great name! You added alias : " << param1 << endl;
			return;
		}
		else if (params.size()>0 && params[0] == 's') {
			SimbolicsMap->addSimb(param1, Selections::findSelection(param2));
			cout << "You are so creative! You added simbolic name : " << param1 << endl;
			return;
		}
		else throw new NonExistingOption();
	}
};

class Command {
public:
	static void parseCommand(string command) {
		istringstream s(command);
		string com, word; s >> com;
		chars params(4);
		string param1 = "", param2 = ""; int numParams = 0;
		while (s >> word) {
			if (word[0] == '-') {
				word.erase(0, 1);
				unsigned int i = 0;
				while (i < (int)word.length()) params[numParams++] = word[i++];
			}
			else {
				if (param1 == "") param1 = word;
				else param2 = word;
			}
		}
		params.resize(numParams);
		if (com == "MakeDirectory" || com=="mkdir") { (new MakeDirectory())->run(params, param1, param2); return; }
		else if (com == "MakeFile" || com=="mkfile") { (new MakeFile())->run(params, param1, param2); return; }
		else if (com == "ChangeDirectory" || com=="cd") { (new ChangeDirectory())->run(params, param1, param2); return; }
		else if (com == "PrintDirectoryContents" || com=="dir") { (new PrintDirectoryContents())->run(params, param1, param2); return; }
		else if (com == "Copy" || com=="cp") { (new Copy())->run(params, param1, param2); return; }
		else if (com == "Delete" || com=="rm") { (new Delete())->run(params, param1, param2); return; }
		else if (com == "Move" || com=="mv") { (new Move())->run(params, param1, param2); return; }
		else if (com == "Select" || com == "sl") { (new Select())->run(params, param1, param2); return; }
		else if (com == "Assign" || com == "as") { (new Assign())->run(params, param1, param2); return; }
		else if (com == "Exit" || com == "q") end();
		else if (com == "") return;
		else throw new NonExistingCommand();
	}
};

void Selection::copySelection() {
	SelElem* current = first;
	while (current) {
		chars params;
		(new Copy())->run(params, current->elem->filePath().string(), current_path().string());
		current = current->next;
	}
}

