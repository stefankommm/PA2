#ifndef UNTITLED_FOLDER_HPP
#define UNTITLED_FOLDER_HPP

#include<string>

#include"document.hpp"

using namespace std;

class Folder {
public:
    Folder (const string & _name = "");
    Folder (const Folder& other);


    friend ostream & operator << (ostream & os, const Folder & folder);
    /*
     * Adds document to the folder. Throws exception if the folder is full.
     */
    Folder & operator += (const Document & document);
    Folder & operator = (const Folder & other);
    ~Folder();

private:

    struct DocumentsData {
        Document * documents;
        int documents_count = 0;
        int reference_count = 0;
    };
    string name;
    DocumentsData * data;
    int documents_count = 0; //number of documents contained in this folder
};

#endif //UNTITLED_FOLDER_HPP

