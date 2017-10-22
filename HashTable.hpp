//
// Created by Pamela Tabak on 22/10/17.
//

#include <unordered_map>
#include <fstream>
#include <vector>

using namespace std;

template <typename K, typename T>
class HashTable
{
public:
    unordered_map<K, T> hashMap;

    void set(K key, T value)
    {
        this->hashMap[key] = value;
    }

    T get(K key)
    {
        return this->hashMap[key];
    }

    unordered_map<K, T> getMap ()
    {
        return this->hashMap;
    }
};