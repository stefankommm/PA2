#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>

using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX
class CRangeList;

class CRange {
public:

    // constructor
    CRange(const long long &lo, const long long &hi) {
        if (lo > hi) throw std::logic_error("Constructor CRange: lo must be less than hi");
        else {
            this->lo = lo;
            this->hi = hi;
        }
    };

//    Print
    const friend ostream &operator<<(ostream &os, const CRange &dt) {
        if (dt.lo == dt.hi) {
            os << dt.lo;
        } else {
            os << "<" << dt.lo << ".." << dt.hi << ">";
        }
        return os;
    }

    const string toString() const {
        ostringstream os;
        os << "<";
        os << lo << ".." << hi;
        os << ">";
        return os.str();
    }

    CRangeList operator+(CRange ref) const;

    CRangeList operator-(CRange ref) const;


    long long lo;
    long long hi;
private:

};


class CRangeList {
public:
    // constructor
    CRangeList() = default;

    explicit CRangeList(const CRange &range) {
        this->operator+=(range);
    }

    // long long
    const bool includes(long long a) const {
        if (intervals.empty())return false;
        auto one_after = lower_bound(intervals.begin(), intervals.end(), a, cmpLoLong);
        if (one_after != intervals.begin() && (one_after - 1)->hi >= a) return true;
        if (one_after->lo <= a && one_after->hi >= a) return true;
        return false;
    }

    // includes rane
    const bool includes(CRange a) const {
        if (intervals.empty())return false;
        auto one_after = lower_bound(intervals.begin(), intervals.end(), a, cmpLo);
        if (one_after != intervals.begin() && (one_after - 1)->hi >= a.hi) return true;
        if (one_after->lo <= a.lo && one_after->hi >= a.hi) return true;
        return false;
    }

    CRangeList operator+(const CRange &ref);

    CRangeList operator-(const CRange &ref);

    // += range
    CRangeList &operator+=(const CRange &ref) {
        if (intervals.empty()) {
            intervals.emplace_back(ref);
        } else {
//            Novy objekt ak ref obsahuje podinterval v instervals chceme ho vymazat a nastavit lo na jeho este nizsie lo ako je ref
            CRange merged(ref.lo, ref.hi);
            auto one_before = lower_bound(intervals.begin(), intervals.end(), merged, cmpLo);
            // Skontroluj overlapping nazad
            while (one_before != intervals.begin() && (one_before - 1)->hi + 1 >= merged.lo) {
                --one_before;
                if (one_before->lo < merged.lo) merged.lo = one_before->lo;
                if (one_before->hi > merged.hi) merged.hi = one_before->hi;
                one_before = intervals.erase(one_before);
            }

            auto added = intervals.insert(one_before, merged);
            auto one_after = (added + 1);

            // Skontroluj overlapping vpredu
            while (one_after != intervals.end()) {
                if (one_after->lo <= added->hi + 1) {
                    if (one_after->hi > added->hi)
                        added->hi = one_after->hi;
                    one_after = intervals.erase(one_after);
                } else {
                    break;
                }
            }
        }
        return *this;
    }

    // += range list
    CRangeList &operator+=(const CRangeList &ref) {
        for (const auto &range: ref.intervals) {
            *this += range;
        }
        return *this;
    }

    // -= range
    CRangeList &operator-=(const CRange &ref) {

        // Skontroluj ci predtym obsahuje cely ref->rozdel a pridaj koniec / koniec minuleho je vacsi ako ref.lo uprav
        auto one_after = lower_bound(intervals.begin(), intervals.end(), ref, cmpLo);
        if (one_after != intervals.begin() && (one_after - 1)->hi >= ref.lo) {
            if ((one_after - 1)->hi > ref.hi) {
                one_after = intervals.insert(one_after, CRange(ref.hi + 1, (one_after - 1)->hi));
                (one_after - 1)->hi = ref.lo - 1;
                return *this;
            } else {
                (one_after - 1)->hi = ref.lo - 1;
            }
        }
        // ak dalsie intervaly su obsiahnute v ref->vymaz / ak narazis na koniec uprav lo na hi-1 a return
        while (one_after != intervals.end() && one_after->lo <= ref.hi + 1) {
            if (one_after->hi <= ref.hi) {
                one_after = intervals.erase(one_after);
            } else if (one_after->hi > ref.hi && one_after->lo <= ref.hi) {
                one_after->lo = ref.hi + 1;
                break;
            } else {
                break;
            }
        }

        return *this;
    }

    // -= range list
    const CRangeList &operator-=(const CRangeList &ref) {
        for (const auto &range: ref.intervals) {
            this->operator-=(range);
        }
        return *this;
    }

    // = range list
    const CRangeList &operator=(const CRangeList &ref) {
        intervals = ref.intervals;
        return *this;
    }

    // = range
    const CRangeList &operator=(const CRange &ref) {
        intervals.erase(intervals.begin(), intervals.end());
        intervals.emplace_back(ref);
        return *this;
    }


    // compare operator ==
    const bool operator==(const CRangeList &ref) const {
        if (intervals.empty() || intervals.size() != ref.intervals.size()) return false;
        auto iter_int = intervals.begin();
        auto inter_end = intervals.end();
        for (auto ref_int: ref.intervals) {
            if (iter_int->lo != ref_int.lo || iter_int->hi != ref_int.hi) return false;
            if (iter_int != inter_end)++iter_int;
        }
        return true;
    }

    // compare operator !=
    const bool operator!=(const CRangeList &ref) const {
        if (intervals.empty() || intervals.size() != ref.intervals.size()) return true;
        auto iter_int = intervals.begin();
        auto inter_end = intervals.end();
        for (auto ref_int: ref.intervals) {
            if (iter_int->lo != ref_int.lo || iter_int->hi != ref_int.hi) return true;
            if (iter_int != inter_end)++iter_int;
        }
        return false;
    }

    // stream operator <<
    const friend ostream &operator<<(ostream &os, const CRangeList &toprint) {
        os << "{";
        for (size_t i = 0; i < toprint.intervals.size(); i++) {
            os << toprint.intervals[i];
            if (i != toprint.intervals.size() - 1) {
                os << ",";
            }
        }
        os << "}";
        return os;
    }

    const string toString() {
        ostringstream os;
        os << "{";
        for (size_t i = 0; i < intervals.size(); i++) {
            os << intervals[i].toString();
            if (i != intervals.size() - 1) {
                os << ",";
            }
        }
        os << "}";
        return os.str();
    }

private:
    const static bool cmpLo(const CRange &a, const CRange &b) {
        return a.lo < b.lo;
    }

    const static bool cmpLoLong(const CRange &a, const long long &b)  {
        return a.lo < b;
    }

    vector<CRange> intervals;
};


CRangeList CRangeList::operator+(const CRange &ref) {
    this->operator+=(ref);
    return *this;
}

CRangeList CRangeList::operator-(const CRange &ref) {
    this->operator-=(ref);
    return *this;
}

CRangeList CRange::operator+(CRange ref) const{
    CRangeList tmp;
    tmp += *this;
    tmp += ref;
    return tmp;
}

CRangeList CRange::operator-(CRange ref) const {
    CRangeList tmp;
    tmp += *this;
    tmp -= ref;
    return tmp;
}


#ifndef __PROGTEST__

const string toString(const CRangeList &x) {
    ostringstream oss;
    oss << x;
    return oss.str();
}

int main(void) {
    CRangeList a, b;

    assert (sizeof(CRange) <= 2 * sizeof(long long));
    a = CRange(5, 10);
    a += CRange(25, 100);
    assert (toString(a) == "{<5..10>,<25..100>}");
    a += CRange(-5, 0);
    a += CRange(8, 50);
    assert (toString(a) == "{<-5..0>,<5..100>}");
    a += CRange(101, 105) + CRange(120, 150) + CRange(160, 180) + CRange(190, 210);
    assert (toString(a) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}");
    a += CRange(106, 119) + CRange(152, 158);
    assert (toString(a) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}");
    a += CRange(-3, 170);
    a += CRange(-30, 1000);
    assert (toString(a) == "{<-30..1000>}");
    b = CRange(-500, -300) + CRange(2000, 3000) + CRange(700, 1001);
    a += b;
    assert (toString(a) == "{<-500..-300>,<-30..1001>,<2000..3000>}");
    a -= CRange(-400, -400);
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}");
    a -= CRange(10, 20) + CRange(900, 2500) + CRange(30, 40) + CRange(10000, 20000);
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    try {
        a += CRange(15, 18) + CRange(10, 0) + CRange(35, 38);
        assert ("Exception not thrown" == nullptr);
    }
    catch (const std::logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown" == nullptr);
    }
    assert (toString(a) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    b = a;
    assert (a == b);
    assert (!(a != b));
    b += CRange(2600, 2700);
    assert (toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}");
    assert (a == b);
    assert (!(a != b));
    b += CRange(15, 15);
    assert (toString(b) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}");
    assert (!(a == b));
    assert (a != b);
    assert (b.includes(15));
        assert (b.includes(2900));
    assert (b.includes(CRange(15, 15)));
    assert (b.includes(CRange(-350, -350)));
    assert (b.includes(CRange(100, 200)));
    assert (!b.includes(CRange(800, 900)));
    assert (!b.includes(CRange(-1000, -450)));
    assert (!b.includes(CRange(0, 500)));
    a += CRange(-10000, 10000) + CRange(10000000, 1000000000);
    assert (toString(a) == "{<-10000..10000>,<10000000..1000000000>}");
    b += a;
    assert (toString(b) == "{<-10000..10000>,<10000000..1000000000>}");
    b -= a;
    assert (toString(b) == "{}");
    b += CRange(0, 100) + CRange(200, 300) - CRange(150, 250) + CRange(160, 180) - CRange(170, 170);
    assert (toString(b) == "{<0..100>,<160..169>,<171..180>,<251..300>}");
    b -= CRange(10, 90) - CRange(20, 30) - CRange(40, 50) - CRange(60, 90) + CRange(70, 80);
    assert (toString(b) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}");
#ifdef EXTENDED_SYNTAX
    CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  oss << setw ( 10 ) << 1024;
  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */