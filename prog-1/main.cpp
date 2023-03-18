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

int convertUTFToID(ifstream &fin, char first)
{
    char b, c, d;

    vector<int> id;
    /* 1-BAJTOVY */
    if ((first & 0x80) == 0x0)
    {
        cout << "Jednobajtovy utf" << endl;
        charToBinaryFromEnd(first, id, 7);

    }
    /* 2-BAJTOVY */
    else if ((first & 0xE0) == 0xC0)
    {
        cout << "Dvojbajtovy utf" << endl;
        if(!(fin >> b)){
            return -1;
        }
        charToBinaryFromEnd(first, id, 5);
        charToBinaryFromEnd(b, id, 6);
        
    }
    /* 3-BAJTOVY */
    else if ((first & 0xF0) == 0xE0)
    {
        cout << "Trojbajtovy utf" << endl;
        if(!(fin >> b >>c)){
            return -1;
        }
        charToBinaryFromEnd(first, id, 4);
        charToBinaryFromEnd(b, id, 6);
        charToBinaryFromEnd(c, id, 6);

    }
    /* 4-BAJTOVY */
    else if ((first & 0xF8) == 0xF0)
    {
        if(!(fin >> b >> c >> d)){
            return -1;
        }
        cout << "Stvorbajtovy utf" << endl;
        
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
    int fib1 = 1, fib2 = 2;
    vector<int> code;
    code.push_back(1);
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
    code.pop_back();

    return code;
}

void printVector(vector<int> toPrint)
{
    for (int i = 0; i < toPrint.size(); i++)
    {
        cout << toPrint[i];
    }
}

char vectorToChar(vector<int> queue)
{
    char a = '0';
    for (int i = 0; i < 8; i++)
        if (queue[i] >= 1)
        {
            a += 0b00000001 << i;
        }
    return a;
}

void writeChar(char a, ofstream fileTo)
{
    fileTo.put(a);
}

void appendVector(vector<int> &res, vector<int> &toWrite)
{
    for (int i = 0; i < toWrite.size(); i++)
    {
        res.push_back(toWrite[i]);
    }
}

vector<int> reverseVector(vector<int> toRev){
    vector<int> rev;
    for(int i = (toRev.size()-1); i>=0; i--){
        rev.push_back(toRev[i]);
    }
    return rev;
}

void writeVector(vector<int> &queues, ofstream &fileTo, int max = 8)
{
    printVector(queues);
    cout << endl;
    while(queues.size()>=8){
        vector<int> queue = {};
        for(int i=0;i<8;i++){
            queue.push_back(queues[0]);
            queues.erase(queues.begin());
        }
        printVector(queue);
        cout << endl;
        queue = reverseVector(queue);
        printVector(queue);
        cout << endl;
    
    cout << queue.size() << endl;


    unsigned char byte = 0;
    int i = 0;
    for(int bit : queue){
        byte = (byte << 1) | bit;
        i++;
        if(i >=8){
            break;
        }
    }
    fileTo.write(reinterpret_cast<const char*>(&byte), sizeof(byte));
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

    int cnt = 0;
    
    vector<int> toWrite;

    char first;
    while (fin.get(first))
    {
        /* Ziskaj ID znaku */
        int id = convertUTFToID(fin, first);
        if(id == -1){
            cout << "Neplatny znak";
            return 0;
        }
        cout << "ID ZNAKU: " << id << endl;

        /* ID znaku prekonvertuj do fibonnaciho kodu */
        vector<int> FibRepresent= reverseVector(zeckendorf(id+1));
        
        cout << "Fibonnaciho reprezentacia: ";
        printVector(FibRepresent);
        cout << endl;
        cout << "-------------------" << endl << endl;


                /* Pridaj fibonnaciho reprezentaciu do queue */
        for(int i = 0; i<FibRepresent.size();i++){
            toWrite.push_back(FibRepresent[i]);
        }
        /* Pridaj oddelovaci znak */

        /* Ak ma queue aspon osem znakov zapis do suboru bajt */
        if(toWrite.size() >= 8){
            writeVector(toWrite, fileTo);
        }

    };
    if(toWrite.size()!=0){
        int rem = 8-toWrite.size();
        for(int i=0; i< rem; i++){
            toWrite.push_back(0);
        }
        writeVector(toWrite,fileTo);
    }


    fin.close();
    fileTo.close();

    return 0;
}