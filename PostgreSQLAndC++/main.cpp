#include <iostream>
#include <pqxx/pqxx>
#include <vector>
#include <string>
#include <map>
#include <windows.h>

struct Client {
    int id = -1;
    std::string name;
    std::string surname;
    std::string email;
    std::vector<std::string> phone_numbers;
};

class ClientManager {
public:
    ClientManager(pqxx::connection& conn) : conn(conn) {}

    void createDatabase() {
        pqxx::work tx(conn);
        tx.exec(R"(
            CREATE TABLE IF NOT EXISTS clients (
                id SERIAL PRIMARY KEY,
                name VARCHAR(100),
                surname VARCHAR(100),
                email VARCHAR(100) UNIQUE
            );
        )");
   
        tx.exec(R"(
            CREATE TABLE IF NOT EXISTS phones (
                id SERIAL PRIMARY KEY,
                client_id INTEGER REFERENCES clients(id) ON DELETE CASCADE,
                phone_number VARCHAR(15)
            );
        )");
        tx.commit();
    }
    void prepareStatements()
    {
        conn.prepare("insert_client",
            "INSERT INTO clients (name, surname, email) VALUES ($1, $2, $3) RETURNING id");

        conn.prepare("insert_phone",
            "INSERT INTO phones (client_id, phone_number) VALUES ($1, $2)");

        conn.prepare("update_client",
            "UPDATE clients SET name = $1, surname = $2, email = $3 WHERE id = $4");

        conn.prepare("delete_phone",
            "DELETE FROM phones WHERE id = $1");

        conn.prepare("delete_client",
            "DELETE FROM clients WHERE id = $1");

        conn.prepare("find_clients",
            R"(
            SELECT clients.id, name, surname, email, phone_number
            FROM clients
            LEFT JOIN phones ON clients.id = phones.client_id
            WHERE name ILIKE $1 OR surname ILIKE $1 OR email ILIKE $1 OR phone_number ILIKE $1
            )"
        );
    }

    void addClient(const Client& client) {
        pqxx::work tx(conn);
        pqxx::result r = tx.exec_prepared("insert_client", client.name, client.surname, client.email);
        int client_id = r[0][0].as<int>();
        for (const auto& phone : client.phone_numbers) {
            tx.exec_prepared("insert_phone", client_id, phone);
        }
        tx.commit();
    }

    void addPhone(int client_id, const std::string& phone) {
        pqxx::work tx(conn);
        tx.exec_prepared("insert_phone", client_id, phone);
        tx.commit();
    }

    void updateClient(int client_id, const Client& client) {
        pqxx::work tx(conn);
        tx.exec_prepared("update_client", client.name, client.surname, client.email, client_id);
        tx.commit();
    }

    void deletePhone(int phone_id) {
        pqxx::work tx(conn);
        tx.exec_prepared("delete_phone", phone_id);
        tx.commit();
    }

    void deleteClient(int client_id) {
        pqxx::work tx(conn);
        tx.exec_prepared("delete_client", client_id);
        tx.commit();
    }

    std::vector<Client> findClients(const std::string& query) {
        pqxx::work tx(conn);
        std::vector<Client> results;

        std::string pattern = "%" + query + "%";
        pqxx::result r = tx.exec_prepared("find_clients", pattern);

        std::map<int, Client> clients_map;
        for (auto row : r) {
            int id = row[0].as<int>();
            Client& client = clients_map[id];
            client.id = id;
            client.name = row[1].as<std::string>();
            client.surname = row[2].as<std::string>();
            client.email = row[3].as<std::string>();
            if (!row[4].is_null()) {
                client.phone_numbers.push_back(row[4].as<std::string>());
            }
        }

        for (auto& [_, client] : clients_map) {
            results.push_back(client);
        }

        return results;
    }

private:
    pqxx::connection& conn;
};

int main() {
    SetConsoleOutputCP(CP_UTF8);

    try {
        pqxx::connection conn(
            "host=localhost port=5432 dbname=TestCpp user=postgres password=123456"
        );

        ClientManager manager(conn);
        manager.createDatabase();
        manager.prepareStatements();

        Client newClient;
        newClient.name = "Ivan";
        newClient.surname = "Ivanov";
        newClient.email = "ivan@example.com";
        newClient.phone_numbers = {"123456", "987654"};
        manager.addClient(newClient);

        auto found = manager.findClients("ivan");
        for (const auto& c : found) {
            std::cout << "ID: " << c.id << " | " << c.name << " " << c.surname
                      << " | " << c.email << "\n";
            for (const auto& phone : c.phone_numbers) {
                std::cout << "   Phone: " << phone << "\n";
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cin.get();

    return 0;
}
