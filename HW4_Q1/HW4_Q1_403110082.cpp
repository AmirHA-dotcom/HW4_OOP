#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

string add_HugeInt (string& type, string& number1, string& number2)
{
    int base;
    if (type == "Decimal") {base = 10;}
    else if (type == "Binary") {base = 2;}
    else if (type == "Hexadecimal") {base = 16;}

    int n1 = number1.size(), n2 = number2.size();
    int carry = 0;
    vector<int> sum_result(max(n1,n2) + 1, 0);
    int i = n1 - 1, j = n2 - 1, k = sum_result.size() - 1;

    while (i >= 0 || j >= 0 || carry != 0)
    {
        int digit1 = 0, digit2 = 0;
        if (i >=0 )
        {
            digit1 = isdigit(number1[i]) ? number1[i] - '0' : toupper(number1[i]) - 'A' + 10;
            i--;
        }
        if (j >= 0)
        {
            digit2 = isdigit(number2[j]) ? number2[j] - '0' : toupper(number2[j]) - 'A' + 10;
            j--;
        }
        int sum = carry + digit1 + digit2;
        carry = sum / base;
        sum_result[k--] = sum % base;
    }
    string result;
    int start = (sum_result[0] == 0) ? 1 : 0;
    for (i = start; i < sum_result.size(); i++) {
        if (sum_result[i] < 10)
            result += sum_result[i] + '0';
        else
            result += sum_result[i] - 10 + 'A';
    }
    return result.empty() ? "0" : result;
}

string multiply_HugeInt(string& type, string& number1, string& number2)
{
    int base;
    if (type == "Decimal") {base = 10;}
    else if (type == "Binary") {base = 2;}
    else if (type == "Hexadecimal") {base = 16;}
    if (number1 == "0" || number2 == "0") return "0";

    int n1 = number1.size(), n2 = number2.size();
    int carry = 0;
    vector<int> product(n1+n2, 0);

    for (int i = n1 - 1; i >= 0; i--)
    {
        int digit1 = isdigit(number1[i]) ? number1[i] - '0' : toupper(number1[i]) - 'A' + 10;
        for (int j = n2 - 1; j >= 0; j--)
        {
            int digit2 = isdigit(number2[j]) ? number2[j] - '0' : toupper(number2[j]) - 'A' + 10;
            int sum = (digit1 * digit2) + product[i + j + 1];
            product[i + j + 1] = sum % base;
            product[i + j] += sum / base;
        }
    }

    string result;
    bool has_zero = true;
    for (int number: product)
    {
        if (has_zero && number == 0) continue;
        has_zero = false;

        if (number < 10)
            result += number + '0';
        else
            result += number - 10 + 'A';
    }
    return result.empty() ? "0" : result;
}

class HugeInt
{
private:
    string name;
    string type;
    string number;
public:
    void create_HugeInt_with_comment(string name, string type, string number)
    {
        this-> name = name;
        this-> type = type;
        this-> number = number;
        cout << this-> name << " as " << this-> type << " is " << this-> number << endl;
    }

    void create_HugeInt_without_comment(string name, string type, string number)
    {
        this-> name = name;
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

    void plus(string type, string number1, string number2)
    {
        this-> type = type;
        this-> number = add_HugeInt(type, number1, number2);

        cout << this-> name << " as " << this-> type << " is " << this-> number << endl;
    }

    void multiply(string type, string number1, string number2)
    {
        this-> type = type;
        this-> number = multiply_HugeInt(type, number1, number2);

        cout << this-> name << " as " << this-> type << " is " << this-> number << endl;
    }

};

int HugeInt_index(string& name, vector<HugeInt>& HugeInts)
{
    for (int i = 0; i < HugeInts.size(); i++)
    {
        if (name == HugeInts[i].get_name())
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    vector<HugeInt> HugeInts;
    //regex create (R"(^create\s+(\w+)\s+as\s+(\w+)\s+(\w+)$)");
    regex create (R"(^\s*create\s+(\w+)\s+as\s*(\w+)\s*([01]+|[0-9A-F]+|\d+)\s*$)");
    regex plus (R"(^\s*(\w+)\s*\+\s*(\w+)\s*in\s*(\w+)\s*$)");
    regex multiply (R"(^\s*(\w+)\s*\*\s*(\w+)\s*in\s*(\w+)\s*$)");
    smatch match;
    string command;
    while (true)
    {
        getline(cin, command);

        if (command.find("end") != string::npos)
        {
            return 0;
        }

        if (regex_match(command, match, create))
        {
            string name = match[1];
            string type = match[2];
            string number = match[3];
            HugeInts.emplace_back();
            HugeInts.back().create_HugeInt_with_comment(name, type, number);
            continue;
        }

        if (regex_match(command, match, plus))
        {
            string name1 = match[1];
            string name2 = match[2];
            string name3 = match[3];
            int name1_index = HugeInt_index(name1, HugeInts);
            int name2_index = HugeInt_index(name2, HugeInts);
            int name3_index = HugeInt_index(name3, HugeInts);

            if (name1_index == -1 || name2_index == -1){continue;}
            if (HugeInts[name1_index].get_type() != HugeInts[name2_index].get_type()){continue;}

            if (name3_index == -1)
            {
                HugeInts.emplace_back();
                HugeInts.back().create_HugeInt_without_comment(name3, "", "");
            }
            name3_index = HugeInt_index(name3, HugeInts);

            HugeInts[name3_index].plus(HugeInts[name1_index].get_type(), HugeInts[name1_index].get_number(), HugeInts[name2_index].get_number());
        }

        if (regex_match(command, match, multiply))
        {
            string name1 = match[1];
            string name2 = match[2];
            string name3 = match[3];
            int name1_index = HugeInt_index(name1, HugeInts);
            int name2_index = HugeInt_index(name2, HugeInts);
            int name3_index = HugeInt_index(name3, HugeInts);

            if (name1_index == -1 || name2_index == -1){continue;}
            if (HugeInts[name1_index].get_type() != HugeInts[name2_index].get_type()){continue;}

            if (name3_index == -1)
            {
                HugeInts.emplace_back();
                HugeInts.back().create_HugeInt_without_comment(name3, "", "");
            }
            name3_index = HugeInt_index(name3, HugeInts);

            HugeInts[name3_index].multiply(HugeInts[name1_index].get_type(), HugeInts[name1_index].get_number(), HugeInts[name2_index].get_number());
        }
    }
}