#include "link_hash.h"
#include <iostream>

using namespace std;
int main(int argc, char const *argv[])
{
    hashtable_link<Employee> emp;

    for (size_t i = 0; i < 1000; i++)
    {
        Employee e("sss"+i, 52, 6);
        emp.insert(e);        
    }
    
    for (size_t i = 0; i < 1000; i++)
    {
        Employee e("sss"+i, 52, 6);
        cout << emp.contains(e) << endl;
    }
    
    
    return 0;
}
