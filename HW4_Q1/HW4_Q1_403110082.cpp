#include <iostream>
#include <string>
#include <regex>
#include <vector>

using namespace std;

string add_HugeInt (string type, string number1, string number2)
{
    string result;
    int left_over = 0;
    int i = number1.size() - 1;
    int j = number2.size() - 1;
    if (type == "Decimal")
    {
        while (i >= 0 || j >= 0 || left_over != 0)
        {
            int digit1 = (i >= 0) ? number1[i] - '0' : 0;
            int digit2 = (j >= 0) ? number2[j] - '0' : 0;
            int sum = left_over + digit1 + digit2;
            left_over = sum / 10;
            result += (sum % 10) + '0';
            i--;
            j--;
        }
    }

    if (type == "Binary")
    {
        while (i >= 0 || j >= 0 || left_over != 0)
        {
            int digit1 = (i >= 0) ? number1[i] - '0' : 0;
            int digit2 = (j >= 0) ? number2[j] - '0' : 0;
            int sum = left_over + digit1 + digit2;
            left_over = sum / 2;
            result += (sum % 2) + '0';
            i--;
            j--;
        }
    }

    if (type == "Hexadecimal")
    {
        while (i >= 0 || j >= 0 || left_over != 0)
        {
            int digit1 = (i >= 0) ? (isdigit(number1[i]) ? number1[i] - '0' : toupper(number1[i]) - 'A' + 10) : 0;
            int digit2 = (j >= 0) ? (isdigit(number2[j]) ? number2[j] - '0' : toupper(number2[j]) - 'A' + 10) : 0;
            int sum = digit1 + digit2 + left_over;
            left_over = sum / 16;
            int remainder = sum % 16;
            result += (remainder < 10) ? remainder + '0' : remainder - 10 + 'A';
            i--;
            j--;
        }
    }
    i = 0; j = result.size() - 1;
    while (i < j)
    {
        swap (result[i++], result[j--]);
    }
    return result;
}

string multiply_HugeInt(string type, string number1, string number2)
{
    string result = "0";
    int n1 = number1.size(), n2 = number2.size();

    if (type == "Decimal")
    {
        for (int i = n1 - 1; i >= 0; i--)
        {
            string temp_result = "";
            int left_over = 0;
            int digit1 = number1[i] - '0';

            for (int j = n2 - 1; j >= 0; j--)
            {
                int digit2 = number2[j] - '0';
                int product = left_over + digit1 * digit2;
                left_over = product / 10;
                temp_result += (product % 10) + '0';
            }

            if (left_over > 0) { temp_result += left_over + '0'; }

            reverse(temp_result.begin(), temp_result.end());
            temp_result.append(n1 - 1 - i, '0');
            result = add_HugeInt("Decimal", result, temp_result);
        }
    }

    if (type == "Binary")
    {
        for (int i = n1 - 1; i >= 0; i--)
        {
            string temp_result = "";
            int left_over = 0;
            int digit1 = number1[i] - '0';

            for (int j = n2 - 1; j >= 0; j--)
            {
                int digit2 = number2[j] - '0';
                int product = left_over + digit1 * digit2;
                left_over = product / 2;
                temp_result += (product % 2) + '0';
            }

            if (left_over > 0) { temp_result += left_over + '0'; }

            reverse(temp_result.begin(), temp_result.end());
            temp_result.append(n1 - 1 - i, '0');
            result = add_HugeInt("Binary", result, temp_result);
        }
    }

    if (type == "Hexadecimal")
    {
        for (int i = n1 - 1; i >= 0; i--)
        {
            string temp_result = "";
            int left_over = 0;
            int digit1 = isdigit(number1[i]) ? number1[i] - '0' : toupper(number1[i]) - 'A' + 10;

            for (int j = n2 - 1; j >= 0; j--)
            {
                int digit2 = isdigit(number2[j]) ? number2[j] - '0' : toupper(number2[j]) - 'A' + 10;
                int product = left_over + digit1 * digit2;
                left_over = product / 16;
                int remainder = product % 16;
                temp_result += (remainder < 10) ? remainder + '0' : remainder - 10 + 'A';
            }

            if (left_over > 0) { temp_result += (left_over < 10) ? left_over + '0' : left_over - 10 + 'A'; }

            reverse(temp_result.begin(), temp_result.end());
            temp_result.append(n1 - 1 - i, '0');
            result = add_HugeInt("Hexadecimal", result, temp_result);
        }
    }
    return result;
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
                HugeInts.back().create_HugeInt_without_comment(name3, "template", "template");
            }
            name3_index = HugeInt_index(name3, HugeInts);

            HugeInts[name3_index].plus(HugeInts[name1_index].get_type(), HugeInts[name2_index].get_number(), HugeInts[name1_index].get_number());
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
                HugeInts.back().create_HugeInt_without_comment(name3, "template", "template");
            }
            name3_index = HugeInt_index(name3, HugeInts);

            HugeInts[name3_index].multiply(HugeInts[name1_index].get_type(), HugeInts[name2_index].get_number(), HugeInts[name1_index].get_number());
        }
    }
}