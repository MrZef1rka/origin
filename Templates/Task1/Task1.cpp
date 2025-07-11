#include <string>
#include <iostream>
#include <vector>
#include <map>

class SqlSelectQueryBuilder {
public:
    SqlSelectQueryBuilder() = default;

    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns_.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        table_ = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        where_conditions_[column] = value;
        return *this;
    }

    std::string BuildQuery() const {
        std::string query = "SELECT ";

        if (columns_.empty()) {
            query += "*";
        } else {
            for (size_t i = 0; i < columns_.size(); ++i) {
                if (i != 0) {
                    query += ", ";
                }
                query += columns_[i];
            }
        }

        query += " FROM " + table_;

        if (!where_conditions_.empty()) {
            query += " WHERE ";
            bool is_first_condition = true;
            for (const auto& condition : where_conditions_) {
                if (!is_first_condition) {
                    query += " AND ";
                }
                query += condition.first + "=" + condition.second;
                is_first_condition = false;
            }
        }

        query += ";";
        return query;
    }

private:
    std::vector<std::string> columns_;
    std::string table_;
    std::map<std::string, std::string> where_conditions_;
};

int main()
{
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");

    std::cout << query_builder.BuildQuery();
 
    return 0;
}