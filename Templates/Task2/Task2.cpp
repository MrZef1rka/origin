#include <iostream>
#include <string>
#include <vector>
#include <map>

class SqlSelectQueryBuilder {
public:
    SqlSelectQueryBuilder() = default;
    
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns_.push_back(column);
        return *this;
    }
    
    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept {
        columns_.insert(columns_.end(), columns.begin(), columns.end());
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
    
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& entry : kv) {
            where_conditions_[entry.first] = entry.second;
        }
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
    
        query_builder.AddColumn("id");
        query_builder.AddColumns({"name", "email"}); 
        query_builder.AddFrom("users");
        query_builder.AddWhere("age", "25");
        query_builder.AddWhere({{"status", "active"}, {"role", "admin"}});
    
        std::cout << query_builder.BuildQuery() << std::endl;
    
        return 0;
    }