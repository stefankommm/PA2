#ifndef __PROGTEST__

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

    friend bool operator==(const CInvoice& lhs, const CInvoice& rhs) {
        return lhs.c_date.compare(rhs.c_date) == 0 && lhs.c_seller == rhs.c_seller && lhs.c_buyer == rhs.c_buyer &&
               lhs.c_amount == rhs.c_amount && lhs.c_vat == rhs.c_vat;
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
public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    CSortOpt(void);

    CSortOpt &addKey(int key,
                     bool ascending = true);

private:
    // todo
};


class CVATRegister {
public:
    CVATRegister(void) = default;

    bool registerCompany(const string &name) {
        const string normalizedName =name; // ZMEN TOOOO TODO:ZMEN
        if(firmy.count(normalizedName) == 0){
            firmy[normalizedName] = normalizedName;
            vystavene_faktury[normalizedName] = {};
            prijate_faktury[normalizedName] = {};
            return true;
        }
        return false;
    };

    bool addIssued(const CInvoice &x) {
        const string buyer_normalized = normalizeName(x.seller());
        const string seller_normalized = normalizeName(x.buyer());

        if(firmy.count(buyer_normalized) == 0 || firmy.count(seller_normalized) == 0 || seller_normalized == buyer_normalized){
            return false;
        }

        // Najdi referenciu na vektor CInvoices sellera
        auto & zoznam_invoices = vystavene_faktury[seller_normalized];
        // Ak dany Invoice uz existuje != koncu pridaj ho
        if (find(zoznam_invoices.begin() , zoznam_invoices.end(), x) == zoznam_invoices.end()) {
            zoznam_invoices.push_back(x);
            return true;
        };
        return false;
    }

    bool addAccepted(const CInvoice &x) {
        const string & buyer_normalized = normalizeName(x.seller());
        const string & seller_normalized = normalizeName(x.buyer());

        if(firmy.count(buyer_normalized) == 0 || firmy.count(seller_normalized) == 0 || seller_normalized == buyer_normalized){
            return false;
        }

        auto & prijate = prijate_faktury[buyer_normalized];

        if (find(prijate.begin() , prijate.end(), x) == prijate.end()) {
            prijate.push_back(x);
            return true;
        };
        return false;

    };

    bool delIssued(const CInvoice &x) {
        string normalized_name = normalizeName(x.seller());
        if(firmy.count(normalized_name) == 0 ){
            return false;
        }
        auto & vystavene = vystavene_faktury[normalized_name];
        auto toRemove = find(vystavene.begin(), vystavene.end(), x);
        if(toRemove != vystavene.end()){
            vystavene.erase(toRemove);
            return true;
        }
        return false;
    };

    bool delAccepted(const CInvoice &x) {
        string normalized_name = normalizeName(x.buyer());
        if(firmy.count(normalized_name) == 0 ){
            return false;
        }
        auto & prijate = prijate_faktury[normalized_name];
        auto toRemove = find(prijate.begin(), prijate.end(), x);
        if(toRemove != prijate.end()){
            prijate.erase(toRemove);
            return true;
        }
        return false;

    };

    list<CInvoice> unmatched(const string &company,
                             const CSortOpt &sortBy) const {

        auto vystavene = vystavene_faktury.find(normalizeName(company));

        list<CInvoice> notMatched;

        for(const auto & vystavena : vystavene_faktury.at(normalizeName((company)))) {
            if(find(prijate_faktury.at(company).begin(),
                    prijate_faktury.at(company).end(),
                    vystavena)
                    == prijate_faktury.at(company).end()) {
                notMatched.push_back(vystavena);
            }
        };


        for(const auto & prijata : prijate_faktury.at(normalizeName((company)))) {
            if(find(vystavene_faktury.at(company).begin(),
                    vystavene_faktury.at(company).end(),
                    prijata)
               == vystavene_faktury.at(company).end()) {
                notMatched.push_back(prijata);
            }
        };
        //TREBA ZOSORTIT

        return notMatched;
    }

private:

    const string & normalizeName(const string & nameIn) const{
        string nameLowerSingleWhite;
        // Vytvor kópiu nameIn kde sa vynechajú po sebe idúce medzery, pretransformuj znaky na lowercase
        unique_copy(nameIn.begin(), nameIn.end(), std::back_inserter(nameLowerSingleWhite),
                         [](char a, char b) {
                             char first = tolower(static_cast<unsigned char>(a));
                             char second = tolower(static_cast<unsigned char>(b));
                             return (first == ' ' && second == ' ');
                         });

        // Vymaž prebytočné medzery na konci ak nejaké sú;
        while(!nameLowerSingleWhite.empty()){
            if(nameLowerSingleWhite.back() == ' '){
                nameLowerSingleWhite.pop_back();
            }else break;
        }

        if (!nameLowerSingleWhite.empty()) {
            nameLowerSingleWhite.front() = tolower(static_cast<unsigned char>(nameLowerSingleWhite.front()));
        };

        return nameLowerSingleWhite;
    }

    map<string, string> firmy;
    map<string, list<CInvoice>> vystavene_faktury;
    map<string, list<CInvoice>> prijate_faktury;

    // todo
};

#ifndef __PROGTEST__

bool equalLists(const list<CInvoice> &a, const list<CInvoice> &b) {
    // todo
}

int main(void) {
    CVATRegister r;
    assert (r.registerCompany("first Company"));
    assert (r.registerCompany("Second     Company"));
    assert (r.registerCompany("ThirdCompany, Ltd."));
    assert (r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany("Third Company, Ltd."));
    assert (!r.registerCompany(" Third  Company,  Ltd.  "));
//    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 20)));
//    assert (r.addIssued(CInvoice(CDate(2000, 1, 2), "FirSt Company", "Second Company ", 200, 30)));
//    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 100, 30)));
//    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
//    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
//    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "Second Company ", "First Company", 300, 30)));
//    assert (r.addIssued(CInvoice(CDate(2000, 1, 1), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34)));
//    assert (!r.addIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company ", 300, 30)));
//    assert (!r.addIssued(CInvoice(CDate(2000, 1, 4), "First Company", "First   Company", 200, 30)));
//    assert (!r.addIssued(CInvoice(CDate(2000, 1, 4), "Another Company", "First   Company", 200, 30)));
//    assert (equalLists(r.unmatched("First Company",
//                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER,
//                                                                                       false).addKey(CSortOpt::BY_DATE,
//                                                                                                     false)),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000)
//                               }));
//    assert (equalLists(r.unmatched("First Company",
//                                   CSortOpt().addKey(CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(
//                                           CSortOpt::BY_BUYER, true)),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
//                                                30.000000)
//                               }));
//    assert (equalLists(r.unmatched("First Company",
//                                   CSortOpt().addKey(CSortOpt::BY_VAT, true).addKey(CSortOpt::BY_AMOUNT, true).addKey(
//                                           CSortOpt::BY_DATE, true).addKey(CSortOpt::BY_SELLER, true).addKey(
//                                           CSortOpt::BY_BUYER, true)),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000)
//                               }));
//    assert (equalLists(r.unmatched("First Company", CSortOpt()),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000)
//                               }));
//    assert (equalLists(r.unmatched("second company", CSortOpt().addKey(CSortOpt::BY_DATE, false)),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Third Company, Ltd.", "Second     Company", 400,
//                                                34.000000)
//                               }));
//    assert (equalLists(r.unmatched("last company", CSortOpt().addKey(CSortOpt::BY_VAT, true)),
//                       list<CInvoice>
//                               {
//                               }));
//    assert (r.addAccepted(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company ", 200, 30)));
//    assert (r.addAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
//    assert (r.addAccepted(CInvoice(CDate(2000, 1, 1), "Second company ", "First Company", 300, 32)));
//    assert (equalLists(r.unmatched("First Company",
//                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
//                                           CSortOpt::BY_DATE, true)),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                32.000000)
//                               }));
//    assert (!r.delIssued(CInvoice(CDate(2001, 1, 1), "First Company", "Second Company ", 200, 30)));
//    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "A First Company", "Second Company ", 200, 30)));
//    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Hand", 200, 30)));
//    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 1200, 30)));
//    assert (!r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", "Second Company", 200, 130)));
//    assert (r.delIssued(CInvoice(CDate(2000, 1, 2), "First Company", "Second Company", 200, 30)));
//    assert (equalLists(r.unmatched("First Company",
//                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
//                                           CSortOpt::BY_DATE, true)),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                32.000000)
//                               }));
//    assert (r.delAccepted(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
//    assert (equalLists(r.unmatched("First Company",
//                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
//                                           CSortOpt::BY_DATE, true)),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Third Company, Ltd.", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                32.000000)
//                               }));
//    assert (r.delIssued(CInvoice(CDate(2000, 1, 1), "First Company", " Third  Company,  Ltd.   ", 200, 30)));
//    assert (equalLists(r.unmatched("First Company",
//                                   CSortOpt().addKey(CSortOpt::BY_SELLER, true).addKey(CSortOpt::BY_BUYER, true).addKey(
//                                           CSortOpt::BY_DATE, true)),
//                       list<CInvoice>
//                               {
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                20.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 100,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "first Company", "Second     Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 2), "first Company", "Second     Company", 200,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                30.000000),
//                                       CInvoice(CDate(2000, 1, 1), "Second     Company", "first Company", 300,
//                                                32.000000)
//                               }));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */