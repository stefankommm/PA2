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
              m_Day(d) {
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
    CInvoice() : c_date(CDate(0,0,0)), c_buyer(""),c_seller(""), c_amount(0),c_vat(0){}

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



    friend bool operator==(const CInvoice &first, const CInvoice &scnd) {
        return first.c_date.compare(scnd.c_date) == 0 && first.c_seller == scnd.c_seller && first.c_buyer == scnd.c_buyer &&
               first.c_amount == scnd.c_amount && first.c_vat == scnd.c_vat;
    }

    bool operator!=(const CInvoice& druhe) const {
        return !(c_date.compare(druhe.c_date) == 0 && c_seller == druhe.c_seller && c_buyer == druhe.c_buyer && c_amount == druhe.c_amount && c_vat == druhe.c_vat);
    }

    bool operator<(const CInvoice &druhe) const {
        return (c_date.compare(druhe.c_date) < 0 ||
                (c_date.compare(druhe.c_date) == 0 && c_seller < druhe.c_seller) ||
                (c_date.compare(druhe.c_date) == 0 && c_seller == druhe.c_seller && c_buyer < druhe.c_buyer) ||
                (c_date.compare(druhe.c_date) == 0 && c_seller == druhe.c_seller && c_buyer == druhe.c_buyer && c_amount < druhe.c_amount) ||
                (c_date.compare(druhe.c_date) == 0 && c_seller == druhe.c_seller && c_buyer == druhe.c_buyer && c_amount == druhe.c_amount && c_vat < druhe.c_vat));
    }

    CDate date(void) const {
        return c_date;
    };
    string buyer(void) const {
        return c_buyer;
    };
    string seller(void) const {
        return c_seller;
    };
    unsigned int amount(void) const {
        return c_amount;
    };
    double vat(void) const {
        return c_vat;
    };
    unsigned int order() const {
        return c_order;
    }
    void setOrder(unsigned int ord) {
        c_order = ord;
    }


private:
    CDate c_date = CDate(0, 0, 0);
    string c_buyer;
    string c_seller;
    unsigned int c_amount;
    double c_vat;
    unsigned int c_order;
};
class CSortOpt {
private:


public:
    struct opt {
        opt(int key, bool asc) : ascending(asc),key(key) {};
        bool ascending;
        int key;
    };
    vector<opt> sort_opt;
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;

    CSortOpt(void) = default;

    CSortOpt &addKey(int key, bool ascending = true) {
        sort_opt.emplace_back(key, ascending);
        return *this;
    };

    vector<opt> getSortOption() const{
        return sort_opt;
    }

};
struct CState {
    CState(): seller(nullptr), buyer(nullptr), issued(false), accepted(false){};
    CState(const shared_ptr<string> sel, const shared_ptr<string> buy, bool issued, bool accepted, int order)
            : seller(sel), buyer(buy), issued(issued), accepted(accepted), order(order) {};

    shared_ptr<string> seller = nullptr;
    shared_ptr<string> buyer = nullptr;
    bool issued = false;
    bool accepted = false;
    unsigned long long order;
};
class CVATRegister {
public:
    CVATRegister(void) = default;

    bool registerCompany(const string &name) {
        auto res = registrovaneMena.insert(name);

        std::shared_ptr<std::string> name_ptr = std::make_shared<string>(*res.first);

        if (mena.find(normalizeName(name)) != mena.end()) {
            return false;
        }
        mena[normalizeName(name)] = name_ptr;
        return true;
    };
    bool addIssued(const CInvoice &toInsert) {
        string buyer_normalized = normalizeName(toInsert.buyer());
        string seller_normalized = normalizeName(toInsert.seller());

        if (mena.find(buyer_normalized) == mena.end() || mena.find(seller_normalized) == mena.end() || seller_normalized == buyer_normalized) {
            return false;
        }

        CInvoice fixed_invoice(toInsert.date(), *mena[seller_normalized], *mena[buyer_normalized], toInsert.amount(), toInsert.vat());

        auto it = invoices.find(fixed_invoice);
        if (it != invoices.end()) {
            if (it->second.issued) {
                return false;
            } else {
                it->second.issued = true;
            }
        } else {
            invoices[fixed_invoice] = CState(mena[seller_normalized], mena[buyer_normalized], true, false, invoices.size());
        }
        return true;
    }
    bool addAccepted(const CInvoice &toInsert) {
        string buyer_normalized = normalizeName(toInsert.buyer());
        string seller_normalized = normalizeName(toInsert.seller());

        if (mena.find(buyer_normalized) == mena.end() || mena.find(seller_normalized) == mena.end() || seller_normalized == buyer_normalized) {
            return false;
        }
        CInvoice fixed_inv(toInsert.date(), *mena[seller_normalized], *mena[buyer_normalized], toInsert.amount(), toInsert.vat());


        if (invoices.find(fixed_inv) != invoices.end()) {
            auto ptr = invoices.find(fixed_inv);

            auto& c_ptr = ptr->second;
            if(c_ptr.accepted){
                return false;
            }else {
                c_ptr.accepted = true;
                return true;
            }
        }else {
            invoices[fixed_inv] = CState(mena[seller_normalized], mena[buyer_normalized], false, true, invoices.size());
        }
        return true;
    };
    bool delIssued(const CInvoice &toRemove) {

        string buyer_normalized = normalizeName(toRemove.buyer());
        string seller_normalized = normalizeName(toRemove.seller());

        if (mena.find(seller_normalized) == mena.end() || mena.find(buyer_normalized) == mena.end() || seller_normalized == buyer_normalized) {
            return false;
        }
        CInvoice fixed_inv(toRemove.date(), *mena[seller_normalized], *mena[buyer_normalized], toRemove.amount(), toRemove.vat());

        auto it = invoices.find(fixed_inv);

        if (it != invoices.end()) {
            auto &state = it->second;
            if (state.issued) {
                if(state.accepted){
                    state.issued = false;
                } else {
                    invoices.erase(it);
                }
                return true;
            }
        }

        return false;
    }
    bool delAccepted(const CInvoice &toRemove) {
        string buyer_normalized = normalizeName(toRemove.buyer());
        string seller_normalized = normalizeName(toRemove.seller());

        if (mena.find(seller_normalized) == mena.end() || mena.find(buyer_normalized) == mena.end() || seller_normalized == buyer_normalized) {
            return false;
        }
        CInvoice fixed_inv(toRemove.date(), *mena[seller_normalized], *mena[buyer_normalized], toRemove.amount(), toRemove.vat());

        auto it = invoices.find(fixed_inv);

        if (it != invoices.end()) {
            auto &state = it->second;
            if (state.accepted) {
                if(state.issued){
                    state.accepted = false;
                } else {
                    invoices.erase(it);
                }
                return true;
            }
        }
        return false;
    };


    bool mojPeknyKomparator(const CSortOpt &options, const CInvoice & a, const CInvoice & b) const {
        string sel_nor_a = a.seller();
        string sel_nor_b = b.seller();
        string buy_nor_a = a.buyer();
        string buy_nor_b = b.buyer();

        for (auto option: options.sort_opt) {
            switch (option.key) {
                case CSortOpt::BY_DATE:
                    if (a.date().compare(b.date()) != 0) {
                        if (a.date().compare(b.date()) < 0){
                            return option.ascending;
                        } else {
                            return !option.ascending;
                        }
                    }
                    break;
                case CSortOpt::BY_BUYER:
                    if(a.buyer() != b.buyer()){
                        if(a.buyer() < b.buyer()){
                            return option.ascending;
                        } else return !option.ascending;
                    }
                    break;
                case CSortOpt::BY_SELLER:
                    if(sel_nor_a != sel_nor_b) {
                        if(sel_nor_a < sel_nor_b) return option.ascending;
                        else return option.ascending;
                    }
                    break;
                case CSortOpt::BY_AMOUNT:
                    if(a.amount() != b.amount()){
                        if(a.amount() < b.amount()){
                            return option.ascending;
                        } else return !option.ascending;
                    }
                    break;
                case CSortOpt::BY_VAT:
                    if(a.vat() != b.vat()){
                        if(a.vat() - b.vat() <= __DBL_EPSILON__ * abs(a.vat() + b.vat()) * 1000) return option.ascending;
                        else return !option.ascending;
                    }
                    break;

            }
        }
        return (a.order() < b.order());
    }

    bool komparatorPoradie(const CInvoice a, const CInvoice b) {
        return (a.order() < b.order());
    }

    list<CInvoice> unmatched(const string &company, const CSortOpt &sortBy)  {
        list<CInvoice> notMatched = {};
        string normalized_company = normalizeName(company);

        if (mena.find(normalized_company) == mena.end()) return notMatched;
        string normalized_name = *mena[normalized_company];


        for (const auto & i: invoices) {
            const auto & fr = i.first;
            const auto & cmp = i.second;

            if (fr.seller() == normalized_name || fr.buyer() == normalized_name) {
                if ((cmp.accepted && !cmp.issued) || (!cmp.accepted && cmp.issued)) {
                    notMatched.emplace_back(fr.date(), *cmp.seller, *cmp.buyer, fr.amount(), fr.vat());
                    notMatched.back().setOrder(cmp.order);
                }
            }
        }
        if(notMatched.empty()){
            return notMatched;
        }

        if(!sortBy.sort_opt.empty()){
            notMatched.sort([sortBy, this](const CInvoice& a, const CInvoice& b) {
                return mojPeknyKomparator(sortBy, a, b);
            });
        } else {
            notMatched.sort([this](const CInvoice& a, const CInvoice& b) {
                return komparatorPoradie(a, b);
            });
        }

        return notMatched;

    }

    const string normalizeName(const string & nameIn) const {
        string normalizedName;

        bool passedStart = false;
        char prev;

        for (char a: nameIn) {
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

        while (normalizedName.front() == ' ') {
            normalizedName.erase(0);
        }
        while (normalizedName.back() == ' ') {
            normalizedName.pop_back();
        }
        return normalizedName;
    }

private:
    map<string, shared_ptr<string>> mena;
    map<CInvoice, CState> invoices;
    unordered_set<string> registrovaneMena;
};

#ifndef __PROGTEST__

int main ( void )
{
    CVATRegister r;

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
