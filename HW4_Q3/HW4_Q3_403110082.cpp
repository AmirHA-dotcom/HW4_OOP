#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <cmath>

using namespace std;

int services_total_income = 0;

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
    int cost;
    int usage_count;
    bool is_active;
    void set_cost()
    {
        if (service_name == "Ghoul's Meal")
            this->cost = 30;
        else if (service_name == "Ghost Laundry")
            this->cost = 20;
        else if (service_name == "Torture Gym")
            this->cost = 10;
        else if (service_name == "Beast Massage")
            this->cost = 50;
        else if (service_name == "Room Cleaning")
            this->cost = 15;
        else if (service_name == "Moonlight Feast")
            this->cost = 40;
        else if (service_name == "Haunted Call")
            this->cost = 15;
        else if (service_name == "Vampire Dining")
            this->cost = 50;
        else if (service_name == "Mystic Encounter")
            this->cost = 40;
    }
public:
    void create_service(string& service_name)
    {
        this->service_name = service_name;
        this->special = false;
        usage_count = 0;
        set_cost();
        is_active = true;
    }
    string get_service_name() const {return service_name;}
    void make_special()
    {
        special = true;
    }
    int get_cost() const {return cost;}
    bool is_special() const { return special;}
    void use() {usage_count++;}
    int get_usage_count () const {return usage_count;}
    int get_income ()
    {
        return (usage_count * cost);
    }
    bool is_active_or_not () {return is_active;}
    void activate()  {is_active = true;}
    void deactivate() {is_active = false;}
};

class Room
{
private:
    bool Active;
    string room_ID;
    string type;
    int beds_count;
    string resident;
    bool special_services_included;
    string special_service;
    int price;
    float total_income;
    int check_in_count;
    int current_length_of_stay;
public:
    void add_room(string& room_ID, string& type, int& beds_count, int& price, bool& special_services_included, string special_service)
    {
        Active = true;
        this->room_ID = room_ID;
        this->type = type;
        this->beds_count = beds_count;
        this->resident = "";
        this->total_income = 0;
        this->check_in_count = 0;
        this->price = price;
        this->special_services_included = special_services_included;
        this->special_service = special_service;
        this->current_length_of_stay = 0;
    }
    bool is_active () {return Active;}
    void remove_room () {Active = false;}
    string get_room_ID() {return room_ID;}
    string get_type() {return type;}
    int get_beds_count() const {return beds_count;}
    void assign_guest(string resident, int length_of_stay)
    {
        this->resident = resident;
        check_in_count++;
        current_length_of_stay = length_of_stay;
    }
    void empty_room() {resident.clear(); current_length_of_stay = 0;}
    int get_length_of_stay () const {return current_length_of_stay;}
    string get_resident() {return resident;}
    int get_price() const {return price;}
    bool has_special_services() const {return special_services_included;}
    int get_total_income() const {return total_income;}
    int get_check_in_count() const {return check_in_count;}
    string get_special_service () const {return special_service;}
    void make_income (int income)
    {
        total_income += income;
    }
};

class Guest
{
private:
    string first_name;
    string last_name;
    string ID;
    string phone_number;
    int length_of_stay;
    vector<Service> services_used;
    Room room;
    bool is_checked_in;
    double total_cost;
public:
    void register_guest(string& first_name, string& last_name, string& ID, string& phone_number)
    {
        this->first_name = first_name;
        this->last_name = last_name;
        this->ID = ID;
        this->phone_number = phone_number;
        this->length_of_stay = 0;
        is_checked_in = false;
        total_cost = 0;
    }
    void set_length_of_stay(int& length_of_stay)
    {
        this->length_of_stay = length_of_stay;
    }
    string get_first_name() {return first_name;}
    string get_last_name() {return last_name;}
    void check_out () {is_checked_in = false;}
    string get_ID() {return ID;}
    string get_phone_number() {return phone_number;}
    bool  is_guest_checked_in () const {return is_checked_in;}
    int get_length_of_stay() const {return length_of_stay;}
    void assign_room  (Room room)
    {
        this->room = room;
        is_checked_in = true;
    }
    string get_room_ID() {return room.get_room_ID();}
    void use_service(Service service)
    {
        services_used.push_back(service);
    }
    vector<Service> get_used_services() {return services_used;}
    void add_total_cost(float cost)
    {
        total_cost += cost;
    }
    int get_total_cost() const
    {
        if (total_cost > 1000)
        {
            return (int)(total_cost * 1.1);
        }
        return (int)(total_cost);
    }
};

class Calculations
{
public:
    static float room_cost(Room& room, Guest& guest)
    {
        float discount = 0;
        float total_exclusive_service_cost = 0;
        if (guest.get_length_of_stay() < 4)
        {
            discount = 0;
        }
        else if (guest.get_length_of_stay() >= 4 && guest.get_length_of_stay() <= 6)
        {
            discount = 0.05;
        }
        else if (guest.get_length_of_stay() >= 7 && guest.get_length_of_stay() <= 9)
        {
            discount = 0.1;
        }
        else if (guest.get_length_of_stay() >= 10)
        {
            discount = 0.15;
        }

        int times_used_Haunted_Call = 0;
        int times_used_Vampire_Dining = 0;
        int times_used_Mystic_Encounter = 0;
        for (Service s: guest.get_used_services())
        {
            if (s.get_service_name() == "Haunted Call")
                times_used_Haunted_Call++;
            else if (s.get_service_name() == "Vampire Dining")
                times_used_Vampire_Dining++;
            else if (s.get_service_name() == "Mystic Encounter")
                times_used_Mystic_Encounter++;
        }

        if (room.has_special_services())
        {
            if (room.get_special_service() == "Haunted Call")
            {
                total_exclusive_service_cost = times_used_Haunted_Call * 15 + 1.5 * (times_used_Vampire_Dining * 50 + times_used_Mystic_Encounter * 40);
            }
            else if (room.get_special_service() == "Vampire Dining")
            {
                total_exclusive_service_cost = times_used_Vampire_Dining * 50 + 1.5 * (times_used_Haunted_Call * 15 + times_used_Mystic_Encounter * 40);
            }
            else if (room.get_special_service() == "Mystic Encounter")
            {
                total_exclusive_service_cost = times_used_Mystic_Encounter * 40 + 1.5 * (times_used_Vampire_Dining * 50 + times_used_Haunted_Call * 15);
            }
        }
        else
        {
            total_exclusive_service_cost =  1.5 * (times_used_Mystic_Encounter * 40 + times_used_Vampire_Dining * 50 + times_used_Haunted_Call * 15);
        }


        int total_cost = floor(room.get_price() * guest.get_length_of_stay() * room.get_beds_count() * (1 - discount) + total_exclusive_service_cost);
        return total_cost;
    }

    static float service_cost(Room& room, Guest& guest)
    {
        int total_services_cost = 0;
        for (Service s: guest.get_used_services())
        {
            if (s.get_service_name() == "Ghoul's Meal")
                total_services_cost += 30;
            else if (s.get_service_name() == "Ghost Laundry")
                total_services_cost += 20;
            else if (s.get_service_name() == "Torture Gym")
                total_services_cost += 10;
            else if (s.get_service_name() == "Beast Massage")
                total_services_cost += 50;
            else if (s.get_service_name() == "Room Cleaning")
                total_services_cost += 15;
            else if (s.get_service_name() == "Moonlight Feast")
                total_services_cost += 40;
        }
        return total_services_cost;
    }
};

class Controller
{
public:
    static int number_of_Coffin_Retreat_rooms;
    static bool Coffin_Retreat_found;
    static int number_of_Counts_Suite_rooms;
    static bool Counts_Suite_found;
    static int number_of_Haunted_Chamber_rooms;
    static bool Huanted_Chamber_found;
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
        int service_index = 0;
        bool flag = false;
        for (Service s: services)
        {
            service_index++;
            if (s.get_service_name() == service_name)
            {
                if (s.is_active_or_not())
                    service_found = true;
                flag = true;
                break;
            }
        }
        if (service_found)
        {
            cout << "service " << service_name << " has been added already" << endl;
            return;
        }
        if (flag)
        {
            services[service_index].activate();
        }
        else
        {
            service.create_service(service_name);
            services.push_back(service);
        }
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
            cout << "service " << service_name << " does not exist" << endl;
            return;
        }
        services[i].deactivate();
        cout << "service " << service_name << " has been removed successfully" << endl;
    }

    static void add_room(vector<Room>& rooms, Room& room, string& room_ID, string& room_type, int& beds_count, vector<Manager>& managers, string& manager_ID, vector<Service>& services)
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

        int price;
        bool special_services_included;
        string special_service;
        if (room_type == "Coffin Retreat")
        {
            price = 50;
            special_services_included = true;
            special_service = "Haunted Call";
            if (number_of_Coffin_Retreat_rooms == 0)
            {
                Service new_service;
                new_service.create_service(special_service);
                new_service.make_special();
                services.push_back(new_service);
            }
            number_of_Coffin_Retreat_rooms++;
        }
        else if (room_type == "Dungeon Royal")
        {
            price = 80;
            special_services_included = false;
            special_service = "";
        }
        else if (room_type == "Count's Suite")
        {
            price = 150;
            special_services_included = true;
            special_service = "Vampire Dining";
            if (number_of_Counts_Suite_rooms == 0)
            {
                Service new_service;
                new_service.create_service(special_service);
                new_service.make_special();
                services.push_back(new_service);
            }
            number_of_Counts_Suite_rooms++;
        }
        else if (room_type == "Haunted Chamber")
        {
            price = 90;
            special_services_included = true;
            special_service = "Mystic Encounter";
            if (number_of_Haunted_Chamber_rooms == 0)
            {
                Service new_service;
                new_service.create_service(special_service);
                new_service.make_special();
                services.push_back(new_service);
            }
            number_of_Haunted_Chamber_rooms++;
        }
        else if (room_type == "Shadow Penthouse")
        {
            price = 200;
            special_services_included = false;
            special_service = "";
        }
        room.add_room(room_ID, room_type, beds_count, price, special_services_included, special_service);
        rooms.push_back(room);
//        if (special_services_included)
//        {
//            Service new_service;
//            new_service.create_service(special_service);
//            new_service.make_special();
//            services.push_back(new_service);
//        }
        cout << "room "<< room_ID <<" with type of " << room_type << " has been added successfully" << endl;
    }

    static void remove_room(vector<Room>& rooms, string& room_ID, vector<Manager>& managers, string& manager_ID, vector<Service>& services)
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

        if (rooms[i].get_type() == "Coffin Retreat")
        {
            if (number_of_Coffin_Retreat_rooms == 1)
            {
                number_of_Coffin_Retreat_rooms--;
                string service_name = "Haunted Call";
                services.erase(remove_if(services.begin(), services.end(),[&](Service& s) {return s.get_service_name() == service_name;}),services.end());
            }
            else
                number_of_Coffin_Retreat_rooms--;
        }
        else if (rooms[i].get_type() == "Count's Suite")
        {
            if (number_of_Counts_Suite_rooms == 1)
            {
                number_of_Counts_Suite_rooms--;
                string service_name = "Vampire Dining";
                services.erase(remove_if(services.begin(), services.end(),[&](Service& s) {return s.get_service_name() == service_name;}),services.end());
            }
            else
                number_of_Counts_Suite_rooms--;
        }
        else if (rooms[i].get_type() == "Haunted Chamber")
        {
            if (number_of_Haunted_Chamber_rooms == 1)
            {
                number_of_Haunted_Chamber_rooms--;
                string service_name = "Mystic Encounter";
                services.erase(remove_if(services.begin(), services.end(),[&](Service& s) {return s.get_service_name() == service_name;}),services.end());
            }
            else
                number_of_Haunted_Chamber_rooms--;
        }
        cout << "room " << rooms[i].get_room_ID() << " with type of " << rooms[i].get_type() << " has been removed successfully" << endl;
        rooms[i].remove_room();
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
            if (rooms[room_index].get_room_ID() == room_ID && rooms[room_index].is_active())
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
        rooms[room_index].assign_guest(guests[guest_index].get_ID(), length_of_stay);
        guests[guest_index].set_length_of_stay(length_of_stay);
        guests[guest_index].assign_room(rooms[room_index]);
        rooms[room_index].make_income(Calculations::room_cost(rooms[room_index], guests[guest_index]));
        guests[guest_index].add_total_cost(Calculations::room_cost(rooms[room_index], guests[guest_index]));
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
        sort_rooms(rooms);
        int room_index = -1;
        bool no_room_available = true;
        for (room_index = 0; room_index < rooms.size(); room_index++)
        {
            if (rooms[room_index].get_resident().empty() && rooms[room_index].is_active())
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
        rooms[room_index].assign_guest(guests[guest_index].get_ID(), length_of_stay);
        guests[guest_index].set_length_of_stay(length_of_stay);
        guests[guest_index].assign_room(rooms[room_index]);
        rooms[room_index].make_income(Calculations::room_cost(rooms[room_index], guests[guest_index]));
        guests[guest_index].add_total_cost(Calculations::room_cost(rooms[room_index], guests[guest_index]));
        cout << "guest " << guests[guest_index].get_first_name() << " " << guests[guest_index].get_last_name() << " with ID " << guests[guest_index].get_ID() << " has been checked into room " << rooms[room_index].get_room_ID() << endl;
    }

    static void sort_rooms(vector<Room>& rooms)
    {
        sort(rooms.begin(), rooms.end(), [](Room& a, Room& b)
        {
            if (a.has_special_services() != b.has_special_services())
                return !a.has_special_services();

            if (a.get_price() != b.get_price())
                return a.get_price() * a.get_beds_count() < b.get_price() * b.get_beds_count();

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
        float total_room_cost = Calculations::room_cost(rooms[room_index], guests[guest_index]) + Calculations::service_cost(rooms[room_index], guests[guest_index]);
        if (total_room_cost > 1000)
        {
            total_room_cost *= 1.1;
        }
        cout << "guest with ID " << guests[guest_index].get_ID() << " has checked out with a cost of " << guests[guest_index].get_total_cost() << endl;
        guests[guest_index].check_out();
    }

    static void use_service(vector<Service>& services, string& service_name, vector<Guest>& guests, string& guest_ID, vector<Room>& rooms, int& services_total_income)
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
        if (!guests[guest_index].is_guest_checked_in())
        {
            cout << "guest with ID " << guest_ID << " has not been registered or checked in yet" << endl;
            return;
        }

        int room_index = 0;
        for (room_index = 0; room_index < rooms.size(); room_index++)
        {
            if (rooms[room_index].get_room_ID() == guests[guest_index].get_room_ID())
            {
                break;
            }
        }

        bool service_found = false;
        int service_index = 0;
        for (service_index = 0; service_index < services.size(); service_index++)
        {
            if (services[service_index].get_service_name() == service_name && services[service_index].is_active_or_not())
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

        if (services[service_index].is_special())
        {
            if (services[service_index].get_service_name() == "Haunted Call")
            {
                if (rooms[room_index].get_special_service() == "Haunted Call")
                {
                    rooms[room_index].make_income(15);
                    guests[guest_index].add_total_cost(15);
                }
                else
                {
                    rooms[room_index].make_income(15);
                    guests[guest_index].add_total_cost(15 * 1.5);
                }
            }
            else if (services[service_index].get_service_name() == "Vampire Dining")
            {
                if (rooms[room_index].get_special_service() == "Vampire Dining")
                {
                    rooms[room_index].make_income(50);
                    guests[guest_index].add_total_cost(50);
                }
                else
                {
                    rooms[room_index].make_income(50);
                    guests[guest_index].add_total_cost(50 * 1.5);
                }
            }
            else if (services[service_index].get_service_name() == "Mystic Encounter")
            {
                if (rooms[room_index].get_special_service() == "Mystic Encounter")
                {
                    rooms[room_index].make_income(40);
                    guests[guest_index].add_total_cost(40);
                }
                else
                {
                    rooms[room_index].make_income(40);
                    guests[guest_index].add_total_cost(40 * 1.5);
                }
            }
        }
        else
        {
            services_total_income += services[service_index].get_cost();
            guests[guest_index].add_total_cost(services[service_index].get_cost());
        }

        services[service_index].use();
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
        map<string, int> room_type_count;
        for (Room room: rooms)
        {
            room_type_count[room.get_type()] += room.get_check_in_count();
        }

        int max_count = 0;
        for (auto& entry : room_type_count) {
            if (entry.second > max_count) {
                max_count = entry.second;
            }
        }
        vector<string> most_popular_rooms;
        for (auto& entry : room_type_count) {
            if (entry.second == max_count) {
                most_popular_rooms.push_back(entry.first);
            }
        }
        sort(most_popular_rooms.begin(), most_popular_rooms.end());
        cout << "type: ";
        for (int i = 0; i < most_popular_rooms.size(); ++i)
        {
            cout << most_popular_rooms[i];
            if (i != most_popular_rooms.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }

    static void show_most_popular_service(vector<Service>& services)
    {
        map<string, int> service_type_count;
        for (Service service: services)
        {
            service_type_count[service.get_service_name()] += service.get_usage_count();
        }

        int max_count = 0;
        for (auto& entry : service_type_count) {
            if (entry.second > max_count) {
                max_count = entry.second;
            }
        }
        vector<string> most_popular_services;
        for (auto& entry : service_type_count) {
            if (entry.second == max_count) {
                most_popular_services.push_back(entry.first);
            }
        }
        sort(most_popular_services.begin(), most_popular_services.end());
        cout << "service ";
        for (int i = 0; i < most_popular_services.size(); ++i)
        {
            cout << most_popular_services[i];
            if (i != most_popular_services.size() - 1)
            {
                cout << ", ";
            }
        }
        cout << endl;
    }

    static void show_room_income(vector<Room>& rooms)
    {
        int rooms_total_income = 0;
        for (Room r: rooms)
        {
            rooms_total_income += r.get_total_income();
        }
        cout << "the total income from rooms is " << floor(rooms_total_income) << endl;
    }

    static void show_services_income(int& services_income)
    {
        cout << "the total income from services is " << floor(services_income) << endl;
    }

    static void show_total_income(vector<Guest>& guests)
    {
        int total_income_of_hotel = 0;
        for (const Guest& g: guests)
        {
           total_income_of_hotel += g.get_total_cost();
        }

        cout << "the total income of hotel is " << floor(total_income_of_hotel) << endl;
    }

    static void sort_things(string room_or_service, string type_of_sort, vector<Room> rooms, vector<Service> services)
    {
        if (room_or_service == "rooms")
        {
            if (type_of_sort == "use")
            {
                sort(rooms.begin(), rooms.end(), [](Room& a, Room& b)
                {
                    if (a.get_check_in_count() != b.get_check_in_count())
                        return a.get_check_in_count() > b.get_check_in_count();
                    return a.get_room_ID() < b.get_room_ID();
                });
                for (Room r: rooms)
                {
                    cout << r.get_room_ID() << ": " << r.get_check_in_count() << endl;
                }
            }

            else if (type_of_sort == "income")
            {
                sort(rooms.begin(), rooms.end(), [](Room& a, Room& b)
                {
                    if (a.get_total_income() != b.get_total_income())
                        return a.get_total_income() > b.get_total_income();
                    return a.get_room_ID() < b.get_room_ID();
                });
                for (Room r: rooms)
                {
                    cout << r.get_room_ID() << ": " << r.get_total_income() << endl;
                }
            }
            else
            {
                sort(rooms.begin(), rooms.end(), [](Room& a, Room& b)
                {
                    if (a.get_check_in_count() != b.get_check_in_count())
                        return a.get_check_in_count() > b.get_check_in_count();
                    if (a.get_total_income() != b.get_total_income())
                        return a.get_total_income() > b.get_total_income();
                    return a.get_room_ID() < b.get_room_ID();
                });
                for (Room r: rooms)
                {
                    cout << r.get_room_ID() << ": " << r.get_check_in_count() << " - " << r.get_total_income() << endl;
                }
            }
        }

        else
        {
            if (type_of_sort == "use")
            {
                sort(services.begin(), services.end(), [](const Service& a, const Service& b)
                {
                    if (a.get_usage_count() != b.get_usage_count())
                        return a.get_usage_count() > b.get_usage_count();
                    return a.get_service_name() < b.get_service_name();
                });
                for (Service s: services)
                {
                    if (!s.is_special())
                        cout << s.get_service_name() << ": " << s.get_usage_count() << endl;
                }
            }

            else if (type_of_sort == "income")
            {
                sort(services.begin(), services.end(), [](Service& a, Service& b)
                {
                    if (a.get_income() != b.get_income())
                        return a.get_income() > b.get_income();
                    return a.get_service_name() < b.get_service_name();
                });
                for (Service s: services)
                {
                    if (!s.is_special())
                        cout << s.get_service_name() << ": " << s.get_income() << endl;
                }
            }
            else
            {
                sort(services.begin(), services.end(), [](Service& a, Service& b)
                {
                    if (a.get_usage_count() != b.get_usage_count())
                        return a.get_usage_count() > b.get_usage_count();
                    if (a.get_income() != b.get_income())
                        return a.get_income() > b.get_income();
                    return a.get_service_name() < b.get_service_name();
                });
                for (Service s: services)
                {
                    if (!s.is_special())
                        cout << s.get_service_name() << ": " << s.get_usage_count() << " - " << s.get_income() << endl;
                }
            }
        }
    }
};

int main ()
{
    vector<Manager> managers;
    vector<Guest> guests;
    vector<Service> services;
    vector<Room> rooms;
    regex register_manager_pattern (R"(^register manager (\S+) (\S+) with ID (\S+)$)");
    regex register_guest_pattern (R"(^register guest (\S+) (\S+) with ID (\S+) and phone number (\S+)$)");
    regex add_service_pattern (R"(^add service (\S+)(?: (\S+))? by manager (\S+)$)");
    regex remove_service_pattern (R"(^remove service (\S+)(?: (\S+))? by manager (\S+)$)");
    regex add_room_pattern (R"(^add room (\S+) (\S+)(?: (\S+))? (\S+) by manager (\S+)$)");
    regex remove_room_pattern ("^remove room (\\S+) by manager (\\S+)$");
    regex check_in_with_reservation_pattern (R"(^check in guest (\S+) (\S+) (\S+) in room (\S+) for (\S+) nights$)");
    regex check_in_without_reservation_pattern (R"(^check in guest (\S+) (\S+) (\S+) for (\S+) nights$)");
    regex check_out_pattern (R"(^check out guest (\S+) (\S+) (\S+)$)");
    regex use_services_pattern (R"(^use service (\S+)(?: (\S+))? by guest (\S+)$)");
    regex show_guest_INFO_pattern ("^show guest information (\\S+)$");
    regex show_room_INFO_pattern ("^show room information (\\S+)$");
    regex show_popular_room ("^show the most popular room type$");
    regex show_popular_service ("^show the most frequent service$");
    regex show_total_income ("^show the total income of hotel$");
    regex show_rooms_income ("^show the total income from rooms$");
    regex show_services_income ("^show the total income from services$");
    regex sort_pattern ("^sort the (rooms|services) based on (use|income|use and income)$");
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
            Controller::add_room(rooms, new_room, room_ID, room_type, beds_count, managers, manger_ID,services);
            Controller::sort_rooms(rooms);
            continue;
        }

        if (regex_match(command, match, remove_room_pattern))
        {
            string room_ID = match[1];
            string manager_ID = match[2];
            Controller::remove_room(rooms, room_ID, managers, manager_ID, services);
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
            Controller::use_service(services, service_name, guests, guest_ID, rooms, services_total_income);
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

        if (regex_match(command, show_popular_service))
        {
            Controller::show_most_popular_service(services);
            continue;
        }

        if (regex_match(command, show_total_income))
        {
            Controller::show_total_income(guests);
            continue;
        }

        if (regex_match(command, show_rooms_income))
        {
            Controller::show_room_income(rooms);
            continue;
        }

        if (regex_match(command, show_services_income))
        {
            Controller::show_services_income(services_total_income);
            continue;
        }

        if (regex_match(command, match, sort_pattern))
        {
            string room_or_service = match[1];
            string type_of_sort = match[2];
            Controller::sort_things(room_or_service, type_of_sort, rooms, services);
            continue;
        }
    }
}

int Controller::number_of_Coffin_Retreat_rooms = 0;
int Controller::number_of_Counts_Suite_rooms = 0;
int Controller::number_of_Haunted_Chamber_rooms = 0;
bool Controller::Coffin_Retreat_found = false;
bool Controller::Counts_Suite_found = false;
bool Controller::Huanted_Chamber_found = false;