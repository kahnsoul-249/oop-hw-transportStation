//Tên : Huỳnh Minh Tài
//MSSV : 22110068
//Homework week4
//Transport Station
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ==============================
// Class Passenger (Hành khách)
// ==============================
class Passenger {
private:
    string name;    // Tên hành khách
    int id;         // ID hành khách
    vector<string> bookedTickets; // Danh sách vé đã đặt
public:
    Passenger(string n, int i) : name(n), id(i) {}

    // Đặt vé cho 1 tuyến
    void bookTicket(const string& route) {
        bookedTickets.push_back(route);
        cout << "Passenger " << name << " booked ticket for route: " << route << endl;
    }

    // Hủy vé
    void cancelTicket(const string& route) {
        for (auto it = bookedTickets.begin(); it != bookedTickets.end(); ++it) {
            if (*it == route) {
                bookedTickets.erase(it);
                cout << "Passenger " << name << " canceled ticket for route: " << route << endl;
                return;
            }
        }
        cout << "No ticket found for route: " << route << " to cancel." << endl;
    }

    // In thông tin hành khách
    void displayInfo() const {
        cout << "Passenger ID: " << id << ", Name: " << name << endl;
        cout << "Booked tickets: ";
        if (bookedTickets.empty()) {
            cout << "None";
        } else {
            for (const auto& t : bookedTickets) cout << t << " ";
        }
        cout << endl;
    }
};

// ==============================
// Class Vehicle (Phương tiện - cơ sở)
// ==============================
class Vehicle {
protected:
    string route;     // Tuyến đường
    int capacity;     // Sức chứa
    int booked;       // Số chỗ đã đặt
    string status;    // Trạng thái (On-time, Delayed)
public:
    Vehicle(string r, int c, string s = "On-time")
        : route(r), capacity(c), booked(0), status(s) {}

    virtual ~Vehicle() {}

    // Đặt chỗ cho hành khách
    bool bookPassenger(Passenger& p) {
        if (booked < capacity) {
            booked++;
            p.bookTicket(route);
            cout << "Booking successful! Seats booked: " << booked << "/" << capacity << endl;
            return true;
        } else {
            cout << "Booking failed. Vehicle is full!" << endl;
            return false;
        }
    }

    // Hàm tính thời gian di chuyển (ảo để override)
    virtual double calculateTravelTime(double distance) {
        double speed = 50; // Giả sử tốc độ trung bình 50 km/h
        return distance / speed;
    }

    // In thông tin phương tiện
    virtual void displayInfo() const {
        cout << "Vehicle Route: " << route
             << ", Capacity: " << capacity
             << ", Booked: " << booked
             << ", Status: " << status << endl;
    }
};

// ==============================
// Class ExpressBus (Xe buýt nhanh - kế thừa Vehicle)
// ==============================
class ExpressBus : 
public Vehicle {
private:
    double speed; // Tốc độ cao hơn
public:
    ExpressBus(string r, int c, double s, string st = "On-time")
        : Vehicle(r, c, st), speed(s) {}

    // Override phương thức tính thời gian (xe nhanh tiết kiệm 20%)
    double calculateTravelTime(double distance) 
    override {
        double baseTime = distance / speed;
        return baseTime * 0.8; // Nhanh hơn 20%
    }

    void displayInfo() const override {
        cout << "Express Bus Route: " << route
             << ", Capacity: " << capacity
             << ", Booked: " << booked
             << ", Status: " << status
             << ", Speed: " << speed << " km/h" << endl;
    }
};

// ==============================
// Class Schedule (Lịch trình đến/đi)
// ==============================
class Schedule {
private:
    string time;     // Thời gian
    Vehicle* vehicle; // Con trỏ đến phương tiện
    bool isArrival;  // true = Arrival, false = Departure
public:
    Schedule(const string& t, Vehicle* v, bool arrival)
        : time(t), vehicle(v), isArrival(arrival) {}

    void display() const {
        cout << (isArrival ? "Arrival" : "Departure")
             << " at " << time << " -> ";
        vehicle->displayInfo();
    }
};

// ==============================
// Class Station (Trạm xe)
// ==============================
class Station {
private:
    string name;       // Tên trạm
    string location;   // Vị trí
    string type;       // Loại (Bus/Train)
    vector<Schedule> schedules; // Danh sách lịch trình
    const int maxSchedules = 10; // Giới hạn lịch trong ngày
public:
    Station(string n, string l, string t) : name(n), location(l), type(t) {}

    // Thêm lịch trình
    void addSchedule(const Schedule& s) {
        if ((int)schedules.size() < maxSchedules) {
            schedules.push_back(s);
            cout << "Schedule added at station " << name << endl;
        } else {
            cout << "Cannot add more schedules. Limit reached!" << endl;
        }
    }

    // Xóa lịch trình cuối cùng
    void removeLastSchedule() {
        if (!schedules.empty()) {
            schedules.pop_back();
            cout << "Last schedule removed from station " << name << endl;
        } else {
            cout << "No schedules to remove." << endl;
        }
    }

    // In thông tin trạm và lịch trình
    void displayInfo() const {
        cout << "Station: " << name << " (" << type << "), Location: " << location << endl;
        cout << "Schedules:" << endl;
        if (schedules.empty()) {
            cout << "No schedules available." << endl;
        } else {
            for (const auto& s : schedules) s.display();
        }
    }
};

// ==============================
// Hàm main - test hệ thống
// ==============================
int main() {
    // Tạo một số đối tượng
    Passenger p1("TaiHuynh", 101);
    Passenger p2("Xuan Phung", 102);

    Vehicle bus("Bus Route 1", 2); // Xe bus thường
    ExpressBus express("Express Route A", 2, 100); // Xe bus nhanh

    Station st1("Central Station", "Downtown", "Bus");

    // Thêm lịch trình cho trạm
    st1.addSchedule(Schedule("12:00", &bus, true));
    st1.addSchedule(Schedule("17:00", &express, false));

    // Hiển thị thông tin trạm
    st1.displayInfo();

    cout << "==============================" << endl;

    // Đặt vé cho hành khách
    bus.bookPassenger(p1);
    bus.bookPassenger(p2);
    bus.bookPassenger(p1); // Quá tải

    cout << "==============================" << endl;

    // Hiển thị thông tin phương tiện
    bus.displayInfo();
    express.displayInfo();

    // Tính thời gian di chuyển
    double distance = 100; 
    cout << "Normal bus travel time for " << distance << " km: "
         << bus.calculateTravelTime(distance) << " hours" << endl;
    cout << "Express bus travel time for " << distance << " km: "
         << express.calculateTravelTime(distance) << " hours" << endl;

    cout << "==============================" << endl;

    // Hiển thị thông tin hành khách
    p1.displayInfo();
    p2.displayInfo();

    // Hủy vé
    p1.cancelTicket("Bus Route 1");
    p1.displayInfo();

    return 0;
}
