#include <iostream>
#include <string.h>
#include <zmq.hpp>
#include <sstream>
#include <algorithm>
#include "httplib.h"
struct Student
{
    Student(int id, std::string name, std::string surname, std::string dateOfBirth) : id(id), name(name), surname(surname), dateOfBirth(dateOfBirth) {}
    int id;
	std::string surname;
	std::string name;
    std::string dateOfBirth;
};
class Client
{
protected:
	std::vector<Student> students;
public:
	void printSortedStudents()
	{
		for (int i = 0; i < students.size(); i++)
		{
			std::cout << students[i].id << " " << students[i].name << " " << students[i].surname << " " << students[i].dateOfBirth << std::endl;
		}
	}
	void sort()
	{
		if (students.size() != 0)
		{
			std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
			if (a.surname == b.surname)
			{
				return a.name < b.name;
			}
 			else return a.surname < b.surname; });
		}
	}
};
class ZMQClient : public Client
{
private:
	zmq::context_t context;
	zmq::socket_t socket;
	std::istringstream iss;
public:
	ZMQClient() {
		context = zmq::context_t{ 1 };
		socket = zmq::socket_t{ context,zmq::socket_type::sub };
		socket.connect("tcp://localhost:5555");
		socket.set(zmq::sockopt::subscribe, "");
	}
	void receive()
	{
		zmq::message_t reply{};
		socket.recv(reply, zmq::recv_flags::none);
		int id,size;
		std::string surname, name, dateOfBirth;
		iss.str(reply.to_string());
		iss >> size;
		students.clear();
		for (auto request_num = 0; request_num < size; request_num++)
		{
			iss.clear();
			socket.recv(reply, zmq::recv_flags::none);
			iss.str(reply.to_string());
			iss >> id;
			iss >> name >> surname >> dateOfBirth;
			students.push_back(Student(id, name, surname, dateOfBirth));
			std::cout << "Received " << reply.to_string();
			//std::cout << "(" << request_num << ")" << std::endl;
		}
		std::cout << "\n";
		sort();
	}
};
class HTTPClient : public Client
{
private:
	std::unique_ptr<httplib::Client> cli;
public:
	HTTPClient() {
		cli = std::make_unique<httplib::Client>("localhost:1234");
	}
	void get(std::string toGet)
	{
		students.clear();
		std::istringstream iss;
		int id;
		std::string surname, name, dateOfBirth;
		auto res = cli->Get(toGet.c_str());
		if(toGet == "/students")
		{
			if (res) {
				std::cout << "Response status: " << res->status << std::endl;
				iss.clear();
				iss.str(res->body);
				while (iss >> id)
				{
					iss >> name;
					iss >> surname;
					iss >> dateOfBirth;
					students.push_back(Student(id, name, surname, dateOfBirth));
				}
				std::cout << res->body << std::endl;
			}
			sort();
		}

	}
	void getStop()
	{
		auto res = cli->Get("/stop");
	}
};

int main()
{
	ZMQClient zmqclient;
	HTTPClient httpClient;
	std::string command;
    while (command != "/stop")
	{
		std::cin >> command;
		if (command == "/zmq")
		{
			zmqclient.receive();
			std::cout << "Sorted: \n";
			zmqclient.printSortedStudents();
		}
		else if (command == "/http")
		{
			httpClient.get("/students");
			std::cout << "Sorted: \n";
			httpClient.printSortedStudents();
		}
		else if (command == "/stop")
			httpClient.getStop();
		std::cout << "\n";
	}
}