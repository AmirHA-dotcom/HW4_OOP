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

class Service
{
private:
    string service_name;
    bool special;
public:
    void create_service(string& service_name)
    {
        this->service_name = service_name;
        this->special = false;
    }

    string get_service_name() {return service_name;}
    void make_special()
    {
        special = true;
    }
    bool is_special() const { return special;}
};


class Guest
{
private:
    string first_name;
    string last_name;
    string ID;
    string phone_number;
    int length_of_stay;
    string room_ID;
    vector<Service> services_used;
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
    int get_length_of_stay() const {return length_of_stay;}
    void assign_room  (string room_ID)
    {
        this->room_ID = room_ID;
    }
    string get_room_ID() {return room_ID;}
    void check_out() {room_ID.clear();}
    void use_service(Service service)
    {
        services_used.push_back(service);
    }
    vector<Service> get_used_services() {return services_used;}
};

class Room
{
private:
    string room_ID;
    string type;
    int beds_count;
    string resident;
    bool special_services_included;
    string special_service;
    int price;
    int total_income;
    int check_in_count;
public:
    void add_room(string& room_ID, string& type, int& beds_count)
    {
        this->room_ID = room_ID;
        this->type = type;
        this->beds_count = beds_count;
        this->resident = "";
        this->total_income = 0;
        this->check_in_count = 0;
        if (type == "Coffin Retreat")
        {
            this->price = 50;
            this->special_services_included = true;
            this->special_service = "Haunted Call";
        }
        else if (type == "Dungeon Royal")
        {
            this->price = 80;
            this->special_services_included = false;
            this->special_service = "";
        }
        else if (type == "Count's Suite")
        {
            this->price = 150;
            this->special_services_included = true;
            this->special_service = "Vampire Dining";
        }
        else if (type == "Haunted Chamber")
        {
            this->price = 90;
            this->special_services_included = true;
            this->special_service = "Mystic Encounter";
        }
        else if (type == "Shadow Penthouse")
        {
            this->price = 200;
            this->special_services_included = false;
            this->special_service = "";
        }
    }
    string get_room_ID() {return room_ID;}
    string get_type() {return type;}
    int get_beds_count() const {return beds_count;}
    void assign_guest(string resident)
    {
        this->resident = resident;
        check_in_count++;
    }
    void empty_room() {resident.clear();}
    string get_resident() {return resident;}
    int get_price() const {return price;}
    bool has_special_services() const {return special_services_included;}
    int get_total_income() const {return total_income;}
    int get_check_in_count() const {return check_in_count;}
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
        cout << "room " << rooms[i].get_room_ID() << " with type of " << rooms[i].get_type() << " has been removed successfully" << endl;
        rooms.erase(rooms.begin() + i);
    }

    static void check_in_with_reservation(vector<Guest>& guests, string& first_name, string& last_name, string& guest_ID, vector<Room>& rooms, string& room_ID, int& length_of_stay)
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
        guests[guest_index].assign_room(room_ID);
        cout << "guest " << guests[guest_index].get_first_name() << " " << guests[guest_index].get_last_name() << " with ID " << guests[guest_index].get_ID() << " has been checked in successfully" << endl;
    }

    static void check_in_without_reservation(vector<Guest>& guests, string& first_name, string& last_name, string& guest_ID, int& length_of_stay, vector<Room>& rooms)
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
        int room_index = -1;
        bool no_room_available = true;
        for (room_index = 0; room_index < rooms.size(); room_index++)
        {
            if (rooms[room_index].get_resident().empty())
            {
                no_room_available = false;
                break;
            }
        }
        if (no_room_available)
        {
            cout << "there are no empty rooms" << endl;
            return;
        }
        rooms[room_index].assign_guest(guests[guest_index].get_ID());
        guests[guest_index].set_length_of_stay(length_of_stay);
        guests[guest_index].assign_room(rooms[room_index].get_room_ID());
        cout << "guest " << guests[guest_index].get_first_name() << " " << guests[guest_index].get_last_name() << " with ID " << guests[guest_index].get_ID() << " has been checked into room " << rooms[room_index].get_room_ID() << endl;
    }

    static void sort_rooms(vector<Room>& rooms)
    {
        sort(rooms.begin(), rooms.end(), [](Room& a, Room& b)
        {
            if (a.has_special_services() != b.has_special_services())
                return !a.has_special_services();

            if (a.get_price() != b.get_price())
                return a.get_price() < b.get_price();

            return a.get_room_ID() < b.get_room_ID();
        });
    }

    static void check_out(vector<Room>& rooms, vector<Guest>& guests, string& first_name, string& last_name, string& guest_ID)
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
        if (guests[guest_index].get_room_ID().empty())
        {
            cout << "person " << guests[guest_index].get_ID() << " is not a guest" << endl;
            return;
        }
        int room_index = -1;
        for (room_index = 0; room_index < rooms.size(); room_index++)
        {
            if (rooms[room_index].get_room_ID() == guests[guest_index].get_room_ID())
                break;
        }
        rooms[room_index].empty_room();
        guests[guest_index].check_out();
        cout << "guest with ID " << guests[guest_index].get_ID() << " has checked out with a cost of " << rooms[room_index].get_price() << endl;
    }

    static void use_service(vector<Service>& services, string& service_name, vector<Guest>& guests, string& guest_ID)
    {
        bool guest_found = false;
        int guest_index = 0;
        for (guest_index = 0; guest_index < guests.size(); guest_index++)
        {
            if (guests[guest_index].get_ID() == guest_ID)
            {
                guest_found = true;
                break;
            }
        }
        if (!guest_found)
        {
            cout << "guest with ID " << guest_ID << " has not been registered or checked in yet" << endl;
            return;
        }
        if (guests[guest_index].get_room_ID().empty())
        {
            cout << "guest with ID " << guest_ID << " has not been registered or checked in yet" << endl;
            return;
        }
        bool service_found = false;
        int service_index = 0;
        for (service_index = 0; service_index < services.size(); service_index++)
        {
            if (services[service_index].get_service_name() == service_name)
            {
                service_found = true;
                break;
            }
        }
        if (!service_found)
        {
            cout << "service " << service_name << " does not exist" << endl;
            return;
        }
        guests[guest_index].use_service(services[service_index]);
        cout << "guest " << guests[guest_index].get_ID() << " has used the service " << services[service_index].get_service_name() << " successfully" << endl;
    }

    static void show_guest_INFO(vector<Guest>& guests, string& guest_ID)
    {
        bool guest_found = false;
        int guest_index = 0;
        for (guest_index = 0; guest_index < guests.size(); guest_index++)
        {
            if (guests[guest_index].get_ID() == guest_ID)
            {
                guest_found = true;
                break;
            }
        }
        if (!guest_found)
        {
            cout << "guest " << guest_ID << " has not been registered yet" << endl;
            return;
        }
        Guest guest = guests[guest_index];
        cout << "name: " << guest.get_first_name() << " " << guest.get_last_name() << endl;
        cout << "phone number: " << guest.get_phone_number() << endl;
        cout << "ID number: " << guest.get_ID() << endl;
        cout << "length of stay: " << guest.get_length_of_stay() << endl;
        cout << "used services: " << guest.get_used_services().size() << endl;
    }

    static void show_room_INFO(vector<Room>& rooms, string& room_ID)
    {
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
        Room room = rooms[room_index];
        cout << "ID number: " << room.get_room_ID() << endl;
        cout << "type: " << room.get_type() << endl;
        cout << "number of beds: " << room.get_beds_count() << endl;
        cout << "total income: " << room.get_total_income() << endl;
        if (room.get_resident().empty())
            cout << "guest: " << "empty" << endl;
        else
            cout << "guest: " << room.get_resident() << endl;
    }

    static void show_most_popular_room(vector<Room>& rooms)
    {

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
    regex add_service_pattern (R"(^add service (\S+) (\w+) by manager (\w+)$)");
    regex remove_service_pattern (R"(^remove service (\S+) (\w+) by manager (\w+)$)");
    regex add_room_pattern (R"(^add room (\w+) (\S+) (\w+) (\d+) by manager (\w+)$)");
    regex remove_room_pattern ("^remove room (\\w+) by manager (\\w+)$");
    regex check_in_with_reservation_pattern (R"(^check in guest (\w+) (\w+) (\w+) in room (\w+) for (\d+) nights$)");
    regex check_in_without_reservation_pattern (R"(^check in guest (\w+) (\w+) (\w+) for (\d+) nights$)");
    regex check_out_pattern (R"(^check out guest (\w+) (\w+) (\w+)$)");
    regex use_services_pattern (R"(^use service (\S+) (\w+) by guest (\w+)$)");
    regex show_guest_INFO_pattern ("^show guest information (\\w+)$");
    regex show_room_INFO_pattern ("^show room information (\\w+)$");
    regex show_popular_room ("^show the most popular room type$");
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
            service_name += " ";
            service_name += match[2];
            string manager_ID = match[3];
            Service new_service;
            Controller::add_service(services, new_service, service_name, managers, manager_ID);
            continue;
        }

        if (regex_match(command, match, remove_service_pattern))
        {
            string service_name = match[1];
            service_name += " ";
            service_name += match[2];
            string manager_ID = match[3];
            Controller::remove_service(services, service_name, managers, manager_ID);
            continue;
        }

        if (regex_match(command, match, add_room_pattern))
        {
            string room_ID = match[1];
            string room_type = match[2];
            room_type += " ";
            room_type += match[3];
            int beds_count = stoi(match[4]);
            string manger_ID = match[5];
            Room new_room;
            Controller::add_room(rooms, new_room, room_ID, room_type, beds_count, managers, manger_ID);
            Controller::sort_rooms(rooms);
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

        if (regex_match(command, match, check_in_without_reservation_pattern))
        {
            string first_name = match[1];
            string last_name = match[2];
            string guest_ID = match[3];
            int length_of_stay = stoi(match[4]);
            Controller::check_in_without_reservation(guests, first_name, last_name, guest_ID, length_of_stay, rooms);
            continue;
        }

        if (regex_match(command, match, check_out_pattern))
        {
            string first_name = match[1];
            string last_name = match[2];
            string guest_ID = match[3];
            Controller::check_out(rooms, guests, first_name, last_name, guest_ID);
            continue;
        }

        if (regex_match(command, match, use_services_pattern))
        {
            string service_name = match[1];
            service_name += " ";
            service_name += match[2];
            string guest_ID = match[3];
            Controller::use_service(services, service_name, guests, guest_ID);
            continue;
        }

        if (regex_match(command, match, show_guest_INFO_pattern))
        {
            string guest_ID = match[1];
            Controller::show_guest_INFO(guests, guest_ID);
            continue;
        }

        if (regex_match(command, match, show_room_INFO_pattern))
        {
            string room_ID = match[1];
            Controller::show_room_INFO(rooms, room_ID);
            continue;
        }

        if (regex_match(command, show_popular_room))
        {
            Controller::show_most_popular_room(rooms);
            continue;
        }
    }
}