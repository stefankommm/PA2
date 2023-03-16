#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
const char *FILENAME = "test.in";

void detectCharacter(const char c){};

class letter
{
    string letter_in_char;
    int bytes;
    int utfID;
    vector<int> arr;

public:
    letter(string in)
    {
        int bytes = in.length();
        int a;
        vector<int> b1;
        vector<int> b2;
        vector<int> b3;
        vector<int> b4;
        vector<int> join;
        cout << "bruh";
        int power = 2;

        /* 0xxxxxxx */
        for (int j = 0; j < 7; j++, power *= 2)
        {
            if (in[0] % power == 0)
            {
                join.insert(join.begin(), 1);
            }
            else
            {
                join.insert(join.begin(), 0);
            }
        }

        for (int i = 0; i < join.size(); i++)
        {
            cout << join[i] << in;
        }
        cout << endl;
    }
};

string utfToCharacter(ifstream &fin)
{
    char a;
    if (!(fin >> a))
    {
    }
    int bits = 0;
    string arr = "";
    char b, c, d;
    if ((a & 0x80) == 0x0)
    {
        arr.push_back(a);
    }
    else if ((a & 0xE0) == 0xC0)
    {
        fin >> b;
        arr.push_back(a);
        arr.push_back(b);
    }
    else if ((a & 0xF0) == 0xE0)
    {
        fin >> b >> c;
        arr.push_back(a);
        arr.push_back(b);
        arr.push_back(c);
    }
    else if ((a & 0xF8) == 0xF0)
    {
        fin >> b >> c >> d;
        arr.push_back(a);
        arr.push_back(b);
        arr.push_back(c);
        arr.push_back(d);
    }
}

void    charToBinaryFromEnd(char c, vector<int> &pushTo, int last = 8)
{
    int asciiCode = static_cast<int>(c);
    for (int i = last - 1; i >= 0; i--)
    {
        int bit = (asciiCode >> i) & 1;
        pushTo.push_back(bit);
    }
}

int vectorToID(const vector<int> &sumOf)
{
    int sum = 0;
    int power = 1;
    for (int i = sumOf.size() - 1; i >= 0; i--, power *= 2)
    {
        if (sumOf[i] == 1)
        {
            sum += power;
        }
    }
    return sum;
}

int convertUTFToID(ifstream &fin)
{
    char a, b, c, d;
    /* --------CHECK */
    if (!(fin >> a))
        exit(0);
    /* --------CHECK */

    vector<int> id;
    /* 1-BAJTOVY */
    if ((a & 0x80) == 0x0)
    {
        charToBinaryFromEnd(a, id, 7);
    }
    /* 2-BAJTOVY */
    else if ((a & 0xE0) == 0xC0)
    {
        if (!(fin >> b))
            exit(1);
        charToBinaryFromEnd(a, id, 5);
        charToBinaryFromEnd(b, id, 6);
    }
    /* 3-BAJTOVY */
    else if ((a & 0xF0) == 0xE0)
    {
        if (!(fin >> b >> c))
            exit(1);
        charToBinaryFromEnd(a, id, 4);
        charToBinaryFromEnd(b, id, 6);
        charToBinaryFromEnd(c, id, 6);
    }
    /* 4-BAJTOVY */
    else if ((a & 0xF8) == 0xF0)
    {
        if (!(fin >> b >> c >> d))
            exit(1);
        charToBinaryFromEnd(a, id, 3);
        charToBinaryFromEnd(b, id, 6);
        charToBinaryFromEnd(c, id, 6);
        charToBinaryFromEnd(d, id, 6);
    }
    else
    {
        cout << "Znak neodpoveda UTF8 kódovaniu";
        fin.close();
        exit(0);
    }

    return vectorToID(id);
}

vector<int> zeckendorf(int n)
{
    int fib1 = 1, fib2 = 2;
    vector<int> code;

    while (fib2 <= n)
    {
        int temp = fib2;
        fib2 = fib1 + fib2;
        fib1 = temp;
    }

    while (fib1 > 0)
    {
        if (n >= fib1)
        {
            code.push_back(1);
            n -= fib1;
        }
        else
        {
            code.push_back(0);
        }
        int temp = fib2 - fib1;
        fib2 = fib1;
        fib1 = temp;
    }

    vector<int> res;
    /* string result = ""; */
    for (int i = 0; i < code.size(); i++)
    {
        res.push_back(code[i]);
        /* result += to_string(code[i]); */
    }
    return res;
}

void printVector(vector<int> toPrint)
{
    for (int i = 0; i < toPrint.size(); i++)
    {
        cout << toPrint[i];
    }
}

void writeZero(ofstream &to)
{
    int zero = 0;
    cout << "pis0";
    to.write((const char *)&zero, sizeof(zero));
}

void writeOne(ofstream &to)
{
    int one = 1;
    cout << "pis1";
    to.write((const char *)&one, sizeof(one));
}
int writeVector(ofstream &to, vector<int> vec)
{
    int sum = 0;
    int size = vec.size();
    for (int i = size; i >= 0; i--)
    {
        if (vec[i] == 1)
            writeOne(to);
        else
            writeZero(to);
        sum++;
    }
    return sum + 1;
}

int main()
{
    ifstream fin;
    ofstream fileTo;

    fin.open(FILENAME);
    fileTo.open("bruhmomentos.txt", std::ios::out | std::ios::binary);
    int zero = 0b00000001;
    fileTo.write((const char *)&zero, sizeof(zero));

    if (!fin.is_open() || !fin)
    {
        cout << "Nepodarilo sa otvorit subor" << endl;
        exit(1);
    }

    int counting = 0;
    int sum = 0;
    while (!fin.eof())
    {
        int id = convertUTFToID(fin);
        cout << id << endl;
        vector<int> res = zeckendorf(id);
        res.push_back(1);

        cout << "BIN: ";
        printVector(res);
        cout << endl
             << id + 1 << endl;

        printVector(res);
        sum += writeVector(fileTo, res);
        cout << endl;
    };
    int remaining = sum % 8;
    for (int i = 0; i < remaining; i++)
    {
        writeZero(fileTo);
    }

    fin.close();
    fileTo.close();

    return 0;
}