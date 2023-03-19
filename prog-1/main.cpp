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
const char *FILENAME_FROM = "test.in";
const char *FILENAME_TO = "bruh.txt";

void charToBinaryFromEnd(char c, vector<int> &pushTo, int last = 8)
{
    int asciiCode = (int)(c);
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
            sum += power;
    }
    return sum;
}

int convertUTFToID(ifstream &fin, char first)
{
    char b, c, d;
    vector<int> id;
    /* 1-BAJTOVY */
    if ((first & 0x80) == 0x0)
    {
        charToBinaryFromEnd(first, id, 7);
    }
    /* 2-BAJTOVY */
    else if ((first & 0xE0) == 0xC0)
    {
        if (!(fin >> b))
            return -1;
        charToBinaryFromEnd(first, id, 5);
        charToBinaryFromEnd(b, id, 6);
    }
    /* 3-BAJTOVY */
    else if ((first & 0xF0) == 0xE0)
    {
        if (!(fin >> b >> c))
            return -1;
        charToBinaryFromEnd(first, id, 4);
        charToBinaryFromEnd(b, id, 6);
        charToBinaryFromEnd(c, id, 6);
    }
    /* 4-BAJTOVY */
    else if ((first & 0xF8) == 0xF0)
    {
        if (!(fin >> b >> c >> d))
            return -1;
        charToBinaryFromEnd(first, id, 3);
        charToBinaryFromEnd(b, id, 6);
        charToBinaryFromEnd(c, id, 6);
        charToBinaryFromEnd(d, id, 6);
    }
    else
    {
        cout << "Znak neodpoveda UTF8 kódovaniu";
        return -1;
    }
    return vectorToID(id);
}

vector<int> zeckendorf(int n)
{
    int first = 1, second = 2;
    vector<int> rep;
    /* Pridaj ukoncovaciu 1 */
    rep.push_back(1);
    
    /* Najdi najvacsie fibbonaciho cislo mensie ako n */
    while (second <= n)
    {
        int temp = second;
        second = first + second;
        first = temp;
    }

    /* Postupne odpocitavaj cisla naspat a ak cislo je obsiahle pridaj 1 */
    while (first > 0)
    {
        if (n >= first)
        {
            rep.push_back(1);
            n -= first;
        }
        else
            rep.push_back(0);
        
        int temp = second - first;
        second = first;
        first = temp;
    }
    /* Odstran prebytocnu nulu*/
    rep.pop_back();
    return rep;
}

void appendVector(vector<int> &res, vector<int> &toWrite)
{
    for (int i = 0; i < (int)toWrite.size(); i++)
        res.push_back(toWrite[i]);
}

vector<int> reverseVector(vector<int> toRev)
{
    vector<int> rev;
    for (int i = (toRev.size() - 1); i >= 0; i--)
        rev.push_back(toRev[i]);
    return rev;
}

void writeVector(vector<int> &queues, ofstream &fileTo, int max = 8)
{
    while (queues.size() >= 8)
    {
        vector<int> queue = {};
        for (int i = 0; i < 8; i++)
        {
            queue.push_back(queues[0]);
            queues.erase(queues.begin());
        }
        queue = reverseVector(queue);

        unsigned char byte = 0;
        int i = 0;
        for (int bit : queue)
        {
            byte = (byte << 1) | bit;
            i++;
            if (i >= 8)
            {
                break;
            }
        }
        fileTo.write((const char *)(&byte), sizeof(byte));
    }
}

int main()
{
    ifstream fin;
    ofstream fileTo;

    fin.open(FILENAME_FROM);
    fileTo.open(FILENAME_TO, std::ios::out | std::ios::binary);

    if (!fin.is_open() || !fin)
    {
        cout << "Nepodarilo sa otvorit subor" << endl;
        exit(1);
    }

    vector<int> toWrite;

    char first;
    while (fin.get(first))
    {
        int id = convertUTFToID(fin, first);
        if (id == -1)
        {
            cout << "Neplatny znak";
            return 0;
        }

        vector<int> FibRepresent = reverseVector(zeckendorf(id + 1));
        for (int i = 0; i < (int)FibRepresent.size(); i++)    
            toWrite.push_back(FibRepresent[i]);
        if (toWrite.size() >= 8)
            writeVector(toWrite, fileTo);
        
    };
    if (toWrite.size() != 0)
    {
        int rem = 8 - toWrite.size();
        for (int i = 0; i < rem; i++)
        {
            toWrite.push_back(0);
        }
        writeVector(toWrite, fileTo);
    }

    fin.close();
    fileTo.close();
    return 1;
}