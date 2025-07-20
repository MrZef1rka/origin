#include <map>
#include <memory>
#include <iostream>
#include <string>

using namespace std;

class VeryHeavyDatabase
{
public:
    virtual string get_data(const string& key)
    {
        return "Data from Data Base: " + key;
    }
    
};

class CacheDB : public VeryHeavyDatabase
{
    unique_ptr<VeryHeavyDatabase> real_object;
    map<string, string> _cache;
public:
    CacheDB(unique_ptr<VeryHeavyDatabase> object) : real_object(std::move(object)) {}
    string get_data(const string& key) override
    {
        if (_cache.find(key) == _cache.end())
        {
            cout << "From Very Heavy DB : ";
            _cache[key] = real_object->get_data(key);
        }else
        {
            cout << "From Cache DB : ";
        }
        return _cache.at(key);
    }
};

class OneShotDB : VeryHeavyDatabase
{
    VeryHeavyDatabase* _real_object;
    size_t _shots;
    size_t _used_shots;
public:
    explicit OneShotDB (VeryHeavyDatabase* real_object, size_t shots = 1) : _real_object(real_object), _shots(shots) {}
    string get_data(const string& key) override {
        if (_used_shots < _shots) {
            _used_shots++;
            return _real_object->get_data(key);
        }
        return "error";
    }
};

int main()
{
    auto realDB = make_unique<VeryHeavyDatabase>();
    auto cacheDB = make_unique<CacheDB>(std::move(realDB));
    cout << cacheDB->get_data("Key1") << endl;
    cout << cacheDB->get_data("Key1") << endl;
    cout << cacheDB->get_data("Key1") << endl;
    cout << cacheDB->get_data("Key2") << endl;

    cout << "------------------------------------------" << endl;

    auto real_db = VeryHeavyDatabase();
    auto limit_db = OneShotDB(std::addressof(real_db), 2);
    std::cout << limit_db.get_data("key") << std::endl;
    std::cout << limit_db.get_data("key") << std::endl;
    std::cout << limit_db.get_data("key") << std::endl;
    
    
    return 0;
}
