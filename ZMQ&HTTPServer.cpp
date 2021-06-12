#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <zmq.hpp>
#include "httplib.h"
std::string firstFileName = "student_file_1.txt";
std::string secondFileName = "student_file_2.txt";
std::string studentsOutput = "studentsOutput.txt";
struct Student
{
    Student(int id, std::string name, std::string surname, std::string dateOfBirth) : id(id),name(name),surname(surname),dateOfBirth(dateOfBirth) {}
    int id;
    std::string surname;
    std::string name;
    std::string dateOfBirth;
    std::string toString()
    {
        return std::to_string(id) + " " + name + " " + surname + " " + dateOfBirth + "\n";
    }
};
std::vector<Student> students1, students2;

void addNonMatchingRecords(std::vector<Student> &people1, std::vector<Student>&people2)
{
    for (int i = 0; i < people1.size(); i++)
    {
        for (int j = 0; j < people2.size(); j++)
        {
            if (people1[i].surname == people2[j].surname && people1[i].name == people2[j].name && people1[i].dateOfBirth == people2[j].dateOfBirth)
            {
                people2.erase(people2.begin() + j);
            }
        }
    }
    for (int i = 0; i < people2.size(); i++)
    {
        people1.push_back(people2[i]);
    }
	std::cout << "\nFormatted list: \n";
	for (int i = 0; i < people1.size(); i++)
	{
		std::cout << people1[i].id << " " << people1[i].name << " " << people1[i].surname << " " << people1[i].dateOfBirth << std::endl;
	}
}
void fillStudentsInfo()
{
	int id;
	std::string surname, name, date;
	std::ifstream firstStream(firstFileName);
	std::ifstream secondStream(secondFileName);
	std::cout << "First file:\n";
	while (firstStream >> id >> name >> surname >> date)
	{
		std::cout << id << " " << name << " " << surname << " " << date << std::endl;
		students1.push_back(Student(id, name, surname, date));
	}
	std::cout << "\nSecond file:\n";
	while (secondStream >> id >> name >> surname >> date)
	{
		students2.push_back(Student(id, name, surname, date));
		std::cout << id << " " << name << " " << surname << " " << date << std::endl;
	}
}
class ZMQServer
{
private:
    zmq::context_t context;
    zmq::socket_t socket;
public:
    ZMQServer() {
        context = zmq::context_t{ 1 };
        socket = zmq::socket_t{ context,zmq::socket_type::pub };
		socket.bind("tcp://*:5555");
    }
    void send()
    {
        while(true)
		{
			socket.send(zmq::buffer(std::to_string(students1.size())), zmq::send_flags::none);
			for (int i = 0; i < students1.size(); i++)
			{
				zmq::message_t request;
				socket.send(zmq::buffer(students1[i].toString()), zmq::send_flags::none);
			}
            std::this_thread::sleep_for(std::chrono::seconds(1));
		}
    }
};
using namespace httplib;
class HTTPServer
{

public:
	void listen()
	{
		svr.Get("/students", [&](const Request& req, Response& res) {
			res.set_chunked_content_provider(
				"text/plain",
				[](size_t offset, DataSink& sink) {
					for (int i = 0; i < students1.size(); i++)
					{
						sink.write(students1[i].toString().c_str(), students1[i].toString().size());
					}
					sink.done();
					return false;
				}
			);
			});
		svr.Get("/stop", [&](const Request& req, Response& res) {
			svr.stop();
			});
		svr.listen("localhost", 1234);
	}
private:
	httplib::Server svr;
};

int main()
{
    ZMQServer server;
    HTTPServer httpServer;
    fillStudentsInfo();
	std::ofstream ofs(studentsOutput);
	for (int i = 0; i < students1.size();i++)
	{
		ofs << students1[i].id << " " << students1[i].name << " " << students1[i].surname << " " << students1[i].dateOfBirth << "\n";
	}
	ofs.close();
    addNonMatchingRecords(students1, students2);
    std::thread zmqTh(&ZMQServer::send,&server);
    std::thread httpTh(&HTTPServer::listen, &httpServer);
    zmqTh.detach();
    httpTh.join();
    //std::cin.get();
}
