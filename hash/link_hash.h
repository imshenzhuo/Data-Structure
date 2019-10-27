#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class Employee {
    public:
        Employee(string n, double s, int se) :
            name(n), salary(s), seniority(se) {}
        const string & getName() const { return name; }
        bool operator==(const Employee &rhs) const {
            return getName() == rhs.getName();
        }
        bool operator!=(const Employee & rhs) const {
            return !(*this == rhs);
        }
    private:
        string name;
        double salary;
        int seniority;
};


template <typename HashedObj>
class hashtable_link{
    public:
        explicit hashtable_link(int size = 101)
        {
            hashtable.resize(size);
        }
        
        bool contains(const HashedObj & x) const{
            const list<HashedObj> &whichList = hashtable[myhash(x)];
            return find(whichList.begin(), whichList.end(), x) != whichList.end();
        }

        void makeEmpty() {
            for (int i=0; i<hashtable.size(); i++) {
                hashtable[i].clear();
            }
        }
        void insert(const HashedObj & x) {
            list<HashedObj> &whichList = hashtable[myhash(x)];
            if (find(whichList.begin(), whichList.end(), x) != whichList.end()) 
                return;
            whichList.push_back(x);
            if (++currentSize > hashtable.size())
                rehash();
        }
        void remove(const HashedObj & x) {
            list<HashedObj>& whichList = hashtable[myhash(x)];
            typename list<HashedObj>::iterator itr = find(whichList.cbegin(), whichList.cend(), x);

            if(itr == whichList.cend()) {
                return false;
            }
            whichList.erase(itr);
            --currentSize();
            return true;
        }

    private:
        vector<list<HashedObj>> hashtable;
        // 当前存储数据的个数
        int currentSize;

        void rehash() {
            auto old = hashtable;
            hashtable.resize( /*nextPrime*/(2 * hashtable.size()));
            for (int i = 0; i < hashtable.size(); i++)
            {
                hashtable[i].clear();
            }
            
            currentSize = 0;
            for (int i = 0; i < old.size(); i++)
            {
                typename list<HashedObj>::iterator itr = old[i].begin();
                while (itr != old[i].end())
                {
                    insert(*itr++);
                }   
            }
        }
        int myhash(const HashedObj & x) const {
            hash<string> hasher;
            size_t hashVal = hasher(x.getName());
            hashVal %= hashtable.size();
            if (hashVal < 0){
                hashVal += hashtable.size();
            }
            return hashVal;
        }
};
