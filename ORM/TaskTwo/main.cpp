#include <iostream>
#include <string>
#include <set>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/Dbo/Exception.h>
#include <windows.h>

using namespace std;
namespace dbo = Wt::Dbo;

class Publisher;
class Book;
class Shop;
class Stock;

class Publisher 
{
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

class Book 
{
public:
    string name;
    dbo::ptr<Publisher> publisher;
    dbo::collection<dbo::ptr<Stock>> stocks;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, name, "name");
        dbo::belongsTo(a, publisher, "publisher");
        dbo::hasMany(a, stocks, dbo::ManyToOne, "book");
    }
};

class Shop 
{
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

class Stock 
{
public:
    int count;
    dbo::ptr<Book> book;
    dbo::ptr<Shop> shop;

    template<class Action>
    void persist(Action& a)
    {
        dbo::field(a, count, "count");
        dbo::belongsTo(a, book, "book");
        dbo::belongsTo(a, shop, "shop");
    }
};

void fillTestData(dbo::Session& session) 
{
    dbo::Transaction t(session);

    if (session.query<int>("select count(*) from publisher").resultValue() > 0) 
    {
        cout << "Данные уже присутствуют в базе. Пропускаем добавление.\n";
        return;
    }

    auto pub1 = session.add(std::make_unique<Publisher>());
    pub1.modify()->name = "Azbuka";

    auto pub2 = session.add(std::make_unique<Publisher>());
    pub2.modify()->name = "Piter";

    auto book1 = session.add(std::make_unique<Book>());
    book1.modify()->name = "C++ Basics";
    book1.modify()->publisher = pub1;

    auto book2 = session.add(std::make_unique<Book>());
    book2.modify()->name = "Advanced C++";
    book2.modify()->publisher = pub1;

    auto book3 = session.add(std::make_unique<Book>());
    book3.modify()->name = "Modern C++";
    book3.modify()->publisher = pub2;

    auto shop1 = session.add(std::make_unique<Shop>());
    shop1.modify()->name = "Bookland";

    auto shop2 = session.add(std::make_unique<Shop>());
    shop2.modify()->name = "Knigograd";

    auto s1 = session.add(std::make_unique<Stock>());
    s1.modify()->book = book1;
    s1.modify()->shop = shop1;
    s1.modify()->count = 3;

    auto s2 = session.add(std::make_unique<Stock>());
    s2.modify()->book = book1;
    s2.modify()->shop = shop2;
    s2.modify()->count = 2;

    auto s3 = session.add(std::make_unique<Stock>());
    s3.modify()->book = book2;
    s3.modify()->shop = shop2;
    s3.modify()->count = 5;

    auto s4 = session.add(std::make_unique<Stock>());
    s4.modify()->book = book3;
    s4.modify()->shop = shop1;
    s4.modify()->count = 1;

    t.commit();
    cout << "Тестовые данные добавлены.\n";
}

void infoData(dbo::Session& session)
{
    string input;
    cout << "Введите имя или ID издателя: ";
    getline(cin, input);

    if (input.empty()) {
        cout << "Вы не ввели данные. Завершение." << endl;
        return;
    }

    dbo::ptr<Publisher> pub;
    dbo::Transaction t(session);

    try {
        long long id = stoll(input);
        pub = session.load<Publisher>(id);
    } catch (...) {
        pub = session.find<Publisher>().where("name = ?").bind(input);
    }

    if (!pub) 
    {
        cout << "Издатель не найден." << endl;
        return;
    } else
    {
        cout << "Найден издатель: " << pub->name << " (ID: " << pub.id() << ")\n";
    }

    set<string> uniqueShops;

    for (const auto& book : pub->books) 
    {
        for (const auto& stock : book->stocks) 
        {
            uniqueShops.insert(stock->shop->name);
        }
    }

    if (uniqueShops.empty()) 
    {
        cout << "Нет магазинов, продающих книги издателя " << pub->name << "." << endl;
    } 
    else 
    {
        cout << "Магазины, продающие книги издателя " << pub->name << ":" << endl;
        for (const auto& name : uniqueShops) 
        {
            cout << " - " << name << endl;
        }
    }
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    try {
        string connectionString =
            "host=localhost "
            "port=5432 "
            "dbname=TestORM "
            "user=postgres "
            "password=123456";

        auto postgres = std::make_unique<Wt::Dbo::backend::Postgres>(connectionString);

        dbo::Session session;
        session.setConnection(std::move(postgres));

        session.mapClass<Publisher>("publisher");
        session.mapClass<Book>("book");
        session.mapClass<Shop>("shop");
        session.mapClass<Stock>("stock");

        dbo::Transaction t(session);
        auto count = session.query<int>("SELECT COUNT(*) FROM information_schema.tables WHERE table_name = 'publisher'").resultValue();
        t.commit();

        if (count == 0) {
            session.createTables();
        } else {
            std::cout << "Таблицы уже существуют, пропускаем создание.\n";
        }

        try 
        {
            fillTestData(session);
        } catch (const dbo::Exception& e) 
        {
            cout << "Ошибка при заполнении тестовыми данными: " << e.what() << endl;
            return 1;
        }

        try 
        {
            infoData(session);
        } catch (const dbo::Exception& e) 
        {
            cout << "Ошибка при выполнении запроса: " << e.what() << endl;
            return 1;
        }

    } catch (const dbo::Exception& e) 
    {
        cout << "Ошибка подключения или инициализации: " << e.what() << endl;
        return 1;
    }

    cout << "Для выхода нажмите любую клавишу." << endl; 
    cin.get();
    return 0;
}
