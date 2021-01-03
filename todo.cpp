#include <iostream>
#include <bits/stdc++.h>
using namespace std;
#define REP(i, a, b) for (int i = a; i < b; i++)
#define vs vector<string>
#define vi vector<int>
#define vvs vector<vs>
#define pushb push_back
#define umapSS unordered_map<string, string>
#include <time.h>

const string TODO_FILENAME = "todo.txt";
const string DONE_FILENAME = "done.txt";

string helperString = "Usage :-\n $ ./todo add \"todo item\"  # Add a new todo \n $ ./todo ls               # Show remaining todos\n $ ./todo del NUMBER       # Delete a todo\n $ ./todo done NUMBER      # Complete a todo\n $ ./todo help             # Show usage \n $ ./todo report           # Statistics \n";

class DateTime
{
public:
    string getDate()
    {
        auto now = chrono::system_clock::now();
        auto itt = std::chrono::system_clock::to_time_t(now);
        std::ostringstream ss;
        ss << std::put_time(gmtime(&itt), "%FT%TZ");

        string newDate = ss.str().substr(0, 10);

        return newDate;
    }
};

class FileReadWrite
{
public:
    bool deleteFile()
    {

        if (remove("todo.txt") == 0)
        {
            return false;
        }
        return true;
    }

    void readFile(string fileName, vs &contents)
    {
        ifstream file;
        file.open(fileName, ios::out);

        string line;
        while (file)
        {

            getline(file, line);
            if (line.size() != 0)
                contents.push_back(line);
            //cout << line << endl;
        }

        file.close();
    }

    bool writeFile(string fileName, string item)
    {
        ofstream file;
        file.open(fileName, ios::app);

        file << item << endl;

        file.close();

        return true;
    }

    bool writeDoneFile(string fileName, string item)
    {
        ofstream file;
        file.open(fileName, ios::app);

        DateTime *date = new DateTime();

        string curDate = date->getDate();

        file << "x " << curDate << " " << item << endl;

        file.close();

        return true;
    }

    bool deleteItem(string fileName, int number)
    {
        vs contents;
        readFile(fileName, contents);

        if (contents.empty() || contents.size() < number - 1)
        {
            return false;
        }
        contents.erase(contents.begin() + number - 1);

        deleteFile();
        for (auto ele : contents)
            writeFile(fileName, ele);
        return true;
    }

    bool doneTODO(string srcFile, string destFile, int number)
    {
        vs contents;
        readFile(srcFile, contents);
        if (contents.empty() || contents.size() < number - 1)
            return false;
        string item = contents[number - 1];
        contents.erase(contents.begin() + number - 1);
        writeDoneFile(DONE_FILENAME, item);
        deleteFile();
        for (auto ele : contents)
            writeFile(srcFile, ele);

        return true;
    }
};

int main(int argc, char *argv[])
{
    FileReadWrite *obj = new FileReadWrite;
    DateTime *date = new DateTime;

    string commandType;
    if (argc == 1)
    {
        commandType = "help";
    }
    else
    {
        commandType = argv[1];
    }

    if (commandType == "help")
    {
        cout << helperString;
    }
    else if (commandType == "ls")
    {
        vs contents;
        obj->readFile(TODO_FILENAME, contents);

        int size = contents.size();
        int i = size - 1;

        while (i >= 0)
        {

            cout << "[" << i + 1 << "]"
                 << " " << contents[i] << endl;
            i--;
        }
    }
    else if (commandType == "add")
    {
        string addingItem = argv[2];

        if (obj->writeFile(TODO_FILENAME, addingItem))
            cout << "Added todo: \"" << addingItem << "\"" << endl;
    }
    else if (commandType == "del")
    {
        int number = stoi(argv[2]);

        if (obj->deleteItem(TODO_FILENAME, number))
        {
            cout << "Deleted todo #" << number << endl;
        }
        else
        {
            cout << "Error: todo #" << number << " does not exist. Nothing deleted." << endl;
        }
    }
    else if (commandType == "done")
    {
        int number = stoi(argv[2]);
        if (obj->doneTODO(TODO_FILENAME, DONE_FILENAME, number))
            cout << "Marked todo #" << number << " as done." << endl;
        else
            cout << "Error: todo #" << number << " does not exist." << endl;
    }

    else if (commandType == "report")
    {

        vector<string> todoContents, doneContents;
        string curDate = date->getDate();
        obj->readFile(TODO_FILENAME, todoContents);
        obj->readFile(DONE_FILENAME, doneContents);

        cout << curDate << " Pending : " << todoContents.size() << " Completed : " << doneContents.size() << endl;
    }

    return 0;
}
