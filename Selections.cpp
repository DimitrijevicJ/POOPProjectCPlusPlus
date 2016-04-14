#include "Header.h"

Selections::selectionsList* Selections::selections = nullptr;
extern Simbolics* SimbolicsMap;

	void Selection::operator+=(File* file) {
		SelElem* newE = new SelElem(file);
		if (first == nullptr) first = last = newE;
		else { last->next = newE; last = newE; }
	}
	void Selection::operator-=(File* file) {
		SelElem* curr = first, *prev = nullptr;
		while (curr) {
			if (*(curr->elem) == *file) break;
			prev = curr; curr = curr->next;
		}
		if (curr) {
			if (prev) prev->next = curr->next;
			else first = curr->next;
			delete curr->elem;
			delete curr;
		}
	}
	void Selection::deleteSelection() {
		SelElem* el;
		while (first) {
			el = first; el->next = nullptr;
			first = first->next;
			delete el->elem;
			delete el;
		}
		first = nullptr;
	}


	Selection* Selections::findSelection(string sl) {
		selectionsList* current = selections;
		while (current) {
			if (current->selection->fetchName() == sl) break;
			current = current->next;
		}
		return current->selection;
	}
	void Selections::removeSelection(string sl) {
		selectionsList* current = selections, *previous = nullptr;
		while (current) {
			if (current->selection->fetchName() == sl) break;
			previous = current;
			current = current->next;
		}
		if (previous)previous->next = current->next;
		else selections = current->next;
		current->selection->deleteSelection();
		delete current->selection;
		delete current;
	}
	void Selections::saveSelections(path pathh) {
		selectionsList* current = selections;
		fstream file;
		file.open(pathh.string().c_str(), std::fstream::out);
		while (current)
		{
			file << "*" << current->selection->name << "\n";
			Selection::SelElem* elem = current->selection->first;
			while (elem) {
				file << "~" << elem->elem->filePath() << "\n";
				elem = elem->next;
			}
			current = current->next;
		}
		file.close();
	}
	void Selections::loadSelections(path pathh) {
		fstream file;
		file.open(pathh.string().c_str(), std::fstream::in);
		string s, selname;
		Selection* sel = nullptr;
		getline(file, s);
		while (s!="")
		{
			s.erase(s.length() - 1, 1);
			if (s[0] == '*') {
				selname = s.erase(0, 1);
			}
			else {
				sel = SimbolicsMap->findObject(selname);
				(*sel) += new File(s.erase(0, 1));
			}
			getline(file, s);
		}
		file.close();
	}

	bool Selections::isSelection(string selection) {
		selectionsList* current = selections;
		for (; current; current = current->next) if (current->selection->fetchName() == selection) return true;
		return false;
	}

