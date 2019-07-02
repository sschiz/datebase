#include <string>
#include <iostream>
#include <sstream>
#include <exception>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>

class Date {
 public:
  explicit Date(const std::string& date) {
      std::istringstream buffer(date);
      int _year, _month, _day;
      buffer >> _year;
      if (buffer.peek() != '-') {
          throw std::runtime_error("Wrong date format: "+date);
      }
      buffer.ignore(1);
      buffer >> _month;
      if (buffer.peek() != '-') {
          throw std::runtime_error("Wrong date format: "+date);
      }
      if ((_month < 1) || (_month > 12)) {
          throw std::runtime_error("Month value is invalid: "+std::to_string(_month));
      }
      buffer.ignore(1);
      buffer >> _day;
      if ((_day < 1) || (_day > 31)) {
          throw std::runtime_error("Day value is invalid: "+std::to_string(_day));
      }
      year = _year;
      month = _month;
      day = _day;
      
  }
  int GetYear() const {
      return year;
  }
  int GetMonth() const {
      return month;
  }
  int GetDay() const {
      return day;
  }
  std::string GetDate() const {
      std::ostringstream buffer;
      buffer << std::setfill('0') << std::setw(4) << year << '-';
      buffer << std::setfill('0') << std::setw(2) << month << '-';
      buffer << std::setfill('0') << std::setw(2) << day;
      return buffer.str();
  }
 private:
  int year = 0;
  int month = 1;
  int day = 1;
};

bool operator<(const Date& lhs, const Date& rhs) {
    return (lhs.GetYear() < rhs.GetYear()) && (lhs.GetMonth() < rhs.GetMonth()) && (lhs.GetDay() < rhs.GetDay());
}

class Database {
 public:
  void AddEvent(const Date& date, const std::string& event) {
      if (db.count(date) > 0) {
          if (std::find(db[date].begin(), db[date].end(), event) == db[date].end()) {
              db[date].push_back(event);
          }
      } else {
          db[date].push_back(event);
      }
  }
  bool DeleteEvent(const Date& date, const std::string& event) {
      if (db.count(date) > 0) {
          std::vector<std::string>::iterator element = std::find(db[date].begin(), db[date].end(), event);
          if (element != db[date].end()) {
              db[date].erase(element);
              return true;
          } else {
              return false;
          }
      }
      return false;
  }
  int  DeleteDate(const Date& date) {
      if (db.count(date) > 0) {
          int size = db[date].size();
          db.erase(date);
          return size;
      }
      return 0;
  }

  void Find(const Date& date) const {
      std::vector<std::string> el = db.at(date);
      std::sort(el.begin(), el.end());
      for (const std::string& i : el) {
          std::cout << i << std::endl;
      }
  }
  
  void Print() const {
      for (const auto& date : db) {
          for(const std::string& element : date.second) {
              std::cout << date.first.GetDate() << " " << element << std::endl;
          }
      }
  }
 private:
  std::map<Date, std::vector<std::string>> db;
};

int main() {
  Database db;
    
  std::string str;
  std::string command, date, event;
  while (getline(std::cin, str)) {
      std::istringstream ss(str);
      ss >> command;
      try {
          if (command == "Add") {
              ss >> date >> event;
              db.AddEvent(Date(date), event);
          } else if (command == "Del") {
              ss >> date >> event;
              if (event[0]) {
                  db.DeleteEvent(Date(date), event);
              } else {
                  db.DeleteDate(Date(date));
              }
          } else if (command == "Find") {
              ss >> date;
              db.Find(Date(date));
          } else if (command == "Print") {
              db.Print();
          } else {
              if (command[0] != char(0)) {
                  std::cout << "Unknown command: " << command << std::endl;
              }
          }
      }
      catch(const std::exception& e)
      {
          std::cout << e.what() << '\n';
      }
      
  }

  return 0;
}