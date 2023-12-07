//
// Created by 邦邦 on 2022/4/18.
//
#include "mysqlorm/sql/dml_type.h"

namespace bb {
    dml_type *dml_type::tinyint_(const std::string &key) {
        sql_arr_.push_back({key, "TINYINT", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::tinyint_(const std::string &key, const int &size) {
        sql_arr_.push_back({key, "TINYINT", std::to_string(size), "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::smallint_(const std::string &key) {
        sql_arr_.push_back({key, "SMALLINT", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::smallint_(const std::string &key, const int &size) {
        sql_arr_.push_back({key, "SMALLINT", std::to_string(size), "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::mediumint_(const std::string &key) {
        sql_arr_.push_back({key, "MEDIUMINT", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::mediumint_(const std::string &key, const int &size) {
        sql_arr_.push_back({key, "MEDIUMINT", std::to_string(size), "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::int_(const std::string &key) {
        sql_arr_.push_back({key, "INT", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::int_(const std::string &key, const int &size) {
        sql_arr_.push_back({key, "INT", std::to_string(size), "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::bigint_(const std::string &key) {
        sql_arr_.push_back({key, "BIGINT", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::bigint_(const std::string &key, const int &size) {
        sql_arr_.push_back({key, "BIGINT", std::to_string(size), "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::float_(const std::string &key) {
        sql_arr_.push_back({key, "FLOAT", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::float_(const std::string &key, const int &size) {
        sql_arr_.push_back({key, "FLOAT", std::to_string(size > 255 ? 255 : size), "0", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::float_(const std::string &key, const int &size, const int &precision) {
        sql_arr_.push_back({key, "FLOAT", std::to_string(size > 255 ? 255 : size), std::to_string(precision > 30 ? 30 : precision),"NOT NULL"});
        return this;
    }

    dml_type *dml_type::double_(const std::string &key) {
        sql_arr_.push_back({key, "DOUBLE", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::double_(const std::string &key, const int &size) {
        sql_arr_.push_back({key, "DOUBLE", std::to_string(size > 255 ? 255 : size), "0", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::double_(const std::string &key, const int &size, const int &precision) {
        sql_arr_.push_back({key, "DOUBLE", std::to_string(size > 255 ? 255 : size),std::to_string(precision > 30 ? 30 : precision), "NOT NULL"});
        return this;
    }

    dml_type *dml_type::decimal_(const std::string &key) {
        sql_arr_.push_back({key, "DECIMAL", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::decimal_(const std::string &key, const int &size) {
        sql_arr_.push_back({key, "DECIMAL", std::to_string(size > 65 ? 65 : size), "0", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::decimal_(const std::string &key, const int &size, const int &precision) {
        sql_arr_.push_back({key, "DECIMAL", std::to_string(size > 65 ? 65 : size), std::to_string(precision > 30 ? 30 : precision),"NOT NULL"});
        return this;
    }

    dml_type *dml_type::string_(const std::string &key) {
        sql_arr_.push_back({key, "VARCHAR", "255", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::string_(const std::string &key, const int &size) {
        std::string size_s = std::to_string(size);
        if (size > 16371) {
            size_s = "16371";
            bb::secure::Log::obj().warn(key + " max 16371");
        }
        sql_arr_.push_back({key, "VARCHAR", size_s, "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::text_(const std::string &key) {
        sql_arr_.push_back({key, "TEXT", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::date_(const std::string &key) {
        sql_arr_.push_back({key, "DATE", "", "", "NOT NULL"});
        return this;
    }
    dml_type *dml_type::time_(const std::string &key) {
        sql_arr_.push_back({key, "TIME", "", "", "NOT NULL"});
        return this;
    }
    dml_type *dml_type::year_(const std::string &key) {
        sql_arr_.push_back({key, "YEAR", "", "", "NOT NULL"});
        return this;
    }
    dml_type *dml_type::datetime_(const std::string &key) {
        sql_arr_.push_back({key, "DATETIME", "", "", "NOT NULL"});
        return this;
    }
    dml_type *dml_type::timestamp_(const std::string &key) {
        sql_arr_.push_back({key, "TIMESTAMP", "", "", "NOT NULL"});
        return this;
    }

    dml_type *dml_type::dateAt_() {
        sql_arr_.push_back({"created_at", "TIMESTAMP", "", "",
                            "DEFAULT CURRENT_TIMESTAMP COMMENT '创建时间'"}); //在创建新记录的时候把这个字段设置为当前时间，但以后修改时，不再刷新它
        sql_arr_.push_back({"updated_at", "TIMESTAMP", "", "",
                            "ON UPDATE CURRENT_TIMESTAMP COMMENT '更新时间'"}); //在创建新记录的时候把这个字段设置为0，以后修改时刷新它
        return this;
    }

    dml_type *dml_type::unsigned_() {
        std::string type = sql_arr_.back()[1];
        if (type == "TINYINT" || type == "SMALLINT" || type == "MEDIUMINT" || type == "INT" || type == "BIGINT" ||
            type == "FLOAT" || type == "DOUBLE" || type == "DECIMAL") {
            sql_arr_.back()[4] = "UNSIGNED " + sql_arr_.back()[4];
        } else {
            bb::secure::Log::obj().warn(type + " 不支持 UNSIGNED");
        }
        return this;
    }

    dml_type *dml_type::unique_() {
        std::string type = sql_arr_.back()[1];
        if (type != "TEXT" && type != "BLOB") {
            if (type == "VARCHAR") {
                if (!sql_arr_.back()[2].empty()) {
                    if (std::stoi(sql_arr_.back()[2]) > 1024) {
                        sql_arr_.back()[2] = "1024";
                        perror("VARCHAR UNIQUE max 1024");
                    }
                }
            }
            sql_arr_.back()[4] += " UNIQUE";
        } else {
            bb::secure::Log::obj().warn(type + " 不支持 UNIQUE");
        }
        return this;
    }

    dml_type *dml_type::nullable_() {
        std::string attach = sql_arr_.back()[4];
        sql_arr_.back()[4] = attach.replace(attach.find("NOT NULL"), 8, "NULL");
        return this;
    }

    dml_type *dml_type::default_(const std::string &value) {
        std::string type = sql_arr_.back()[1];
        if (type != "DATETIME" && type != "TIMESTAMP" && type != "TEXT") {
            sql_arr_.back()[4] += " DEFAULT '" + value + '\'';
        } else {
            bb::secure::Log::obj().warn(type + " 不允许 DEFAULT");
        }
        return this;
    }

    dml_type *dml_type::default_(const int &value) {
        std::string type = sql_arr_.back()[1];
        if (type != "DATETIME" && type != "TIMESTAMP" && type != "TEXT") {
            sql_arr_.back()[4] += " DEFAULT " + std::to_string(value);
        } else {
            bb::secure::Log::obj().warn(type + " 不允许 DEFAULT");
        }
        return this;
    }

    dml_type *dml_type::comment_(const std::string &value) {
        sql_arr_.back()[4] += " COMMENT '" + value + '\'';
        return this;
    }

    dml_type *dml_type::zerofill_() {
        std::string type = sql_arr_.back()[1];
        if (type == "TINYINT" || type == "SMALLINT" || type == "MEDIUMINT" || type == "INT" || type == "BIGINT" ||
            type == "FLOAT" || type == "DOUBLE" || type == "DECIMAL") {
            sql_arr_.back()[4] = "ZEROFILL " + sql_arr_.back()[4];
        } else {
            bb::secure::Log::obj().warn(type + " 不支持 ZEROFILL");
        }
        return this;
    }

    dml_type *dml_type::index_(const std::vector<std::string> &key_list, const short &is_unique) {
        std::string sql;
        for (auto &item: key_list) {
            sql += '`' + item + "`,";
        }
        sql.pop_back();
        switch (is_unique) {
            case 0:
                sql = "INDEX(" + sql + ")"; //普通索引
                break;
            case 1:
                sql = "UNIQUE INDEX(" + sql + ")"; //唯一索引
                break;
            case 2:
                sql = "FULLTEXT (" + sql + ")"; //全文索引
                /*if(sql_arr_.back()[1].find("CHAR",0)!=std::string::npos||sql_arr_.back()[1].find("VARCHAR",0)!=std::string::npos||sql_arr_.back()[1].find("TEXT",0)!=std::string::npos){
                    sql="FULLTEXT ("+sql+")"; //全文索引
                }else{
                    throw bb::error("db:index_() 全文索引只支持 char、varchar和text");
                }*/
                break;
            case 3:
                sql = "PRIMARY KEY (" + sql + ")"; //主键索引
                break;
            default:
                break;
        }
        sql_other_.push_back(sql);
        return this;
    }

    dml_type *dml_type::index_() {
        //普通索引 CREATE TABLE t(c1 INT PRIMARY KEY,c2 INT NOT NULL,c3 INT NOT NULL,c4 VARCHAR(10),INDEX (c2,c3),UNIQUE INDEX(c4));
        sql_other_.push_back("INDEX(`" + sql_arr_.back()[0] + "`)");
        return this;
    }
    dml_type *dml_type::indexPrimaryKey_(){
        sql_other_.push_back("PRIMARY KEY (`" + sql_arr_.back()[0] + "`)");
        return this;
    }
    dml_type *dml_type::indexFulltext_(){
        /*if(sql_arr_.back()[1].find("CHAR",0)!=std::string::npos||sql_arr_.back()[1].find("VARCHAR",0)!=std::string::npos||sql_arr_.back()[1].find("TEXT",0)!=std::string::npos){
            sql_other_.push_back("FULLTEXT (`"+sql_arr_.back()[0]+"`)");
        }else{
            throw bb::error("db:index_() 全文索引只支持 char、varchar和text");
        }*/
        sql_other_.push_back("FULLTEXT (`" + sql_arr_.back()[0] + "`)");
        return this;
    }
    dml_type *dml_type::indexUnique_(){
        sql_other_.push_back("UNIQUE INDEX(`" + sql_arr_.back()[0] + "`)");
        return this;
    }
    
    dml_type *dml_type::NoSQL_() {
        sql_arr_.back()[5] = "1";
        return this;
    }
}