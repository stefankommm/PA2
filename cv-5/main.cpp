#include <iostream>
#include <string>
#include"document.hpp"
#include"folder.hpp"

using namespace std;

int main() {
    Folder folder_agrofert("Agrofert");
    Folder folder_brexit("Brexit");
    Document statement("Statement", 3);
    Document comments("Comments", 65);
    Document bribe("Bribe", 141);
    Document brexit_info("Brexit_info", 5434);

    /* Adding documents to the folders */
    folder_agrofert += statement;
    folder_agrofert += comments;
    folder_agrofert += bribe;
    cout << folder_agrofert << endl;

    folder_brexit += brexit_info;
    cout << folder_brexit << endl;

    /* Copy constructor */
    cout<<"=============== 1a ==============="<<endl;
    Folder folder_current = folder_agrofert;
    cout << folder_current << endl;

    /* Assign operator */
    cout<<"=============== 1b ==============="<<endl;
    folder_current = folder_brexit;
    cout << folder_current << endl;


//    /* Counted references */
//    cout<<"=============== 2 ==============="<<endl;
//    Folder folder_current_copy = folder_current;
//    Folder folder_current_copy2;
//    folder_current_copy2 = folder_current;
//
//    std::cout << folder_current_copy2 << endl;
//
//    /* See that now all folders share the same data */
//    Document report("Report", 592);
//    folder_current_copy += report;
//
//    cout<<"=============== 3 ==============="<<endl;
//    cout << folder_current << endl;
//    cout << folder_current_copy << endl;
//
//    /* Copy on write - two prints above should give different results */
    return 0;
}
