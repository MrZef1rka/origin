#include <iostream>
#include <string>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/WDate.h>
#include <pqxx/pqxx>
#include <windows.h>
using namespace std;
namespace dbo = Wt::Dbo;

class Book;
class Stock;
class Shop;
class Sale;

class Publisher {
public:
    string name;
    dbo::collection<dbo::ptr<Book>> books;

    template<class Action>
    void persist(Action& a) 
    {
        dbo::field(a, name, "name");
        dbo::hasMany(a, books, dbo::ManyToOne, "publisher");
    }
};

class Book {
public:
    string title;
    dbo::ptr<Publisher> publisher;
    dbo::collection<dbo::ptr<Stock>> stocks;
    
    template<class Action>
        void persist(Action& a) 
    {
        dbo::field(a, title, "title");
        dbo::belongsTo(a, publisher, "publisher");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "book");
       
    }
};

class Shop {
public:
    string name;
    dbo::collection<dbo::ptr<Stock>> stocks;
    
    
    template<class Action>
        void persist(Action& a) 
    {
        dbo::field(a, name, "name");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "shop");
    }
};

class Stock {
public:
    int count;
    dbo::ptr<Book> book;
    dbo::ptr<Shop> shop;
    dbo::collection< dbo::ptr<Sale> > sales;
    
    template<class Action>
        void persist(Action& a) 
    {
        dbo::field(a, count, "count");
        dbo::belongsTo(a, book, "book");
        dbo::belongsTo(a, shop, "shop");
        dbo::hasMany(a, sales, dbo::ManyToOne, "stock");

    }
};

class Sale {
public:
    double price;
    Wt::WDate date_sale;
    int count;
    dbo::ptr<Stock> stock;
    
    
    template<class Action>
        void persist(Action& a) 
    {
        dbo::field(a, price, "price");
        dbo::field(a, date_sale, "date_sale");
        dbo::field(a, count, "count");
        dbo::belongsTo(a, stock, "stock");
    }
};

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    


    return 0;
}
