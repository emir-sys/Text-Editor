#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>
#include <stack>

using namespace::std;

//Line class holds content for each line and connects to next line
class Line {
public:
	string content;
	Line* next; //Making Line class a linked list
	Line(string content) {
		//Constructor
		this->content = content;
		next = NULL;
	}
};

//Editor class does all the work in program
class Editor {
public:
	//lines is a linked list of all lines 
	Line* lines = NULL;
	int currentPage;
	Editor() {
		currentPage = 0;
	}
	//Adds new line
	//By inserting new 'Line' object next to last line
	void addNewLine(string content) {
		Line* newLine = new Line(content);
		Line* temp = lines;
		if (lines == NULL) {
			lines = newLine;
			return;
		}
		while (temp->next != NULL)
		{
			temp = temp->next;
		}
		temp->next = newLine;
	}
	//This method counts all the 'Line' objects in lines linked list and returns the value
	int getLineCount() {
		int count = 0;
		Line* curLine = lines;
		while (curLine != NULL)
		{
			count++;
			curLine = curLine->next;
		}
		return count;
	}
	//This method captures the lines at n and m positions
	//Then checks if there are lines at positions
	//If yes, then the linked list objects are not moved but their content will be moved until line 'm' is found
	//If n is smaller then m, then we should move it backwards
	void moveLine(int n, int m) {
		Line* lineN = getNthLine(n);
		Line* lineM = getNthLine(m);
		Line* temp = NULL;
		if (lineN == NULL || lineM == NULL) {
			cout << "No line found at position " << n << " or " << m << endl;
			return;
		}
		else if (n == m) {
			cout << "Values should not be same." << endl;
			return;
		}
		else if (n < m) {
			//Move forward
			string tempContent = lineN->content;
			while (lineN != lineM) {
				tempContent = lineN->next->content;
				lineN->next->content = lineN->content;
				lineN->content = tempContent;
				lineN = lineN->next;
			}
		}
	}
	void readFromFile(string fileName) {
		ifstream file;
		file.open(fileName);
		string line;
		while (getline(file, line))
		{
			addNewLine(line);
		}
		file.close();
	}
	//Writes each line in linked list to given file
	void writeToFile(string fileName) {
		ofstream file;
		file.open(fileName);
		Line* curLine = lines;
		while (curLine != NULL)
		{
			file << curLine->content << endl;
			curLine = curLine->next;
		}
		file.close();
	}
	//This method runs much likely to getLineCount
	//It counts the lines then returns the Line object when lineNo equals to counter
	Line* getNthLine(int lineNo) {
		int count = 0;
		Line* curLine = lines;
		while (curLine != NULL)
		{
			if (count == lineNo)
				return curLine;
			count++;
			curLine = curLine->next;
		}
		return NULL;
	}
	//Deletes given line at position
	//If line is head of the list, then just set header to ist next
	//Otherwise find the line before 'n'th line then set line n-1's next to current line
	void deleteAt(int n) {
		Line* lineN = getNthLine(n);
		if (lineN == NULL) {
			cout << "There is no line " << n << endl;
		}
		else if (lineN == lines)
		{
			lines = lines->next;
		}
		else {
			Line* beforeN = getNthLine(n - 1);
			beforeN->next = lineN->next;
		}
	}
	//This method just finds the line at 'n' then replace its content with given one
	void replaceAtN(int n, string newContent) {
		Line* lineN = getNthLine(n);
		lineN->content = newContent;
	}
	//This method traverses through linked list and prints everything
	void PrintAll() {
		int i = 1;
		Line* curLine = lines;
		while (curLine != NULL)
		{
			cout << i << ") " << curLine->content << endl;
			curLine = curLine->next;
			i++;
		}
		cout << endl << endl;
	}
	//Prints current page
	//Printing every 10 line of a page bydividing it by '10' 
	void PrintCurrentPage() {
		int i = 0;
		Line* curLine = lines;
		while (curLine != NULL)
		{
			if (i / 10 == currentPage) {
				cout << i+1 << ") " << curLine->content << endl;
			}
			curLine = curLine->next;
			i++;
		}
		cout << endl << endl << "--- Page " << currentPage + 1 << " ---" << endl << endl;
	}
	//If there is a next page, just increase currentPAge by 1
	void nextPage() {
		int pageCount = getLineCount() / 10;
		if (currentPage < pageCount)
		{
			currentPage++;
		}
	}
	//If page is bigger then 0, just decrease currentPage by 1
	void prevPage() {
		int pageCount = getLineCount() / 10;
		if (currentPage > 0)
		{
			currentPage--;
		}
	}
	//Ths method inserts each line content into vector to use it in stack to undo changes
	vector<string> toVector() {
		vector<string> vec;
		Line* curLine = lines;
		int i = 0;
		while (curLine != NULL)
		{
			vec.push_back(curLine->content);
			curLine = curLine->next;
		}
		return vec;
	}
	//This method takes a vector parameter, empties the linked list then inserts each string at vector to LL
	void fromArray(vector<string> vec) {
		lines = NULL;
		for (int i = 0; i < vec.size(); i++) {
			addNewLine(vec.at(i));
		}
	}
	//This method inserts new line at given n position
	//If n is bigger then count of lines, then insert 'm' times new empty line
	//Otherwise move each line
	void insertAt(int n, string content) {
		Line* lineN = getNthLine(n);
		if (lineN == NULL) {
			int lineCount = getLineCount();
			for (int i = lineCount; i < n; i++)
			{
				addNewLine("");
			}
			addNewLine(content);
		}
		else {
			Line* newLine = new Line(content);
			if (n > 0) {
				Line* beforeN = getNthLine(n - 1);
				newLine->next = lineN;
				beforeN->next = newLine;
			}
			else {
				Line* temp = lines;
				newLine->next = lines;
				lines = newLine;
			}
		}
	}
};

//Undo method, if stack is not empty, get top value and call fromArray method
//then pop the top value from stack
void Undo(Editor* editor,stack<vector<string>> *stck) {
	if (stck->empty() == false) {
		editor->fromArray(stck->top());
		stck->pop();
		cout << "Undo done." << endl;
	}
}
//This method uses 'regex' to compare strings
//Then extract parameters needed from string and run command by calling indivudual method
void RunCommand(const string command, Editor* editor,stack<vector<string>> *stck) {
	smatch matchParameter;
	int numParameter, numParameter2;
	string strParameter;
	regex fileRegex = regex("[A-z.]+$");
	if (regex_match(command, regex("open [A-z.]+"))) {
		regex_search(command.begin(), command.end(), matchParameter, fileRegex);
		editor->readFromFile(matchParameter[0]);
		cout << "Data succesfully readed from " << matchParameter[0] << endl;
	}
	else if (regex_match(command, regex("save [A-z.]+"))) {
		regex_search(command.begin(), command.end(), matchParameter, fileRegex);
		editor->writeToFile(matchParameter[0]);
		cout << "Data saved to " << matchParameter[0] << endl;
		return;
	}
	else if (regex_match(command, regex("insert [0-9]+ .*"))) {
		regex_search(command.begin(), command.end(), matchParameter, regex("[0-9]+"));
		stringstream toInt(matchParameter[0]);
		toInt >> numParameter;
		regex_search(command.begin(), command.end(), matchParameter, regex("[0-9]+ .*"));
		strParameter = matchParameter[0];
		strParameter = regex_replace(strParameter, regex("([0-9]+" "?)(.*)"), "$2");
		editor->insertAt(numParameter - 1, strParameter.erase(0, 1));
		cout << "Line successfully inserted at position " << numParameter << endl;
	}
	else if (regex_match(command, regex("replace [0-9]+ .*"))) {
		regex_search(command.begin(), command.end(), matchParameter, regex("[0-9]+"));
		stringstream toInt(matchParameter[0]);
		toInt >> numParameter;
		regex_search(command.begin(), command.end(), matchParameter, regex("[0-9]+ .*"));
		strParameter = matchParameter[0];
		strParameter = regex_replace(strParameter, regex("([0-9]+" "?)(.*)"), "$2");
		editor->replaceAtN(numParameter - 1, strParameter.erase(0, 1));
		cout << "Line successfully replaced at position " << numParameter << endl;
	}
	else if (regex_match(command, regex("delete [0-9]+$"))) {
		regex_search(command.begin(), command.end(), matchParameter, regex("[0-9]+"));
		stringstream toInt(matchParameter[0]);
		toInt >> numParameter;
		if (editor->getNthLine(numParameter) == NULL) {
			cout << "This line is empty" << endl;
			return;
		}
		editor->deleteAt(numParameter-1);
	}
	else if (regex_match(command, regex("move [0-9]+ [0-9]+$"))) {
		regex_search(command.begin(), command.end(), matchParameter, regex("[0-9]+"));
		stringstream toInt(matchParameter[0]);
		toInt >> numParameter;
		regex_search(command.begin(), command.end(), matchParameter, regex("[0-9]+$"));
		stringstream toInt2(matchParameter[0]);
		toInt2 >> numParameter2;
		editor->moveLine(numParameter-1, numParameter2-1);
		cout << "Move command done." << endl;
	}
	else if (regex_match(command, regex("next"))) {
		editor->nextPage();
		editor->PrintCurrentPage();
		return;
	}
	else if (regex_match(command, regex("prev"))) {
		editor->prevPage();
		editor->PrintCurrentPage();
		return;
	}
	else if (regex_match(command, regex("undo"))) {
		Undo(editor,stck);
		editor->PrintCurrentPage();
		return;
	}
	else {
		cout << "Command not found or invalid arguments." << endl;
		return;
	}
	stck->push(editor->toVector());
	editor->PrintCurrentPage();
}
int main(int argc, char** argv) {
	stack<vector<string>> *stck = new stack<vector<string>>();
	Editor* editor = new Editor();
	string command = "";
	while (command != "quit") {
		getline(cin, command);
		RunCommand(command, editor, stck);
	}
	cout << "Exiting...";
	return 0;
}

