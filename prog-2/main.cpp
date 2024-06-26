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

    struct CEmployeeName;

    struct CEmployeeEmail {
        explicit CEmployeeEmail(const string &email) {
            this->email = email;
        }

        CEmployeeEmail(const string &name, const string &surname, const string &email) {
            this->name = name;
            this->surname = surname;
            this->email = email;
        }

        string name;
        string surname;
        string email;
    };

    struct CEmployeeName {
        CEmployeeName(const string &name, const string &surname) {
            this->name = name;
            this->surname = surname;
        }

        CEmployeeName(const string &name, const string &surname, const string &email, const unsigned int &salary) {
            this->name = name;
            this->surname = surname;
            this->email = email;
            this->salary = salary;
        }

        string name;
        string surname;
        unsigned int salary;
        string email;
    };

    CPersonalAgenda() = default;

    ~CPersonalAgenda() = default;

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
    vector<CEmployeeName> emp_name;
    vector<CEmployeeEmail> emp_email;
    mutable size_t next_iter = 0;

    void calculateSalary(int &rankMin, int &rankMax, unsigned int cmpTo) const {
        int min = 0, max = 0;
        for (const auto &i: emp_name) {
            if (i.salary < cmpTo)min++;
            if (i.salary == cmpTo)max++;
        }
        rankMin = min;
        rankMax = min + max - 1;
    }

    static bool compareNames(const CEmployeeName &a, const CEmployeeName &b) {
        if (a.surname == b.surname)
            return a.name < b.name;
        return a.surname < b.surname;
    };

    static bool compareEmails(const CEmployeeEmail &a, const CEmployeeEmail &b) {
        return a.email < b.email;
    };

    vector<CEmployeeName>::iterator CNameIterator(const string &name, const string &surname) {
        CEmployeeName tmp_name = CEmployeeName(name, surname);
        auto name_to = lower_bound(emp_name.begin(), emp_name.end(), tmp_name, compareNames);
        return name_to;
    }

    vector<CEmployeeName>::const_iterator CNameIteratorConst(const string &name, const string &surname) const {
        CEmployeeName tmp_name = CEmployeeName(name, surname);
        auto name_to = lower_bound(emp_name.begin(), emp_name.end(), tmp_name, compareNames);
        return name_to;
    }

    vector<CEmployeeEmail>::iterator CEmailIterator(const string &email) {
        CEmployeeEmail tmp_email = CEmployeeEmail(email);
        auto email_to = lower_bound(emp_email.begin(), emp_email.end(), tmp_email, compareEmails);
        return email_to;
    }

    vector<CEmployeeEmail>::const_iterator CEmailIteratorConst(const string &email) const {
        CEmployeeEmail tmp_email = CEmployeeEmail(email);
        auto email_to = lower_bound(emp_email.begin(), emp_email.end(), tmp_email, compareEmails);
        return email_to;
    }

};


bool CPersonalAgenda::add(const string &name, const string &surname, const string &email, unsigned int salary) {
    if (emp_name.empty()) {
        emp_name.emplace_back(name, surname, email, salary);
        emp_email.emplace_back(name, surname, email);
        return true;
    }
    auto name_to = CNameIterator(name, surname);
    auto email_to = CEmailIterator(email);

    if ((name_to = CNameIterator(name, surname)) != emp_name.end() && name_to->name == name &&
        name_to->surname == surname)
        return false;
    if (email_to != emp_email.end() && email_to->email == email) return false;

    emp_name.insert(name_to, CEmployeeName(name, surname, email, salary));
    emp_email.insert(email_to, CEmployeeEmail(name, surname, email));
    return true;
}

bool CPersonalAgenda::del(const string &name, const string &surname) {
    auto name_to = CNameIterator(name, surname);
    if (name_to == emp_name.end() || name_to->name != name || name_to->surname != surname) return false;
    auto email_to = CEmailIterator(name_to->email);
    emp_name.erase(name_to);
    emp_email.erase(email_to);
    return true;

}

bool CPersonalAgenda::del(const string &email) {
    auto email_to = CEmailIterator(email);
    if (email_to == emp_email.end() || email_to->email != email) return false;
    auto name_to = CNameIterator(email_to->name, email_to->surname);

    emp_name.erase(name_to);
    emp_email.erase(email_to);
    return true;
}

bool CPersonalAgenda::changeName(const string &email, const string &newName, const string &newSurname) {
    auto tmp_name_to = CNameIterator(newName, newSurname);
    if (tmp_name_to->name == newName && tmp_name_to->surname == newSurname) return false;

    auto email_to = CEmailIterator(email);
    if (email_to == emp_email.end() || email_to->email != email) return false;

    auto name_to = CNameIterator(email_to->name, email_to->surname);

    unsigned long tmp_salary = name_to->salary;
    emp_email.erase(email_to);
    emp_name.erase(name_to);
    this->add(newName, newSurname, email, tmp_salary);
    return true;
}

bool CPersonalAgenda::changeEmail(const string &name, const string &surname, const string &newEmail) {
    auto name_to = CNameIteratorConst(name, surname);
    if (name_to == emp_name.end() || name_to->name != name || name_to->surname != surname) return false;

    auto tmp_check = CEmailIterator(newEmail);
    if (tmp_check->email == newEmail)return 0;

    auto email_to = CEmailIterator(name_to->email);
    unsigned long tmp_salary = name_to->salary;
    emp_email.erase(email_to);
    emp_name.erase(name_to);

    this->add(name, surname, newEmail, tmp_salary);

    return true;
}

bool CPersonalAgenda::setSalary(const string &name, const string &surname, unsigned int salary) {
    auto name_to = CNameIterator(name, surname);
    if (name_to == emp_name.end() || (name_to->name != name) || (name_to->surname != surname)) return false;

    name_to->salary = salary;
    return true;
}

bool CPersonalAgenda::setSalary(const string &email, unsigned int salary) {
    auto email_to = CEmailIterator(email);
    if (email_to == emp_email.end() || email_to->email != email) return false;
    auto name_to = CNameIterator(email_to->name, email_to->surname);
    name_to->salary = salary;
    return true;
}

unsigned int CPersonalAgenda::getSalary(const string &name, const string &surname) const {
    auto name_to = CNameIteratorConst(name, surname);
    if ((emp_name.end() == name_to) || (name_to->name != name) || name_to->surname != surname) return 0;

    return name_to->salary;

}

unsigned int CPersonalAgenda::getSalary(const string &email) const {
    auto email_to = CEmailIteratorConst(email);
    if (email_to == emp_email.end() || email_to->email != email) return 0;

    auto name_to = CNameIteratorConst(email_to->name, email_to->surname);
    return name_to->salary;


}

bool CPersonalAgenda::getRank(const string &name, const string &surname, int &rankMin, int &rankMax) const {
    auto name_to = CNameIteratorConst(name, surname);
    if (emp_name.end() == name_to || name_to->name != name || name_to->surname != surname) return false;
    calculateSalary(rankMin, rankMax, name_to->salary);

    return true;

}

bool CPersonalAgenda::getRank(const string &email, int &rankMin, int &rankMax) const {

    auto email_to = CEmailIteratorConst(email);
    if (email_to == emp_email.end() || email_to->email != email) return false;
    auto name_to = CNameIteratorConst(email_to->name, email_to->surname);
    calculateSalary(rankMin, rankMax, name_to->salary);
    return true;
}

bool CPersonalAgenda::getFirst(string &outName, string &outSurname) const {
    if (next_iter >= emp_name.size())return false;
    outName = emp_name[0].name;
    outSurname = emp_name[0].surname;

    return true;
}

bool CPersonalAgenda::getNext(const string &name, const string &surname, string &outName, string &outSurname) const {
    auto name_to = CNameIteratorConst(name, surname);
    if (distance(emp_name.begin(), name_to) + (long unsigned int)1 >= emp_name.size()) return false;


    outName = (name_to + 1)->name;
    outSurname = (name_to + 1)->surname;
    return true;
}


#ifndef __PROGTEST__

int main() {
    string outName, outSurname;
    int lo, hi;

    CPersonalAgenda b1;
    assert (b1.add("John", "Smith", "john", 30000));
    assert (b1.add("John", "Miller", "johnm", 35000));
    assert (b1.add("Peter", "Smith", "peter", 23000));
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (b1.getNext("John", "Smith", outName, outSurname)
            && outName == "Peter"
            && outSurname == "Smith");
    assert (!b1.getNext("Peter", "Smith", outName, outSurname));
    assert (b1.setSalary("john", 32000));
    assert (b1.getSalary("john") == 32000);
    assert (b1.getSalary("John", "Smith") == 32000);
    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 1);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("johnm", lo, hi)
            && lo == 2
            && hi == 2);
    assert (b1.setSalary("John", "Smith", 35000));
    assert (b1.getSalary("John", "Smith") == 35000);
    assert (b1.getSalary("john") == 35000);
    assert (b1.getRank("John", "Smith", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("john", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.getRank("peter", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getRank("johnm", lo, hi)
            && lo == 1
            && hi == 2);
    assert (b1.changeName("peter", "James", "Bond"));
    assert (b1.getSalary("peter") == 23000);
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("Peter", "Smith") == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "James"
            && outSurname == "Bond");
    assert (b1.getNext("James", "Bond", outName, outSurname)
            && outName == "John"
            && outSurname == "Miller");
    assert (b1.getNext("John", "Miller", outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));
    assert (b1.changeEmail("James", "Bond", "james"));
    assert (b1.getSalary("James", "Bond") == 23000);
    assert (b1.getSalary("james") == 23000);
    assert (b1.getSalary("peter") == 0);
    assert (b1.del("james"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 1);
    assert (b1.del("John", "Miller"));
    assert (b1.getRank("john", lo, hi)
            && lo == 0
            && hi == 0);
    assert (b1.getFirst(outName, outSurname)
            && outName == "John"
            && outSurname == "Smith");
    assert (!b1.getNext("John", "Smith", outName, outSurname));
    assert (b1.del("john"));
    assert (!b1.getFirst(outName, outSurname));
    assert (b1.add("John", "Smith", "john", 31000));
    assert (b1.add("john", "Smith", "joHn", 31000));
    assert (b1.add("John", "smith", "jOhn", 31000));

    CPersonalAgenda b2;
    assert (!b2.getFirst(outName, outSurname));
    assert (b2.add("James", "Bond", "james", 70000));
    assert (b2.add("James", "Smith", "james2", 30000));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (!b2.add("James", "Bond", "james3", 60000));
    assert (!b2.add("Peter", "Bond", "peter", 50000));
    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.setSalary("Joe", "Black", 90000));
    assert (!b2.setSalary("joe", 90000));
    assert (b2.getSalary("Joe", "Black") == 0);
    assert (b2.getSalary("joe") == 0);
    assert (!b2.getRank("Joe", "Black", lo, hi));
    assert (!b2.getRank("joe", lo, hi));
    assert (!b2.changeName("joe", "Joe", "Black"));
    assert (!b2.changeEmail("Joe", "Black", "joe"));
    assert (!b2.del("Joe", "Black"));
    assert (!b2.del("joe"));
    assert (!b2.changeName("james2", "James", "Bond"));
    assert (!b2.changeEmail("Peter", "Smith", "james"));
    assert (!b2.changeName("peter", "Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter"));
    assert (b2.del("Peter", "Smith"));
    assert (!b2.changeEmail("Peter", "Smith", "peter2"));
    assert (!b2.setSalary("Peter", "Smith", 35000));
    assert (b2.getSalary("Peter", "Smith") == 0);
    assert (!b2.getRank("Peter", "Smith", lo, hi));
    assert (!b2.changeName("peter", "Peter", "Falcon"));
    assert (!b2.setSalary("peter", 37000));
    assert (b2.getSalary("peter") == 0);
    assert (!b2.getRank("peter", lo, hi));
    assert (!b2.del("Peter", "Smith"));
    assert (!b2.del("peter"));
    assert (b2.add("Peter", "Smith", "peter", 40000));
    assert (b2.getSalary("peter") == 40000);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */