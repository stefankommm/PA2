#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */


class CPersonalAgenda {
public:

    struct Employee {
        string name;
        string surname;
        string email;
        unsigned int salary;
    };

    CPersonalAgenda(void) = default;

    ~CPersonalAgenda(void) = default;

    bool add(const string &name, const string &surname, const string &email, unsigned int salary);

    bool del(const string &name, const string &surname);

    bool del(const string &email);

    bool changeName(const string &email, const string &newName, const string &newSurname);

    bool changeEmail(const string &name, const string &surname, const string &newEmail);

    bool setSalary(const string &name, const string &surname, unsigned int salary);

    bool setSalary(const string &email, unsigned int salary);

    unsigned int getSalary(const string &name, const string &surname) const;

    unsigned int getSalary(const string &email) const;

    bool getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const;

    bool getRank(const string &email, int &rankMin, int &rankMax) const;

    bool getFirst(string &outName, string &outSurname) const;

    bool getNext(const string &name, const string &surname, string &outName, string &outSurname) const;

private:
    vector<Employee> employees;
    unsigned int size = 0;
    int iterating = 0;


    static bool compareNames(const Employee &a, const Employee &b) {
        if (a.name == b.name) {
            return a.surname < b.surname;
        }
        return a.name < b.name;
    };
    static bool sameName(const Employee &a , const Employee &b){
        return (a.name == b.name && b.surname == a.surname);
    }

};

bool CPersonalAgenda::add(const string &name, const string &surname, const string &email, unsigned int salary) {
    Employee* emp = new Employee {name, surname, email, salary};
    auto insert_to = lower_bound(employees.begin(), employees.end(), *emp, compareNames);
    if (insert_to->name == emp->name && insert_to->surname == emp->surname && insert_to != employees.end()) {
        return false;
    } else {}
    employees.insert(insert_to, *emp);
    size++;
    return true;
    // TODO
    // Handle exception if the employees can't be resized

}

bool CPersonalAgenda::del(const string &name, const string &surname) {
    Employee tmpSearchFor = {name, surname};
    auto remove_at = lower_bound(employees.begin(), employees.end(), tmpSearchFor, compareNames);
    if(remove_at == employees.end() || !sameName(*remove_at, tmpSearchFor)) return false;
    employees.erase(remove_at);
    return true;
}

bool CPersonalAgenda::del(const string &email) {
    for(int i = 0; i < size; i++){
        if(employees[i].email == email){
            employees.erase(employees.begin()+i);
            return true;
        }
    }
    return false;
}

bool CPersonalAgenda::changeName(const string &email, const string &newName, const string &newSurname) {
    for(int i = 0; i < size; i++){
        if(employees[i].email == email){
            (employees.begin()+i)->name = newName;
            (employees.begin()+i)->surname = newSurname;
            return true;
        }
    }
    return false;
}

bool CPersonalAgenda::changeEmail(const string &name, const string &surname, const string &newEmail) {
    Employee tmpSearchFor = {name, surname};
    auto change_at = lower_bound(employees.begin(), employees.end(), tmpSearchFor, compareNames);
    if(change_at == employees.end() || !sameName(*change_at, tmpSearchFor)) return false;
    change_at->email = newEmail;
    return true;
}

bool CPersonalAgenda::setSalary(const string &name, const string &surname, unsigned int salary) {
    Employee tmpSearchFor = {name, surname};
    auto change_at = lower_bound(employees.begin(), employees.end(), tmpSearchFor, sameName);
    if(change_at == employees.end())return false;
    change_at->salary = salary;
    return true;
}

bool CPersonalAgenda::setSalary(const string &email, unsigned int salary) {
    for(int i = 0; i < size; i++){
        if(employees[i].email == email){
            (employees.begin()+i)->salary = salary;
            return true;
        }
    }
    return false;
}

unsigned int CPersonalAgenda::getSalary(const string &name, const string &surname) const {
    Employee tmpSearchFor = {name, surname};
    auto change_at = lower_bound(employees.begin(), employees.end(), tmpSearchFor, sameName);
    if(change_at == employees.end())return 0;
    return (change_at->salary)
;
}

unsigned int CPersonalAgenda::getSalary(const string &email) const {
    for(int i = 0; i < size; i++){
        if(employees[i].email == email){
            return (employees.begin()+i)->salary;
        }
    }
    return false;
}

bool CPersonalAgenda::getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
    Employee tmpSearchFor = {name, surname};
    auto change_at = lower_bound(employees.begin(), employees.end(), tmpSearchFor, sameName);
    if(change_at == employees.end())return false;
    int min=0, max=0;
    unsigned int salSearch = change_at->salary;
    for(auto cur = employees.begin() ; cur <= employees.end() ; cur++){
        if(cur->salary < salSearch)min++;
        else if(cur->salary > salSearch)max++;
    }

    rankMin = min;
    rankMax = max;
    return true;
}
bool CPersonalAgenda::getRank(const string &email, int &rankMin, int &rankMax) const {
    unsigned int salSearch;
    int min=0, max=0;
    for(int i = 0; i < size; i++){
        if(employees[i].email == email){
            auto iter (employees.begin()+i);
            salSearch = iter->salary;

            for(auto cur = employees.begin() ; cur <= employees.end() ; cur++){
                if(cur->salary < salSearch)min++;
                else if(cur->salary > salSearch)max++;
            }
            rankMin = min;
            rankMax = max;
            return true;
        }
    }
    return false;
}


bool CPersonalAgenda::getFirst(string &outName, string &outSurname) const {
    if(size == 0)return false;
    auto iter = employees.begin();
    outName = iter->name;
    outSurname = iter->surname;
    return true;
}

bool CPersonalAgenda::getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
    Employee tmp  = {name, surname};
    auto change_at = upper_bound(employees.begin(), employees.end(), tmp, compareNames);
    if (change_at == employees.end()) return false;
    outName = change_at->name;
    outSurname = change_at->surname;

    return true;
}




#ifndef __PROGTEST__

int main(void) {
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;
    assert (b1.add("John", "Smith", "john", 30000));
    assert (b1.add("John", "Miller", "johnm", 35000));
    assert (b1.add("Peter", "Smith", "peter", 23000));
//    assert (b1.getFirst(outName, outSurname)
//            && outName == "John"
//            && outSurname == "Miller");
//    assert (b1.getNext("John", "Miller", outName, outSurname)
//            && outName == "John"
//            && outSurname == "Smith");
//    assert (b1.getNext("John", "Smith", outName, outSurname)
//            && outName == "Peter"
//            && outSurname == "Smith");
//    assert (!b1.getNext("Peter", "Smith", outName, outSurname));
//    assert (b1.setSalary("john", 32000));
//    assert (b1.getSalary("john") == 32000);
//    assert (b1.getSalary("John", "Smith") == 32000);
//    assert (b1.getRank("John", "Smith", lo, hi)
//            && lo == 1
//            && hi == 1);
//    assert (b1.getRank("john", lo, hi)
//            && lo == 1
//            && hi == 1);
//    assert (b1.getRank("peter", lo, hi)
//            && lo == 0
//            && hi == 0);
//    assert (b1.getRank("johnm", lo, hi)
//            && lo == 2
//            && hi == 2);
//    assert (b1.setSalary("John", "Smith", 35000));
//    assert (b1.getSalary("John", "Smith") == 35000);
//    assert (b1.getSalary("john") == 35000);
//    assert (b1.getRank("John", "Smith", lo, hi)
//            && lo == 1
//            && hi == 2);
//    assert (b1.getRank("john", lo, hi)
//            && lo == 1
//            && hi == 2);
//    assert (b1.getRank("peter", lo, hi)
//            && lo == 0
//            && hi == 0);
//    assert (b1.getRank("johnm", lo, hi)
//            && lo == 1
//            && hi == 2);
//    assert (b1.changeName("peter", "James", "Bond"));
//    assert (b1.getSalary("peter") == 23000);
//    assert (b1.getSalary("James", "Bond") == 23000);
//    assert (b1.getSalary("Peter", "Smith") == 0);
//    assert (b1.getFirst(outName, outSurname)
//            && outName == "James"
//            && outSurname == "Bond");
//    assert (b1.getNext("James", "Bond", outName, outSurname)
//            && outName == "John"
//            && outSurname == "Miller");
//    assert (b1.getNext("John", "Miller", outName, outSurname)
//            && outName == "John"
//            && outSurname == "Smith");
//    assert (!b1.getNext("John", "Smith", outName, outSurname));
//    assert (b1.changeEmail("James", "Bond", "james"));
//    assert (b1.getSalary("James", "Bond") == 23000);
//    assert (b1.getSalary("james") == 23000);
//    assert (b1.getSalary("peter") == 0);
//    assert (b1.del("james"));
//    assert (b1.getRank("john", lo, hi)
//            && lo == 0
//            && hi == 1);
//    assert (b1.del("John", "Miller"));
//    assert (b1.getRank("john", lo, hi)
//            && lo == 0
//            && hi == 0);
//    assert (b1.getFirst(outName, outSurname)
//            && outName == "John"
//            && outSurname == "Smith");
//    assert (!b1.getNext("John", "Smith", outName, outSurname));
//    assert (b1.del("john"));
//    assert (!b1.getFirst(outName, outSurname));
//    assert (b1.add("John", "Smith", "john", 31000));
//    assert (b1.add("john", "Smith", "joHn", 31000));
//    assert (b1.add("John", "smith", "jOhn", 31000));
//
//    CPersonalAgenda b2;
//    assert (!b2.getFirst(outName, outSurname));
//    assert (b2.add("James", "Bond", "james", 70000));
//    assert (b2.add("James", "Smith", "james2", 30000));
//    assert (b2.add("Peter", "Smith", "peter", 40000));
//    assert (!b2.add("James", "Bond", "james3", 60000));
//    assert (!b2.add("Peter", "Bond", "peter", 50000));
//    assert (!b2.changeName("joe", "Joe", "Black"));
//    assert (!b2.changeEmail("Joe", "Black", "joe"));
//    assert (!b2.setSalary("Joe", "Black", 90000));
//    assert (!b2.setSalary("joe", 90000));
//    assert (b2.getSalary("Joe", "Black") == 0);
//    assert (b2.getSalary("joe") == 0);
//    assert (!b2.getRank("Joe", "Black", lo, hi));
//    assert (!b2.getRank("joe", lo, hi));
//    assert (!b2.changeName("joe", "Joe", "Black"));
//    assert (!b2.changeEmail("Joe", "Black", "joe"));
//    assert (!b2.del("Joe", "Black"));
//    assert (!b2.del("joe"));
//    assert (!b2.changeName("james2", "James", "Bond"));
//    assert (!b2.changeEmail("Peter", "Smith", "james"));
//    assert (!b2.changeName("peter", "Peter", "Smith"));
//    assert (!b2.changeEmail("Peter", "Smith", "peter"));
//    assert (b2.del("Peter", "Smith"));
//    assert (!b2.changeEmail("Peter", "Smith", "peter2"));
//    assert (!b2.setSalary("Peter", "Smith", 35000));
//    assert (b2.getSalary("Peter", "Smith") == 0);
//    assert (!b2.getRank("Peter", "Smith", lo, hi));
//    assert (!b2.changeName("peter", "Peter", "Falcon"));
//    assert (!b2.setSalary("peter", 37000));
//    assert (b2.getSalary("peter") == 0);
//    assert (!b2.getRank("peter", lo, hi));
//    assert (!b2.del("Peter", "Smith"));
//    assert (!b2.del("peter"));
//    assert (b2.add("Peter", "Smith", "peter", 40000));
//    assert (b2.getSalary("peter") == 40000);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */