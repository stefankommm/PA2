#ifndef __PROGTEST__
#define DEBUG
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>

using namespace std;
class CDate {
public:
    //---------------------------------------------------------------------------------------------

    CDate(int y,
          int m,
          int d)
            : m_Year(y),
              m_Month(m),
              m_Day(d){
    }

    //---------------------------------------------------------------------------------------------
    int compare(const CDate &x) const {
        if (m_Year != x.m_Year)
            return m_Year - x.m_Year;
        if (m_Month != x.m_Month)
            return m_Month - x.m_Month;
        return m_Day - x.m_Day;
    }

    //---------------------------------------------------------------------------------------------
    int year(void) const {
        return m_Year;
    }

    //---------------------------------------------------------------------------------------------
    int month(void) const {
        return m_Month;
    }

    //---------------------------------------------------------------------------------------------
    int day(void) const {
        return m_Day;
    }

    //---------------------------------------------------------------------------------------------
    friend ostream &operator<<(ostream &os,
                               const CDate &x) {
        char oldFill = os.fill();
        return os << setfill('0') << setw(4) << x.m_Year << "-"
                  << setw(2) << static_cast<int> ( x.m_Month ) << "-"
                  << setw(2) << static_cast<int> ( x.m_Day )
                  << setfill(oldFill);
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};
#endif /* __PROGTEST__ */

class CInvoice {
public:

    CInvoice(const CDate &date,
             const string &seller,
             const string &buyer,
             unsigned int amount,
             double vat) {
        c_date = date;
        c_seller = seller;
        c_buyer = buyer;
        c_amount = amount;
        c_vat = vat;
    };

    void print() {
        cout << "__________________________" << endl;
        cout << "Seller:" <<  this->seller() << endl;
        cout << "Buyer:" <<  this->buyer() << endl;
        cout << "Date:" <<  this->date() << endl;
        cout << "Amount" <<  this->amount() << endl;
        cout << "Vat" <<  this->vat() << endl;
    };

    friend bool operator==(const CInvoice& lhs, const CInvoice& rhs) {
        return lhs.c_date.compare(rhs.c_date) == 0 && lhs.c_seller == rhs.c_seller && lhs.c_buyer == rhs.c_buyer &&
               lhs.c_amount == rhs.c_amount && lhs.c_vat == rhs.c_vat;
    }

    bool operator<(const CInvoice& other) const {

        if ( int cmpDate = c_date.compare(other.c_date) == 0) {
            return true;
        }
        else if (c_seller != other.c_seller) {
            return true;
        }
        else if (c_buyer != other.c_buyer) {
            return true;
        }
        else if (c_amount != other.c_amount) {
            return true;
        }
        else if(c_vat != other.c_vat){
            return true;
        }

        return false;
    }

    CDate date(void) const {
        return c_date;
    };
    string buyer(void) const {
        return c_buyer;
    };
    string seller(void) const{
        return c_seller;
    };
    unsigned int amount(void) const {
        return c_amount;
    };
    double vat(void) const {
        return c_vat;
    };

private:
    CDate c_date = CDate(1980, 1,1);
    string c_buyer;
    string c_seller;
    unsigned int c_amount;
    double c_vat;
};

// Sortovanie Vyries
class CSortOpt {
private:

    struct opt {
        opt(int key, bool asc) : key(key), ascending(asc) {};
        bool ascending;
        int key;
    };

    // todo

public:
    vector<opt> sort_opt;
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;


    CSortOpt(void) = default;
    CSortOpt &addKey(int key, bool ascending = true){
        sort_opt.emplace_back(key, ascending);
        return *this;
    };
    const size_t sortByOpt(const opt test, const CInvoice& a, const CInvoice& b) const {
        if(test.key == BY_DATE){
            if (a.date().year() != b.date().year()) {
                if (test.ascending) {
                    return a.date().year() < b.date().year();
                } else {
                    return a.date().year() > b.date().year();
                }
            }
            if (a.date().month() != b.date().month()) {
                if (test.ascending) {
                    return a.date().month() < b.date().month();
                } else {
                    return a.date().month() > b.date().month();
                }
            }
            if (a.date().day() != b.date().day()) {
                if (test.ascending) {
                    return a.date().day() < b.date().day();
                } else {
                    return a.date().day() > b.date().day();
                }
            }
            return 2;
        }
        if(test.key == BY_BUYER){
            if(a.buyer() == b.buyer()) return 2;
            if(test.ascending){
                return a.buyer().compare(b.buyer());
            }
            else {
                return !a.buyer().compare(b.buyer());
            }
        }
        if(test.key == BY_SELLER){
            if(a.seller() == b.seller()) return 2;
            if(test.ascending){
                return a.seller().compare(b.seller());
            }
            else {
                return !a.seller().compare(b.seller());
            }
        }
        if(test.key == BY_AMOUNT) {
            if(a.amount() == b.amount())return 2;
            if(test.ascending){
                return a.amount() < b.amount();
            }
            else {
                return a.amount() > b.amount();
            }
        }
        if(test.key == BY_VAT) {
            if(test.ascending){
                return a.vat() < b.vat();
            }else {
                return a.vat() > b.vat();
            }

        }

        #ifdef DEBUG
        cout << "Sorting by nothing";
        #endif
        return false;
    };
};

class CVATRegister {
public:
    CVATRegister(void) = default;

    bool registerCompany(const string &name) {

        auto res = registrovaneMena.insert(name);

        if(!res.second) {
            cout << "meno som nezaregistroval:" << name << endl;
            return false;
        };


        std::shared_ptr<std::string> name_ptr = std::make_shared<std::string>(*res.first);
        if(mena.count(normalizeName(name)) != 0){
            return false;
        };
        mena[normalizeName(name)] = name_ptr;
        return true;
    };

    bool findName(const string & name) {
        return !(mena.find(name) == mena.end());

    }

//    bool findInvoice(const CInvoice & inv) {
//        return !(invoices.find(inv) != invoices.end());
//    }


    bool addIssued(const CInvoice &toInsert) {
        string buyer_normalized = normalizeName(toInsert.buyer());
        string seller_normalized = normalizeName(toInsert.seller());

        if(!findName(buyer_normalized) || !findName(seller_normalized)){
            return false;
        }

        auto iter = invoices.find(toInsert);
//        Invoice este nie je v liste
        if(iter == invoices.end()) {
            invoices[toInsert] = CState(mena[seller_normalized], mena[buyer_normalized], true, false);
        } else {
            if(invoices[toInsert].issued){
                return false;
            }
            invoices[toInsert].issued = true;
        }
        return true;
    }

    bool addAccepted(const CInvoice &toInsert) {
        string buyer_normalized = normalizeName(toInsert.buyer());
        string seller_normalized = normalizeName(toInsert.seller());

        if(!findName(buyer_normalized) || !findName(seller_normalized)){
            return false;
        }

        auto iter = invoices.find(toInsert);
//        Invoice este nie je v liste
        if(iter == invoices.end()) {
            invoices[toInsert] = CState(mena[seller_normalized], mena[buyer_normalized], false, true);
        } else {
            invoices[toInsert].accepted = true;
        }
        return true;

    };

    bool delIssued(const CInvoice &toRemove) {
        string buyer_normalized = normalizeName(toRemove.buyer());
        string seller_normalized = normalizeName(toRemove.seller());

        if(!findName(buyer_normalized) || !findName(seller_normalized)){
            return false;
        }
        auto iter = invoices.find(toRemove);
        if(iter == invoices.end()) {
            return false;
        }
        if(iter->second.accepted){
            iter->second.issued = false;
        }else {
            invoices.erase(iter);
        }
        return true;
    };

    bool delAccepted(const CInvoice &toRemove) {
        string buyer_normalized = normalizeName(toRemove.buyer());
        string seller_normalized = normalizeName(toRemove.seller());

        if(!findName(buyer_normalized) || !findName(seller_normalized)){
            return false;
        }
        auto iter = invoices.find(toRemove);
        if(iter == invoices.end()) {
            return false;
        }
        if(iter->second.issued){
            iter->second.accepted = false;
        }else {
            invoices.erase(iter);
        }
        return true;
    };

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy) const {
        string normalized_company = normalizeName(company);



    }
    const string normalizeName(const string nameIn) const{
        string normalizedName;

        bool passedStart = false;
        char prev;

        for (char a : nameIn) {
            if (!passedStart) {
                if (a == ' ') {
                    continue;
                } else {
                    passedStart = true;
                    prev = tolower(a);
                    normalizedName += prev;
                }
            } else {
                if (a != ' ' || prev != ' ') {
                    prev = tolower(a);
                    normalizedName += prev;
                }
            }
        }

        if(normalizedName.back() == ' '){
            normalizedName.pop_back();
        }
//        #ifdef DEBUG
//            cout << "START_NormalizedName____" << endl;
//            cout << nameIn << endl;
//            cout << normalizedName << endl;
//            cout << "'" << normalizedName << "'" << endl;
//            cout << "END_NormalizedName____" << endl;
//        #endif


        return normalizedName;
    }
private:


//
//    map<string, string> firmy;
//    map<string, list<CInvoice>> vystavene_faktury;
//    map<string, list<CInvoice>> prijate_faktury;

//    // Normalizovane na Registrovane meno

    struct CState {
        CState() = default;
        CState(const shared_ptr<string> sel, const shared_ptr<string> buy, bool issued, bool accepted)
        :seller(sel),buyer(buy), issued(issued), accepted(accepted) {};

        shared_ptr<string> seller;
        shared_ptr<string> buyer;
        bool issued = false;
        bool accepted = false;
    };

    map<string, shared_ptr<string>> mena;

    // Pointer Registrovaneho mena na Pair <SELLER registrovaneMeno, BUYER registrovaneMeno>
    map<CInvoice, CState> invoices;

    set<string> registrovaneMena;







    // todo
};

#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
    // todo
}

int main ( void )
{
    CVATRegister r;
    assert ( r . registerCompany ( "first Company" ) );
    assert ( r . registerCompany ( "Second     Company" ) );
    assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
    assert ( r . registerCompany ( "Third Company, Ltd." ) );
    assert ( !r . registerCompany ( "Third Company, Ltd." ) );
    assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
    assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
    assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
                                  } ) );
    assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
                          list<CInvoice>
                                  {
                                  } ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
    assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );
    assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );
    assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
    assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
                          list<CInvoice>
                                  {
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
                                          CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
                                  } ) );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
