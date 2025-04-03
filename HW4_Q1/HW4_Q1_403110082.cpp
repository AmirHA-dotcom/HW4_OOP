#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

class HugeInt
{
private:
    string name;
    string type;
    string number;
public:
    void create_HugeInt(string name, string type, string number)
    {
        this-> name = name;
        this-> type = type;
        this-> number = number;
        cout << this-> name << " as " << this-> type << " is " << this-> number << endl;
    }

    void replace_HugeInt(string type, string number)
    {
        this-> type = type;
        this-> number = number;
    }

    string get_name()
    {
        return name;
    }

    string get_type()
    {
        return type;
    }

    string get_number()
    {
        return number;
    }

};

int main()
{
    vector<HugeInt> HugeInts;
    //regex create (R"(^create\s+(\w+)\s+as\s+(\w+)\s+(\w+)$)");
    regex create (R"(^create\s+(\w+)\s+as\s+(\w+)\s+([01]+|[0-9A-Fa-f]+|\d+)$)");
    regex plus (R"(^(\w+)\s*\+\s*(\w+)\s+in\s+(\w+)$)");
    regex multiply (R"(^(\w+)\s*\*\s*(\w+)\s+in\s+(\w+)$)");
    smatch match;
    string command;
    while (true)
    {
        getline(cin, command);

        if (command == "end")
        {
            return 0;
        }

        if (regex_match(command, match, create))
        {
            string name = match[1];
            string type = match[2];
            string number = match[3];
            HugeInts.emplace_back();
            HugeInts.back().create_HugeInt(name, type, number);
        }
    }
}