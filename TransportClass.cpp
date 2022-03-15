#include <iostream>
#include <ctime>
using namespace std;

class Transport {
protected:
	int capacity;
	int *passengers;
	int passNum;
	static int count;
	int number;
public:
	int route;
	Transport(int routeNum, int passengerNum) {
		count++;
		number = count;
		route = routeNum;
		capacity = passengerNum;
		passengers = new int[capacity] {-1};
		passNum = 0;
	}
	Transport(Transport& other) {
		count++;
		number = count;
		route = other.route;
		capacity = other.capacity;
		passengers = new int[capacity] {-1};
		passNum = 0;
	}
	virtual void operator=(Transport& other){
		route = other.route;
		capacity = other.getCapacity();
		passNum = 0;
		passengers = new int[capacity] {-1};
	};
	virtual const void printInfo() = 0;
	const int getCapacity(){
		return capacity;
	}
	friend ostream& operator<<(ostream &out, const Transport &transport);
	virtual int addPassenger(int num) {
		if (passNum >= capacity) 
			return -1;
		passengers[passNum++] = num;
		return 0;
	}
	virtual int getPassenger() {
		if (passNum <= 0)
			return -1;
		return passengers[--passNum];
	}
	virtual int operator[](int num) {
		if (num <= 0 || num > capacity) {
			cout << "Invalid count of passenger";
			return -1;
		}
		return passengers[num - 1];
	}
};

class Bus: public Transport {
public:
	int fuel;
	Bus(int routeNum = 0, int passengerNum = 15): Transport(routeNum, passengerNum) {
		fuel = rand() % 100;
	}
	void operator=(Transport& other) {
		Transport::operator=(other);
	}
	const void printInfo() {
		cout << "Bus number " << number << " route " << route << ". " << passNum << "/" << capacity << " passengers on it. Fuel tank = " << fuel;
	}
	~Bus() {
		delete passengers;
	}
};

class Trolleybus: public Transport {
public:
	int voltage;
	Trolleybus(int routeNum = 0, int passengerNum = 60): Transport(routeNum, passengerNum) {
		voltage = 220;
	}
	void operator=(Transport& other) {
		Transport::operator=(other);
	}
	const void printInfo() {
		cout << "Trolleybus number " << number << " route " << route << ". " << passNum << "/" << capacity << " passengers on it. Electricity voltage = " << voltage;
	}
	~Trolleybus() {
		delete passengers;
	}
};

ostream& operator<<(ostream &out, Transport &transport){
	transport.printInfo();
	return out;
}

int Transport::count = 0;

int main() {
	srand(time(0));
	Trolleybus *kamaz = new Trolleybus(34), *zim = kamaz;
	Bus *ikarus = new Bus(908, 90), *liaz = new Bus(119, 50);
	cout << *liaz << endl;
	*liaz = *zim;
	int num = 33;
	while (!zim->addPassenger(num++));
	while (!liaz->addPassenger(num++));
	cout << "Last passenger number: " << liaz->getPassenger() << endl << *liaz << endl;
	cout << "Last passenger number: " << zim->getPassenger() << endl << *zim << endl;
	cout << zim->voltage << " " << liaz->fuel << endl;
}