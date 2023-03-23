#ifndef __PROGTEST__
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
#endif /* __PROGTEST__ */

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
    if (sumOf[i] == 1)
      sum += power;
  return sum;
}

bool checkBetween(char s){
  return ((s & 0b11000000)==0b10000000);
}

int convertUTFToID(ifstream &fin, char first)
{
  char b, c, d;
  vector<int> id;
  /* 1-BAJTOVY */
  if ((first & 0x80) == 0x0)
    charToBinaryFromEnd(first, id, 7);
  /* 2-BAJTOVY */
  else if ((first & 0xE0) == 0xC0)
  {
    if (!(fin >> b))
      return -1;
    if(!checkBetween(b)) return -1;
    charToBinaryFromEnd(first, id, 5);
    charToBinaryFromEnd(b, id, 6);
  }
  /* 3-BAJTOVY */
  else if ((first & 0xF0) == 0xE0)
  {
    if (!(fin >> b >> c))
      return -1;
    if(!checkBetween(b)) return -1;
    if(!checkBetween(c)) return -1;
    charToBinaryFromEnd(first, id, 4);
    charToBinaryFromEnd(b, id, 6);
    charToBinaryFromEnd(c, id, 6);
  }
  /* 4-BAJTOVY */
  else if ((first & 0xF8) == 0xF0)
  {
    if (!(fin >> b >> c >> d))
      return -1;
    if(!checkBetween(b)) return -1;
    if(!checkBetween(c)) return -1;
    if(!checkBetween(d)) return -1;
    charToBinaryFromEnd(first, id, 3);
    charToBinaryFromEnd(b, id, 6);
    charToBinaryFromEnd(c, id, 6);
    charToBinaryFromEnd(d, id, 6);
  }
  else
  {
    return -1;
  }
  return vectorToID(id);
}

vector<int> utfToFibVec(int n)
{
  int prve = 1, druhe = 2;
  vector<int> rep;
  /* Pridaj ukoncovaciu 1 */
  rep.push_back(1);
  /* Najdi najvacsie fibbonaciho cislo mensie ako n */
  while (druhe <= n)
  {
    int temp = druhe;
    druhe = prve + druhe;
    prve = temp;
  }
  /* Postupne odpocitavaj cisla naspat a ak cislo je obsiahle pridaj 1 */
  while (prve > 0)
  {
    if (n >= prve)
    {
      rep.push_back(1);
      n -= prve;
    }
    else
      rep.push_back(0);

    int temp = druhe - prve;
    druhe = prve;
    prve = temp;
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

bool writeVector(vector<int> &queues, ofstream &fileTo, int max = 8)
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
        break;
    }
    fileTo.write((const char *)(&byte), sizeof(byte));
    if(fileTo.fail()){
      return false;
    }
  }
  return true;
}

void appendVector(vector<int> &res, const vector<int> &toWrite)
{
  for (int i = 0; i < (int)toWrite.size(); i++)
    res.push_back(toWrite[i]);
}

int containsWhole(vector<int> &src)
{
  for (size_t i = 1; i < src.size(); i++)
  {
    if ((src[i - 1] == 1) && (src[i] == 1))
      return i;
  }
  return -1;
}

int getIDfromFib(vector<int> rep)
{
  vector<int> fib = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711,
                     28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269, 2178309};
  int id = 0;
  for (size_t i = 0; i < rep.size(); i++)
  {
    if (rep[i] == 1)
      id += fib[i];
  }
  return (id - 1);
}

bool writeIDToFile(ofstream &fileTo, int id)
{
  if (id <= 127)
  {
    unsigned char first = static_cast<unsigned char>(id);
    fileTo.write(reinterpret_cast<const char *>(&first), sizeof(char));
    if(fileTo.fail())return false;
    return true;
  }
  else if (id <= 2047)
  {
    unsigned char first = 0b11000000 | (id >> 6);
    unsigned char second = 0b10000000 | (id & 63);
    fileTo.write((const char *)(&first), sizeof(char));
    fileTo.write((const char *)(&second), sizeof(char));
    if(fileTo.fail())return false;
    return true;
  }
  else if (id <= 65535)
  {
    unsigned char first = 0b11100000 | (id >> 12);
    unsigned char second = 0b10000000 | ((id >> 6) & 63);
    unsigned char third = 0b10000000 | (id & 63);
    fileTo.write((const char *)(&first), sizeof(char));
    fileTo.write((const char *)(&second), sizeof(char));
    fileTo.write((const char *)(&third), sizeof(char));
    if(fileTo.fail())return false;
    return true;
  }
  else if (id <= 1114111)
  {
    unsigned char first = 0b11110000 | (id >> 18);
    unsigned char second = 0b10000000 | ((id >> 12) & 63);
    unsigned char third = 0b10000000 | ((id >> 6) & 63);
    unsigned char fourth = 0b10000000 | (id & 63);
    fileTo.write((const char *)(&first), sizeof(char));
    fileTo.write((const char *)(&second), sizeof(char));
    fileTo.write((const char *)(&third), sizeof(char));
    fileTo.write((const char *)(&fourth), sizeof(char));
    if(fileTo.fail())return false;
    return true;
  }
  else
    return false;
}

bool utf8ToFibonacci(const char *inFile,
                     const char *outFile)
{
  ifstream fin;
  ofstream fileTo;

  fin.open(inFile);
  fileTo.open(outFile, std::ios::out | std::ios::binary);

  if (!fin.is_open() || !fileTo.is_open() || !fin || !fileTo)
  {
    return 0;
  }

  vector<int> toWrite = {};

  char first;
  while (fin.get(first))
  {
    int id = convertUTFToID(fin, first);
    if (id == -1)
    {
      fin.close();
      fileTo.close();
      return false;
    }

    vector<int> FibRepresent = reverseVector(utfToFibVec(id + 1));
    for (int i = 0; i < (int)FibRepresent.size(); i++)
      toWrite.push_back(FibRepresent[i]);
    if (toWrite.size() >= 8){
      if(!writeVector(toWrite, fileTo))return false;
      }
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
  return true;
}

bool fibonacciToUtf8(const char *inFile,
                     const char *outFile)
{
  ifstream fin;
  ofstream fileTo;

  fin.open(inFile, std::ios::binary);
  fileTo.open(outFile, std::ios::out | std::ios::binary);

  if (!fin.is_open() || !fin)
  {
    return 0;
  }

  vector<int> queue = {};
  char first;
  while (fin.get(first) && !fin.eof())
  {
    vector<int> byte = {};
    charToBinaryFromEnd(first, byte);
    byte = reverseVector(byte);
    appendVector(queue, byte);

   
    int till = 0;

    while ((till = containsWhole(queue)) > 0)
    {
      vector<int> c;
      for (int i = 0; i <= till; i++)
      {
        if (i != till)
          c.push_back(queue[0]);
        queue.erase(queue.begin());
      }
      if (writeIDToFile(fileTo, getIDfromFib(c)) == false)
      {
        return false;
      };
      c.clear();
    }
  }

  int till = 0;
  while ((till = containsWhole(queue)) > 0)
    {
      vector<int> c;
      for (int i = 0; i <= till; i++)
      {
        if (i != till)
          c.push_back(queue[0]);
        queue.erase(queue.begin());
      }
      if (writeIDToFile(fileTo, getIDfromFib(c)) == false)
      {
        return false;
      };
      c.clear();
    }

  for (size_t i = 0; i < queue.size(); i++)
  {
    if (queue[i] == 1)
    {
      fin.close();
      fileTo.close();
      return false;
    }
  }

  fin.close();
  fileTo.close();
  return true;
}

#ifndef __PROGTEST__

bool identicalFiles(const char *file1,
                    const char *file2)
{
  // todo
}

int main(int argc, char *argv[])
{
  assert(utf8ToFibonacci("example/src_0.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_0.fib"));
  assert(utf8ToFibonacci("example/src_1.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_1.fib"));
  assert(utf8ToFibonacci("example/src_2.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_2.fib"));
  assert(utf8ToFibonacci("example/src_3.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_3.fib"));
  assert(utf8ToFibonacci("example/src_4.utf8", "output.fib") && identicalFiles("output.fib", "example/dst_4.fib"));
  assert(!utf8ToFibonacci("example/src_5.utf8", "output.fib"));
  assert(fibonacciToUtf8("example/src_6.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_6.utf8"));
  assert(fibonacciToUtf8("example/src_7.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_7.utf8"));
  assert(fibonacciToUtf8("example/src_8.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_8.utf8"));
  assert(fibonacciToUtf8("example/src_9.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_9.utf8"));
  assert(fibonacciToUtf8("example/src_10.fib", "output.utf8") && identicalFiles("output.utf8", "example/dst_10.utf8"));
  assert(!fibonacciToUtf8("example/src_11.fib", "output.utf8"));

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */