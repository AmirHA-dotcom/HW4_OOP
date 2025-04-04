#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

class Manager
{
private:
    string first_name;
    string last_name;
    string ID;
public:
    void register_manager(string& first_name, string& last_name, string& ID)
    {
        this->first_name = first_name;
        this->last_name = last_name;
        this->ID = ID;
    }
    string get_first_name() {return first_name;}
    string get_last_name() {return last_name;}
    string get_ID() {return ID;}
};

class Guest
{
private:
    string first_name;
    string last_name;
    string ID;
    string phone_number;
public:
    void register_guest(string& first_name, string& last_name, string& ID, string& phone_number)
    {
        this->first_name = first_name;
        this->last_name = last_name;
        this->ID = ID;
        this->phone_number = phone_number;
    }
    string get_first_name() {return first_name;}
    string get_last_name() {return last_name;}
    string get_ID() {return ID;}
    string get_phone_number() {return phone_number;}
};

class Service
{
private:
    string service_name;
public:
    void create_service(string& service_name)
    {
        this->service_name = service_name;
    }

    string get_service_name() {return service_name;}
};

class Controller
{
public:
    void register_manager(vector<Manager>& managers, Manager& manager, string& first_name, string& last_name, string& ID)
    {
        for (Manager& m: managers)
        {
            if (first_name == m.get_first_name() && last_name == m.get_last_name())
            {
                cout << "manager " << first_name << " " << last_name << " has been registered already" << endl;
                return;
            }
        }
        manager.register_manager(first_name, last_name, ID);
        managers.push_back(manager);
        cout << "manager " << manager.get_first_name() << " " << manager.get_last_name() << " has been registered successfully" << endl;
    }

    void register_guest(vector<Guest>& guests, Guest& guest, string& first_name, string& last_name, string& ID, string& phone_number)
    {
        for (Guest& g: guests)
        {
            if (ID == g.get_ID())
            {
                cout << "guest " << ID << " has been registered already" << endl;
                return;
            }
        }
        guest.register_guest(first_name, last_name, ID, phone_number);
        guests.push_back(guest);
        cout << "guest " << ID << " has been registered successfully" << endl;
     }

    void add_service(vector<Service>& services, Service& service, string& service_name, vector<Manager>& managers, string& manager_ID)
    {
        bool manager_ID_found = false;
        for (Manager m: managers)
        {
            if (m.get_ID() == manager_ID)
            {
                manager_ID_found = true;
                break;
            }
        }
        if (!manager_ID_found)
        {
            cout << "manager " << manager_ID << " is not registered" << endl;
            return;
        }

        bool service_found = false;
        for (Service s: services)
        {
            if (s.get_service_name() == service_name)
            {
                service_found = true;
                break;
            }
        }
        if (service_found)
        {
            cout << "service " << service_name << " has been added already" << endl;
            return;
        }
        service.create_service(service_name);
        services.push_back(service);
        cout << "service " << service_name << " has been added successfully" << endl;
    }

    void remove_service(vector<Service>& services, string& service_name, vector<Manager>& managers, string& manager_ID)
    {
        bool manager_ID_found = false;
        for (Manager m: managers)
        {
            if (m.get_ID() == manager_ID)
            {
                manager_ID_found = true;
                break;
            }
        }
        if (!manager_ID_found)
        {
            cout << "manager " << manager_ID << " is not registered" << endl;
            return;
        }

        bool service_found = false;
        int i;
        for (i = 0; i < services.size(); i++)
        {
            if (services[i].get_service_name() == service_name)
            {
                service_found = true;
                break;
            }
        }
        if (!service_found)
        {
            cout << "service " << service_name << " is not registered" << endl;
            return;
        }
        services.erase(services.begin() + i);
        cout << "service " << service_name << " has been removed successfully" << endl;
    }
};


int main ()
{
    Controller controller;
    vector<Manager> managers;
    vector<Guest> guests;
    vector<Service> services;

    regex register_manager_pattern (R"(^register manager (\w+) (\w+) with ID (\w+)$)");
    regex register_guest_pattern (R"(^register guest (\w+) (\w+) with ID (\w+) and phone number (\w+)$)");
    regex add_service_pattern ("^add service (\\w+) by manager (\\w+)$");
    regex remove_service_pattern ("^remove service (\\w+) by manager (\\w+)$");
    smatch match;

    string command;
    while (true)
    {
        getline(cin, command);
        if (command == "exit")
            return 0;

        if (regex_match(command, match, register_manager_pattern))
        {
            string first_name = match[1];
            string last_name = match[2];
            string ID = match[3];
            Manager new_manager;
            controller.register_manager(managers, new_manager, first_name, last_name, ID);
            continue;
        }

        if (regex_match(command, match, register_guest_pattern))
        {
            string first_name = match[1];
            string last_name = match[2];
            string ID = match[3];
            string phone_number = match[4];
            Guest new_guest;
            controller.register_guest(guests, new_guest, first_name, last_name, ID, phone_number);
            continue;
        }

        if (regex_match(command, match, add_service_pattern))
        {
            string service_name = match[1];
            string manager_ID = match[2];
            Service new_service;
            controller.add_service(services, new_service, service_name, managers, manager_ID);
            continue;
        }

        if (regex_match(command, match, remove_service_pattern))
        {
            string service_name = match[1];
            string manager_ID = match[2];
            controller.remove_service(services, service_name, managers, manager_ID);
            continue;
        }
    }
}