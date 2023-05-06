#ifndef __PROGTEST__

#include <cstdlib>
#include <cassert>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <deque>
#include <unordered_map>

#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */


template<typename M_>
class CContest {

private:
    class CompWrapper {
    private:
        int (*comparator)(const M_ &x);

    public:
        explicit CompWrapper(int (*c)(const M_ &x)) {
            comparator = c;
        }

        int operator()(const M_ &x) const {
            return comparator(x);
        }
    };

    class Person {
    public:
        Person() {
            won_against = {};
            count_lost_against = 0;
        };
        int count_lost_against;
        vector<Person *> won_against;
    };


    template<class M_COMP>
    unordered_map<string, Person> generatePeopleToHandle(bool &solvable, const M_COMP &comparator) const{
        unordered_map<string, Person> people_to_handle;

        for (const auto &per: m_matches) {
            int res = comparator(per.second);
            if (res < 0) {
                // Vyhral druhy nad prvym
                people_to_handle[per.first.second].won_against.push_back(&people_to_handle[per.first.first]);
                people_to_handle[per.first.first].count_lost_against++;
            } else if (res == 0) {
                // Nastala remiza, nemoze existovat clovek ktory remizoval
                solvable = false;
                return people_to_handle;
            } else if (res > 0) {
                // Vyhral prvy nad druhym
                people_to_handle[per.first.first].won_against.push_back(&people_to_handle[per.first.second]);
                people_to_handle[per.first.second].count_lost_against++;
            }
        }
        solvable = true;
        return people_to_handle;
    }


    list<string> generateWinners(bool &solvable, unordered_map<string, Person> &people_to_handle) const  {

        string nextPersonInList;
        int countOfNextPeople = 0;
        list<string> winnersList;

        // Prechadzaj vsetkych ludi ktori aspon raz vyhrali. Ak najdes ludi ktory neprehrali nad nikym tak su na prvej pozicii
        // Ak najdes viac tak je to chyba nemozu byt dvaja prvi. Ak je jeden tak potom vsetkym ludom nad ktorymi vyhral nastav
        // lost_against o -1 a vyhod prveho zo zoznamu. Potom toto opakuj na zvysnych az do kym neostane nikto na konci
        while (!people_to_handle.empty()) {
            for (const auto &it: people_to_handle) {
                if (it.second.count_lost_against == 0) {
                    if (countOfNextPeople++ != 0) {
                        solvable = false;
                        return winnersList;
                    }
                    nextPersonInList = it.first;
                }
            };

            if (countOfNextPeople != 1) {
                solvable = false;
                return winnersList;
            } else {
                for (auto i: people_to_handle[nextPersonInList].won_against) {
                    (i->count_lost_against) -= 1;
                }
                winnersList.push_back(nextPersonInList);
                people_to_handle.erase(nextPersonInList);
                countOfNextPeople = 0;
            }
        }
        solvable = true;
        return winnersList;
    }

    map<pair<string, string>, M_> m_matches;
    map<string, list<string>> m_Wins;

public:

    CContest() = default;

    CContest &addMatch(const std::string &contestant1, const std::string &contestant2, const M_ &result) {
        pair<string, string> pair1 = {contestant1, contestant2};
        pair<string, string> pair2 = {contestant2, contestant1};

        if (this->m_matches.count(pair1) || this->m_matches.count(pair2))
            throw std::logic_error("duplicate match");

        m_matches.emplace(pair1, std::move(result));
        return *this;
    }


    // ISORDERED - Wrapper
    bool isOrdered(int (*comparator)(const M_ &x)) const {
        CompWrapper compWrap(comparator);
        return isOrdered < CompWrapper > (compWrap);
    };


    // isOrdered ( comparator )
    template<class M_COMP>
    bool isOrdered(const M_COMP comparator) const {
        bool solvable;
        unordered_map<string, Person> people_to_handle = std::move(generatePeopleToHandle(solvable, comparator));
        if (!solvable)
            return false;
        list<string> winnersList = std::move(generateWinners(solvable, people_to_handle));
        if (!solvable) {
            return false;
        }
        return true;
    }


    // RESULTS - Wrapper
    list<string> results(int (*comparator)(const M_ &x)) const {
        CompWrapper compWrap(comparator);
        return results < CompWrapper > (compWrap);
    }

    // results ( comparator )
    template<class M_COMP>
    list<string> results(const M_COMP &comparator) {

        bool solvable;
        unordered_map<string, Person> people_to_handle = std::move(generatePeopleToHandle(solvable, comparator));
        if (!solvable) {
            throw logic_error("");
        }

        list<string> winnersList = std::move(generateWinners(solvable, people_to_handle));
        if (!solvable) {
            throw logic_error("");
        }

        return winnersList;
    }
};


#ifndef __PROGTEST__

struct CMatch {
public:
    CMatch(int a,
           int b)
            : m_A(a),
              m_B(b) {
    }

    int m_A;
    int m_B;
};

class HigherScoreThreshold {
public:
    explicit HigherScoreThreshold(int diffAtLeast)
            : m_DiffAtLeast(diffAtLeast) {
    }

    int operator()(const CMatch &x) const {
        return (x.m_A > x.m_B + m_DiffAtLeast) - (x.m_B > x.m_A + m_DiffAtLeast);
    }

private:
    int m_DiffAtLeast;
};

int HigherScore(const CMatch &x) {
    return (x.m_A > x.m_B) - (x.m_B > x.m_A);
}

int main() {
    CContest<CMatch> x;

    x.addMatch("C++", "Pascal", CMatch(10, 3))
            .addMatch("C++", "Java", CMatch(8, 1))
            .addMatch("Pascal", "Basic", CMatch(40, 0))
            .addMatch("Java", "PHP", CMatch(6, 2))
            .addMatch("Java", "Pascal", CMatch(7, 3))
            .addMatch("PHP", "Basic", CMatch(10, 0));


    assert (!x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        cout << "B";
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    x.addMatch("PHP", "Pascal", CMatch(3, 6));

    assert (x.isOrdered(HigherScore));
    try {
        list<string> res = x.results(HigherScore);
        assert ((res == list<string>{"C++", "Java", "Pascal", "PHP", "Basic"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }


    assert (!x.isOrdered(HigherScoreThreshold(3)));
    try {
        list<string> res = x.results(HigherScoreThreshold(3));
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    assert (x.isOrdered([](const CMatch &x) {
        return (x.m_A < x.m_B) - (x.m_B < x.m_A);
    }));
    try {
        list<string> res = x.results([](const CMatch &x) {
            return (x.m_A < x.m_B) - (x.m_B < x.m_A);
        });
        assert ((res == list<string>{"Basic", "PHP", "Pascal", "Java", "C++"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    CContest<bool> y;

    y.addMatch("Python", "PHP", true)
            .addMatch("PHP", "Perl", true)
            .addMatch("Perl", "Bash", true)
            .addMatch("Bash", "JavaScript", true)
            .addMatch("JavaScript", "VBScript", true);

    assert (y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ((res == list<string>{"Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript"}));
    }
    catch (...) {
        assert ("Unexpected exception!" == nullptr);
    }

    y.addMatch("PHP", "JavaScript", false);
    assert (!y.isOrdered([](bool v) {
        return v ? 10 : -10;
    }));
    try {
        list<string> res = y.results([](bool v) {
            return v ? 10 : -10;
        });
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("PHP", "JavaScript", false);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }

    try {
        y.addMatch("JavaScript", "PHP", true);
        assert ("Exception missing!" == nullptr);
    }
    catch (const logic_error &e) {
    }
    catch (...) {
        assert ("Invalid exception thrown!" == nullptr);
    }
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
