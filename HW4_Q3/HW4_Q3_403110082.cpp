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
    int length_of_stay;
public:
    void register_guest(string& first_name, string& last_name, string& ID, string& phone_number)
    {
        this->first_name = first_name;
        this->last_name = last_name;
        this->ID = ID;
        this->phone_number = phone_number;
        this->length_of_stay = 0;
    }
    void set_length_of_stay(int& length_of_stay)
    {
        this->length_of_stay = length_of_stay;
    }
    string get_first_name() {return first_name;}
    string get_last_name() {return last_name;}
    string get_ID() {return ID;}
    string get_phone_number() {return phone_number;}
    int get_length_of_stay() {return length_of_stay;}
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

class Room
{
private:
    string room_ID;
    string type;
    int beds_count;
    string resident;
    bool special_services_included;
    int price;
public:
    void add_room(string& room_ID, string& type, int& beds_count)
    {
        this->room_ID = room_ID;
        this->type = type;
        this->beds_count = beds_count;
        this->resident = "";
        this->special_services_included = false;
        this->price = 0;
    }
    string get_room_ID() {return room_ID;}
    string get_type() {return type;}
    int get_beds_count() {return beds_count;}
    void assign_guest(string resident)
    {
        this->resident = resident;
    }
    void empty_room() {resident.clear();}
    string get_resident() {return resident;}
    void add_special_services()
    {
        special_services_included = true;
    }
};

class Controller
{
public:
    static void register_manager(vector<Manager>& managers, Manager& manager, string& first_name, string& last_name, string& ID)
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

    static void register_guest(vector<Guest>& guests, Guest& guest, string& first_name, string& last_name, string& ID, string& phone_number)
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

    static void add_service(vector<Service>& services, Service& service, string& service_name, vector<Manager>& managers, string& manager_ID)
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

    static void remove_service(vector<Service>& services, string& service_name, vector<Manager>& managers, string& manager_ID)
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

    static void add_room(vector<Room>& rooms, Room& room, string& room_ID, string& room_type, int& beds_count, vector<Manager>& managers, string& manager_ID)
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

        bool room_ID_found = false;
        for (Room r: rooms)
        {
            if (r.get_room_ID() == room_ID)
            {
                room_ID_found = true;
                break;
            }
        }
        if (room_ID_found)
        {
            cout << "room " << room_ID << " has been added already" << endl;
            return;
        }
        room.add_room(room_ID, room_type, beds_count);
        rooms.push_back(room);
        cout << "room "<< room_ID <<" with type of " << room_type << " has been added successfully" << endl;
    }

    static void remove_room(vector<Room>& rooms, string& room_ID, vector<Manager>& managers, string& manager_ID)
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

        bool room_ID_found = false;
        int i = 0;
        for (i = 0; i < rooms.size(); i++)
        {
            if (rooms[i].get_room_ID() == room_ID)
            {
                room_ID_found = true;
                break;
            }
        }
        if (!room_ID_found)
        {
            cout << "room " << room_ID << " does not exist" << endl;
            return;
        }

        if (!rooms[i].get_resident().empty())
        {
            cout << "room " << rooms[i].get_room_ID() << " is not empty" << endl;
            return;
        }
        rooms.erase(rooms.begin() + i);
        cout << "room " << rooms[i].get_room_ID() << " with type of" << rooms[i].get_type() << "has been removed successfully" << endl;
    }

    static void check_in_with_reservation(vector<Guest>& guests, string& first_name, string& last_name, string& guest_ID, vector<Room>& rooms, string& room_ID, int length_of_stay)
    {
        bool guest_found = false;
        int guest_index = 0;
        for (guest_index = 0; guest_index < guests.size(); guest_index++)
        {
            if (guests[guest_index].get_first_name() == first_name && guests[guest_index].get_last_name() == last_name && guests[guest_index].get_ID() == guest_ID)
            {
                guest_found = true;
                break;
            }
        }
        if (!guest_found)
        {
            cout << "guest " << first_name << " " << last_name << " with ID " << guest_ID << " has not been registered yet" << endl;
            return;
        }
        bool room_found = false;
        int room_index = 0;
        for (room_index = 0; room_index < rooms.size(); room_index++)
        {
            if (rooms[room_index].get_room_ID() == room_ID)
            {
                room_found = true;
                break;
            }
        }
        if (!room_found)
        {
            cout << "room " << room_ID << " does not exist" << endl;
            return;
        }
        if (!rooms[room_index].get_resident().empty())
        {
            cout << "room " << rooms[room_index].get_room_ID() << " is not empty" << endl;
            return;
        }
        rooms[room_index].assign_guest(guests[guest_index].get_ID());
        guests[guest_index].set_length_of_stay(length_of_stay);
        cout << "guest " << guests[guest_index].get_first_name() << "" "" << guests[guest_index].get_last_name() << " with ID " << guests[guest_index].get_ID() << " has been checked in successfully" << endl;
    }

    static void check_in_without_reservation(vector<Guest>& guests, string& first_name, string& last_name, string& guest_ID, int& length_of_stay)
    {
        bool guest_found = false;
        int guest_index = 0;
        for (guest_index = 0; guest_index < guests.size(); guest_index++)
        {
            if (guests[guest_index].get_first_name() == first_name && guests[guest_index].get_last_name() == last_name && guests[guest_index].get_ID() == guest_ID)
            {
                guest_found = true;
                break;
            }
        }
        if (!guest_found)
        {
            cout << "guest " << first_name << " " << last_name << " with ID " << guest_ID << " has not been registered yet" << endl;
            return;
        }
        
    }

    static void sort_lexicographically(vector<Room>& rooms)
    {
        sort (rooms.begin(), rooms.end(), [](Room &a, Room &b) {return a.get_room_ID() < b.get_room_ID();});
    }
};

int main ()
{
    vector<Manager> managers;
    vector<Guest> guests;
    vector<Service> services;
    vector<Room> rooms;
    regex register_manager_pattern (R"(^register manager (\w+) (\w+) with ID (\w+)$)");
    regex register_guest_pattern (R"(^register guest (\w+) (\w+) with ID (\w+) and phone number (\w+)$)");
    regex add_service_pattern ("^add service (\\w+) by manager (\\w+)$");
    regex remove_service_pattern ("^remove service (\\w+) by manager (\\w+)$");
    regex add_room_pattern (R"(^add room (\w+) (\w+) (\d+) by manager (\w+)$)");
    regex remove_room_pattern ("^remove room (\\w+) by manager (\\w+)$");
    regex check_in_with_reservation_pattern (R"(^check in guest (\w+) (\w+) (\w+) in room (\w+) for (\d+) nights$)");
    regex check_in_without_reservation_pattern (R"(^check in guest (\w+) (\w+) (\w+) for (\d+) nights$)");
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
            Controller::register_manager(managers, new_manager, first_name, last_name, ID);
            continue;
        }

        if (regex_match(command, match, register_guest_pattern))
        {
            string first_name = match[1];
            string last_name = match[2];
            string ID = match[3];
            string phone_number = match[4];
            Guest new_guest;
            Controller::register_guest(guests, new_guest, first_name, last_name, ID, phone_number);
            continue;
        }

        if (regex_match(command, match, add_service_pattern))
        {
            string service_name = match[1];
            string manager_ID = match[2];
            Service new_service;
            Controller::add_service(services, new_service, service_name, managers, manager_ID);
            continue;
        }

        if (regex_match(command, match, remove_service_pattern))
        {
            string service_name = match[1];
            string manager_ID = match[2];
            Controller::remove_service(services, service_name, managers, manager_ID);
            continue;
        }

        if (regex_match(command, match, add_room_pattern))
        {
            string room_ID = match[1];
            string room_type = match[2];
            int beds_count = stoi(match[3]);
            string manger_ID = match[4];
            Room new_room;
            Controller::add_room(rooms, new_room, room_ID, room_type, beds_count, managers, manger_ID);
            Controller::sort_lexicographically(rooms);
            continue;
        }

        if (regex_match(command, match, remove_room_pattern))
        {
            string room_ID = match[1];
            string manager_ID = match[2];
            Controller::remove_room(rooms, room_ID, managers, manager_ID);
            continue;
        }

        if (regex_match(command, match, check_in_with_reservation_pattern))
        {
            string first_name = match[1];
            string last_name = match[2];
            string guest_ID = match[3];
            string room_ID = match[4];
            int length_of_stay = stoi(match[5]);
            Controller::check_in_with_reservation(guests, first_name, last_name, guest_ID, rooms, room_ID, length_of_stay);
            continue;
        }
    }
}