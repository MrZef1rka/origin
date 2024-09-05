#include <iostream>
#include <pqxx/pqxx>
#include <Windows.h>
#include <string>
#include <tuple>

#pragma execution_character_set("utf-8")
using namespace std;


class Controller {
public:
	Controller(pqxx::connection& db_conn) : conn(db_conn) {};
	~Controller() {};

	void createBD() {

		try {
			pqxx::work tx(conn);
			tx.exec("CREATE TABLE IF NOT EXISTS clients ("
			"id SERIAL PRIMARY KEY, "
			"name VARCHAR(100), "
			"surname VARCHAR(100), "
			"email VARCHAR(100) UNIQUE);");
			tx.exec("CREATE TABLE IF NOT EXISTS phones ("
			"id SERIAL PRIMARY KEY, "
			"client_id INTEGER REFERENCES clients(id) ON DELETE CASCADE, "
			"phone_number VARCHAR(15));");
			tx.commit();
			cout << "Таблицы созданы!" << endl;
		} catch (const exception& e) {
			cout << "Ошибка при создании таблиц! " << e.what() << endl;
		}
	}

	void checkTable() {

		try
		{
			pqxx::work tx(conn);

			auto res = tx.exec("SELECT clients.id, name, surname, email, phone_number FROM clients "
				"LEFT JOIN phones ON clients.id = phones.client_id;");

			if (res.empty()) {
				cout << "Таблица пуста." << endl;
			}
			else {
				for (auto row : res) {

					cout << "ID: " << row[0].as<int>() << " | "
						 << "Имя: " << row[1].as<string>() << " | "
						 << "Фамилия: " << row[2].as<string>() << " | "
						 << "Email: " << row[3].as<string>() << " | "
						 << "Телефон: " << (row[4].is_null() ? "Нет телефона" : row[4].as<string>()) << endl;
				}
			}
		} catch (const exception& e)
		{
			cout << "Ошибка при формировании таблицы! " << e.what() << endl;
		}
	}

	void addUser() {
		cout << "Введите имя: ";
		cin.ignore();
		getline(cin, name);
		cout << "Введите фамилию: ";
		getline(cin, surname);
		cout << "Введите email: ";
		getline(cin, email);

		try
		{
			pqxx::work tx(conn);


			if (name.empty() || surname.empty() || email.empty()) {
				cout << "Ошибка: все поля должны быть заполнены." << endl;
				return;
			}

			string query = "INSERT INTO clients (name, surname, email) VALUES (" 
					+ tx.quote(name) + ", "
					+ tx.quote(surname) + ", "
					+ tx.quote(email) + ");";

			tx.exec(query);
			tx.commit();
			cout << "Клиент добавлен!" << endl;
		}
		catch (const exception& e)
		{
			cout << "Ошибка при добавлении клиента! " << e.what() << endl;
		}
	}

	void addNumber() {
		int client_id;
		cout << "Введите ID клиента: ";

		while (!(cin >> client_id)) {
			cout << "Ошибка: введен неверный ID клиента. Пожалуйста, введите число: ";
			cin.clear(); 
			cin.ignore(10000, '\n');
		}

		cin.ignore(); 

		cout << "Введите номер телефона: ";
		getline(cin, phone_number);

		try {
			pqxx::work tx(conn);
			tx.exec("INSERT INTO phones (client_id, phone_number) VALUES ("
				+ tx.quote(client_id) + ", "
				+ tx.quote(phone_number) + ");");
			tx.commit();
			cout << "Номер телефона добавлен!" << endl;
		}
		catch (const std::exception& e) {
			cout << "Ошибка при добавлении телефона! " << e.what() << endl;
		}
	}


	void changeUser() {
		int client_id;
		cout << "Введите ID клиента, которого хотите изменить: ";

		while (!(cin >> client_id)) {
			cout << "Ошибка: введен неверный ID клиента. Пожалуйста, введите число: ";
			cin.clear();
			cin.ignore(10000, '\n'); 
		}
		cin.ignore();

		cout << "Введите новое имя: ";
		cin >> name;
		cout << "Введите новую фамилию: ";
		cin >> surname;
		cout << "Введите новый email: ";
		cin >> email;

		try {
			pqxx::work tx(conn);
			tx.exec("UPDATE clients SET "
				"name = " + tx.quote(name) + ", "
				"surname = " + tx.quote(surname) + ", "
				"email = " + tx.quote(email) +
				" WHERE id = " + tx.quote(client_id) + ";");
			tx.commit();
			cout << "Данные клиента обновлены!" << endl;
		}
		catch (const std::exception& e) {
			cout << "Ошибка при изменении данных клиента! " << e.what() << endl;
		}
	
	}

	void delNumber() {
		int client_id;
		cout << "Введите ID клиента, у которого хотите удалить номер телефона: ";

		while (!(cin >> client_id)) {
			cout << "Ошибка: введен неверный ID клиента. Пожалуйста, введите число: ";
			cin.clear(); 
			cin.ignore(10000, '\n'); 
		}
		cin.ignore(); 

		try {
			pqxx::work tx(conn);

			// Получение всех номеров телефонов клиента
			pqxx::result res = tx.exec("SELECT id, phone_number FROM phones WHERE client_id = " + tx.quote(client_id) + ";");

			if (res.empty()) {
				cout << "У клиента с ID " << client_id << " нет номеров телефонов." << endl;
				return;
			}

			cout << "Номера телефонов клиента с ID " << client_id << ":" << endl;
			for (auto row : res) {
				cout << "ID телефона: " << row[0].as<int>() << " | Номер: " << row[1].as<string>() << endl;
			}

			int phone_id;
			cout << "Введите ID телефона, который хотите удалить: ";

			while (!(cin >> phone_id)) {
				cout << "Ошибка: введен неверный ID телефона. Пожалуйста, введите число: ";
				cin.clear(); 
				cin.ignore(10000, '\n'); 
			}
			cin.ignore(); 

			pqxx::result del_res = tx.exec("DELETE FROM phones WHERE id = " + tx.quote(phone_id) + " AND client_id = " + tx.quote(client_id) + ";");

			if (del_res.affected_rows() > 0) {
				cout << "Телефон с ID " << phone_id << " удален!" << endl;
			}
			else {
				cout << "Ошибка: телефон с ID " << phone_id << " не найден у клиента с ID " << client_id << "." << endl;
			}

			tx.commit();
		}
		catch (const std::exception& e) {
			cout << "Ошибка при удалении телефона! " << e.what() << endl;
		}
	}

	void delUser() {
		int client_id;
		cout << "Введите ID клиента, которого хотите удалить: ";
		while (!(cin >> client_id)) {
			cout << "Ошибка: введен неверный ID клиента. Пожалуйста, введите число: ";
			cin.clear(); // Сброс состояния ввода
			cin.ignore(10000, '\n'); // Игнорирование оставшегося ввода
		}
		cin.ignore();

		try {

			pqxx::work tx(conn);
			pqxx::result checkRes = tx.exec("SELECT COUNT(*) FROM clients WHERE id = " + tx.quote(client_id) + ";");

			int count = checkRes[0][0].as<int>();
			if (count == 0) {
				cout << "Клиент с ID " << client_id << " не существует." << endl;
				return;
			}

			tx.exec("DELETE FROM clients WHERE id = " + tx.quote(client_id) + ";");
			cout << "Клиент c ID " << client_id << " удален!" << endl;
			tx.commit();
		}
		catch (const std::exception& e) {
			cout << "Ошибка при удалении клиента! " << e.what() << endl;
		}
	}

	void findeUser() {
		cout << "Введите данные для поиска (имя, фамилия, email или телефон): ";
		string search_query;
		cin >> search_query;

		try {
			pqxx::work tx(conn);
			pqxx::result r = tx.exec("SELECT clients.id, name, surname, email, phone_number FROM clients "
				"LEFT JOIN phones ON clients.id = phones.client_id "
				"WHERE name ILIKE " + tx.quote('%' + search_query + '%') +
				" OR surname ILIKE " + tx.quote('%' + search_query + '%') +
				" OR email ILIKE " + tx.quote('%' + search_query + '%') +
				" OR phone_number ILIKE " + tx.quote('%' + search_query + '%') + ";");
			if (r.empty()) {
				cout << "Клиент не найден." << endl;
			}
			else {
				for (auto row : r) {
					cout << "ID: " << row[0].as<int>() << " | "
						<< "Имя: " << row[1].as<string>() << " | "
						<< "Фамилия: " << row[2].as<string>() << " | "
						<< "Email: " << row[3].as<string>() << " | "
						<< "Телефон: " << (row[4].is_null() ? "Нет телефона" : row[4].as<string>()) << endl;
				}
			}
		}
		catch (const std::exception& e) {
			cout << "Ошибка при поиске клиента! " << e.what() << endl;
		}
	}
private:
	pqxx::connection& conn;
	string name;
	string surname;
	string email;
	string phone_number;
};

int main()
{

	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	//setlocale(LC_ALL, "RU");

	int selection = 0;

	try {

		pqxx::connection conn(
			"host=localhost "
			"port=5432 "
			"dbname = postgres "
			"user = TestC++ "
			"password = password "
			"client_encoding=UTF8");

		cout << "Подключение к базе успешно!" << endl;

		Controller db(conn);

		while (selection != 9) {
			cout << endl;
			cout << "Что хотите сделать:" << endl;
			cout <<
				"1. Создать таблицу \n"
				"2. Просмотреть таблицу \n"
				"3. Добавить нового клиента \n"
				"4. Добавить телефон для существующего клиента \n"
				"5. Изменить данные о клиенте \n"
				"6. Удалить телефон у существующего клиента \n"
				"7. Удалить существующего клиента \n"
				"8. Найти клиента по его данным — имени, фамилии, email или телефону \n"
				"9. Выйти \n";
			cout << "Ваш выбор: ";
			cin >> selection;

			switch (selection)
			{
			case 1: db.createBD();
				break;
			case 2: db.checkTable();
				break;
			case 3: db.addUser();
				break;
			case 4: db.addNumber();
				break;
			case 5: db.changeUser();
				break;
			case 6: db.delNumber();
				break;
			case 7: db.delUser();
				break;
			case 8: db.findeUser();
				break;
			case 9:
				cout << "Выход из программы...";
				break;
			default:
				cout << "Неверный выбор!" << endl;
				break;
			}

		}
	}
	catch (const exception& e)
	{
		cout << "Error happened: " << e.what() << endl;
	}


	return 0;
}