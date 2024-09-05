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
			cout << "������� �������!" << endl;
		} catch (const exception& e) {
			cout << "������ ��� �������� ������! " << e.what() << endl;
		}
	}

	void checkTable() {

		try
		{
			pqxx::work tx(conn);

			auto res = tx.exec("SELECT clients.id, name, surname, email, phone_number FROM clients "
				"LEFT JOIN phones ON clients.id = phones.client_id;");

			if (res.empty()) {
				cout << "������� �����." << endl;
			}
			else {
				for (auto row : res) {

					cout << "ID: " << row[0].as<int>() << " | "
						 << "���: " << row[1].as<string>() << " | "
						 << "�������: " << row[2].as<string>() << " | "
						 << "Email: " << row[3].as<string>() << " | "
						 << "�������: " << (row[4].is_null() ? "��� ��������" : row[4].as<string>()) << endl;
				}
			}
		} catch (const exception& e)
		{
			cout << "������ ��� ������������ �������! " << e.what() << endl;
		}
	}

	void addUser() {
		cout << "������� ���: ";
		cin.ignore();
		getline(cin, name);
		cout << "������� �������: ";
		getline(cin, surname);
		cout << "������� email: ";
		getline(cin, email);

		try
		{
			pqxx::work tx(conn);


			if (name.empty() || surname.empty() || email.empty()) {
				cout << "������: ��� ���� ������ ���� ���������." << endl;
				return;
			}

			string query = "INSERT INTO clients (name, surname, email) VALUES (" 
					+ tx.quote(name) + ", "
					+ tx.quote(surname) + ", "
					+ tx.quote(email) + ");";

			tx.exec(query);
			tx.commit();
			cout << "������ ��������!" << endl;
		}
		catch (const exception& e)
		{
			cout << "������ ��� ���������� �������! " << e.what() << endl;
		}
	}

	void addNumber() {
		int client_id;
		cout << "������� ID �������: ";

		while (!(cin >> client_id)) {
			cout << "������: ������ �������� ID �������. ����������, ������� �����: ";
			cin.clear(); 
			cin.ignore(10000, '\n');
		}

		cin.ignore(); 

		cout << "������� ����� ��������: ";
		getline(cin, phone_number);

		try {
			pqxx::work tx(conn);
			tx.exec("INSERT INTO phones (client_id, phone_number) VALUES ("
				+ tx.quote(client_id) + ", "
				+ tx.quote(phone_number) + ");");
			tx.commit();
			cout << "����� �������� ��������!" << endl;
		}
		catch (const std::exception& e) {
			cout << "������ ��� ���������� ��������! " << e.what() << endl;
		}
	}


	void changeUser() {
		int client_id;
		cout << "������� ID �������, �������� ������ ��������: ";

		while (!(cin >> client_id)) {
			cout << "������: ������ �������� ID �������. ����������, ������� �����: ";
			cin.clear();
			cin.ignore(10000, '\n'); 
		}
		cin.ignore();

		cout << "������� ����� ���: ";
		cin >> name;
		cout << "������� ����� �������: ";
		cin >> surname;
		cout << "������� ����� email: ";
		cin >> email;

		try {
			pqxx::work tx(conn);
			tx.exec("UPDATE clients SET "
				"name = " + tx.quote(name) + ", "
				"surname = " + tx.quote(surname) + ", "
				"email = " + tx.quote(email) +
				" WHERE id = " + tx.quote(client_id) + ";");
			tx.commit();
			cout << "������ ������� ���������!" << endl;
		}
		catch (const std::exception& e) {
			cout << "������ ��� ��������� ������ �������! " << e.what() << endl;
		}
	
	}

	void delNumber() {
		int client_id;
		cout << "������� ID �������, � �������� ������ ������� ����� ��������: ";

		while (!(cin >> client_id)) {
			cout << "������: ������ �������� ID �������. ����������, ������� �����: ";
			cin.clear(); 
			cin.ignore(10000, '\n'); 
		}
		cin.ignore(); 

		try {
			pqxx::work tx(conn);

			// ��������� ���� ������� ��������� �������
			pqxx::result res = tx.exec("SELECT id, phone_number FROM phones WHERE client_id = " + tx.quote(client_id) + ";");

			if (res.empty()) {
				cout << "� ������� � ID " << client_id << " ��� ������� ���������." << endl;
				return;
			}

			cout << "������ ��������� ������� � ID " << client_id << ":" << endl;
			for (auto row : res) {
				cout << "ID ��������: " << row[0].as<int>() << " | �����: " << row[1].as<string>() << endl;
			}

			int phone_id;
			cout << "������� ID ��������, ������� ������ �������: ";

			while (!(cin >> phone_id)) {
				cout << "������: ������ �������� ID ��������. ����������, ������� �����: ";
				cin.clear(); 
				cin.ignore(10000, '\n'); 
			}
			cin.ignore(); 

			pqxx::result del_res = tx.exec("DELETE FROM phones WHERE id = " + tx.quote(phone_id) + " AND client_id = " + tx.quote(client_id) + ";");

			if (del_res.affected_rows() > 0) {
				cout << "������� � ID " << phone_id << " ������!" << endl;
			}
			else {
				cout << "������: ������� � ID " << phone_id << " �� ������ � ������� � ID " << client_id << "." << endl;
			}

			tx.commit();
		}
		catch (const std::exception& e) {
			cout << "������ ��� �������� ��������! " << e.what() << endl;
		}
	}

	void delUser() {
		int client_id;
		cout << "������� ID �������, �������� ������ �������: ";
		while (!(cin >> client_id)) {
			cout << "������: ������ �������� ID �������. ����������, ������� �����: ";
			cin.clear(); // ����� ��������� �����
			cin.ignore(10000, '\n'); // ������������� ����������� �����
		}
		cin.ignore();

		try {

			pqxx::work tx(conn);
			pqxx::result checkRes = tx.exec("SELECT COUNT(*) FROM clients WHERE id = " + tx.quote(client_id) + ";");

			int count = checkRes[0][0].as<int>();
			if (count == 0) {
				cout << "������ � ID " << client_id << " �� ����������." << endl;
				return;
			}

			tx.exec("DELETE FROM clients WHERE id = " + tx.quote(client_id) + ";");
			cout << "������ c ID " << client_id << " ������!" << endl;
			tx.commit();
		}
		catch (const std::exception& e) {
			cout << "������ ��� �������� �������! " << e.what() << endl;
		}
	}

	void findeUser() {
		cout << "������� ������ ��� ������ (���, �������, email ��� �������): ";
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
				cout << "������ �� ������." << endl;
			}
			else {
				for (auto row : r) {
					cout << "ID: " << row[0].as<int>() << " | "
						<< "���: " << row[1].as<string>() << " | "
						<< "�������: " << row[2].as<string>() << " | "
						<< "Email: " << row[3].as<string>() << " | "
						<< "�������: " << (row[4].is_null() ? "��� ��������" : row[4].as<string>()) << endl;
				}
			}
		}
		catch (const std::exception& e) {
			cout << "������ ��� ������ �������! " << e.what() << endl;
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

		cout << "����������� � ���� �������!" << endl;

		Controller db(conn);

		while (selection != 9) {
			cout << endl;
			cout << "��� ������ �������:" << endl;
			cout <<
				"1. ������� ������� \n"
				"2. ����������� ������� \n"
				"3. �������� ������ ������� \n"
				"4. �������� ������� ��� ������������� ������� \n"
				"5. �������� ������ � ������� \n"
				"6. ������� ������� � ������������� ������� \n"
				"7. ������� ������������� ������� \n"
				"8. ����� ������� �� ��� ������ � �����, �������, email ��� �������� \n"
				"9. ����� \n";
			cout << "��� �����: ";
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
				cout << "����� �� ���������...";
				break;
			default:
				cout << "�������� �����!" << endl;
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