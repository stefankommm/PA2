#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */


template<typename M_>
class CContest {
public:

    CContest() = default;

    CContest& addMatch(const std::string& contestant1, const std::string& contestant2, const M_& result)
    {
        if (this->m_matches.count({contestant1, contestant2}) || this->m_matches.count({contestant2, contestant1}))
            throw std::logic_error("duplicate match");

        m_matches.emplace(pair<string, string>(minmax(contestant1, contestant2)), move(result));
        return *this;
    }


    // ISORDERED - Wrapper
    bool isOrdered(const int (*comparator)(const M_& x)) const {
        CompWrapper compWrap(comparator);
        return isOrdered<CompWrapper>(compWrap);
    };

    // isOrdered ( comparator )
    template<class M_COMP>
    bool isOrdered(const M_COMP comparator){

        for (const auto& entry : m_matches)
        {
            const std::string& winner = comparator(entry.second) > 0 ? entry.first.first : entry.first.second;
            const std::string& loser = winner == entry.first.first ? entry.first.second : entry.first.first;
            m_Wins[winner].push_back(loser);
        }

        std::list<std::string> result;
        for (const auto& entry : m_Wins)
        {
            if (result.empty() || bfs(entry.first, result.back(), m_Wins))
            {
                result.push_back(entry.first);
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    // RESULTS - Wrapper
    list<string> results(const int (*comparator)(const M_& x)) const {
        CompWrapper compWrap(comparator);
        return results<CompWrapper>(compWrap);
    }

    // results ( comparator )
    template<class M_COMP>
    list<string> results(const M_COMP& comparator) {
        list<string> result;
        if (!orderedResults(comparator, result))
            throw std::logic_error("unordered");
        return result;
    };

    template<class M_COMP>
    bool orderedResults(const M_COMP& comparator, list<string> &result) const
    {
        map<string, list<string>> m_Wins;

        for (const auto &entry : m_matches)
        {
            const std::string &winner = comparator(entry.second) > 0 ? entry.first.first : entry.first.second;
            const std::string &loser = winner == entry.first.first ? entry.first.second : entry.first.first;
            m_Wins[winner].push_back(loser);
        }

        for (const auto &entry : m_Wins)
        {
            if (result.empty() || bfs(entry.first, result.back(), m_Wins))
            {
                result.push_back(entry.first);
            }
            else
            {
                return false;
            }
        }

        return true;
    }


private:
    class CompWrapper
    {
    private:
        int (*comparator)(const M_& x);
    public:
        CompWrapper(const int (*c)(const M_& x))
        {
            comparator = c;
        }
        int operator () ( const M_ & x ) const
        {
            return comparator(x);
        }
    };

    template <class M_COMP>
    bool bfs(const std::string &start, const std::string &end, const M_COMP& comparator) const
    {
        std::queue<std::string> q;
        std::map<std::string, bool> visited;

        q.push(start);
        visited[start] = true;

        while (!q.empty())
        {
            std::string current = q.front();
            q.pop();

            if (current == end)
                return true;

            auto it = m_Wins.find(current);
            if (it != m_Wins.end())
            {
                for (const auto &neighbor : it->second)
                {
                    if (!visited[neighbor])
                    {
                        q.push(neighbor);
                        visited[neighbor] = true;
                    }
                }
            }
        }

        return false;
    }

    map<pair<string,string>, M_> m_matches;
    map<string, list<string>> m_Wins;
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
    HigherScoreThreshold(int diffAtLeast)
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

int main(void) {
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
