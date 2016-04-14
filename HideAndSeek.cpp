#include "Commands.h"

Aliases* AliasesMap;
Simbolics* SimbolicsMap;

int movee = 0;

int main()
{
	AliasesMap = new Aliases();
	SimbolicsMap = new Simbolics();
	colorMe(COLOR_SILVER);
	cout << "Do you have any simbolic names and selections you would like to load?" << endl;
	string answer;
	cin >> answer;
	if (answer == "yes") {
		cout << "Where have you saved your simbolics?" << endl;
		cin >> answer;
		path pathSelections = answer;
		SimbolicsMap->loadSimbolic(pathSelections);
		cout << "Where have you saved your selections?" << endl;
		cin >> answer;
		pathSelections = answer;
		Selections::loadSelections(pathSelections);
	}
	cout << "Do you have any aliases you would like to load?" << endl;
	cin >> answer;
	if (answer == "yes") {
		cout << "Where have you saved your aliases?" << endl;
		cin >> answer;
		path pathSelections = answer;
		AliasesMap->loadAlias(pathSelections);
	}
	cin.ignore();
	//current_path("C:/");

	while (1) {
		try {
			colorMe(COLOR_LIME);
			cout << current_path() << " : ";
			colorMe(COLOR_YELLOW);
			string command;
			getline(cin, command);
			Command::parseCommand(command);
		}
		catch (NonExistingCommand* com) { com->printEx(); }
		catch (exception& ex) {
			colorMe(COLOR_FUCHSIA);
			cout<<ex.what()<<endl;
		}
	}
	
}