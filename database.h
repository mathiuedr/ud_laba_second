#pragma once
#include "src/utec/disk/btree.h"
#include "src/utec/disk/pagemanager.h"
#include <cstring>
#include <string>
#include <vector>
#include <optional>
#include <QDebug>
template <typename T>
struct Pair {
    T id;
    unsigned long long page_id;

    bool operator<(const Pair& p) const {
        if (this->id < p.id) return true;
        else return false;
    }

    bool operator<=(const Pair& p) const {
        if (this->id <= p.id) return true;
        else return false;
    }

    bool operator==(const Pair& p) const {
        if (this->id == p.id) return true;
        else return false;
    }
};
struct varchar_32 {
    char content[32];
    bool operator<(const varchar_32& p) const {
        if (strcmp(this->content, p.content) < 0) return true;
        else return false;
    }

    bool operator<=(const varchar_32& p) const {
        if (strcmp(this->content, p.content) <= 0) return true;
        else return false;
    }

    bool operator==(const varchar_32& p) const {
        if (strcmp(this->content, p.content) == 0) return true;
        else return false;
    }
    varchar_32(std::string v){
        strcpy(this->content, v.c_str());
    }
    varchar_32(){this->content[0]=0;}
    varchar_32(int64_t n){this->content[0]=n;}
};



template <typename T>
std::ostream& operator<<(std::ostream& out, Pair<T>& p) {
    out << p.id;
    return out;
}


struct Student {
    unsigned long long id;
    varchar_32 name;
    varchar_32 surname;
    int isBudget;
    unsigned int course;
    std::string toString() {
        std::string isBudget_str = (isBudget == 2) ? "True" : "False";
        std::string qwe = std::to_string(id) +" "+ std::string(name.content) + " " + std::string(surname.content) + " " + isBudget_str + " " + std::to_string(course);
        return qwe;
    }
};



using namespace utec::disk;


class Database {

    pagemanager* record_manager=NULL;
    pagemanager* settings_manager=NULL;
    btree<Pair<varchar_32>> btree_name;
    btree<Pair<varchar_32>> btree_surname;
    btree<Pair<int>> btree_isbudget;
    btree<Pair<unsigned int>> btree_course;
    uint64_t maxid;
    template <typename T>
    std::vector<Pair<T>> findAll(btree<Pair<T>>& bt, T id) {
        auto iter = bt.find(Pair<T>{id, 0});
        std::vector<Pair<T>> v;
        while (iter != bt.end())
        {
            auto pair = *iter;
            v.push_back(pair);
            bt.remove(Pair<T>{pair.id, 0});
            iter = bt.find(Pair<T>{id, 0});
        }
        for (Pair<T> p : v) {
            bt.insert(p);
        }
        return v;
    }
    template <typename T>
    void deleteFromIndex(btree<Pair<T>>& bt, Pair<T> pair_to_delete) {
        auto iter = bt.find(Pair<T>{pair_to_delete.id, 0});
        std::vector<Pair<T>> v;
        while (iter != bt.end())
        {
            auto pair = *iter;
            bt.remove(Pair<T>{pair.id, 0});
            if (pair.id == pair_to_delete.id && pair.page_id == pair_to_delete.page_id) break;
            v.push_back(pair);
            iter = bt.find(Pair<T>{pair_to_delete.id, 0});
        }
        for (Pair<T> p : v) {
            bt.insert(p);
        }
    }
    template <typename T>
    std::vector<Student> getRecordsByIds(std::vector<Pair<T>> ids) {
        std::vector<Student> result;
        for (Pair<T> id_record : ids) {
            std::optional<Student> record = this->getByPK(id_record.page_id);
            if (record.has_value()) {
                result.push_back(record.value());
            }
        }
        return result;
    }
public:
    Database() {
        this->record_manager = new pagemanager("database.bin");
        this->settings_manager = new pagemanager("settings.bin");
        std::shared_ptr<pagemanager> pm_name = std::make_shared<pagemanager>("name.index", false);
        this->btree_name = btree<Pair<varchar_32>>(pm_name);
        std::shared_ptr<pagemanager> pm_surname = std::make_shared<pagemanager>("surname.index", false);
        this->btree_surname = btree<Pair<varchar_32>>(pm_surname);
        std::shared_ptr<pagemanager> pm_isbudget = std::make_shared<pagemanager>("isbudget.index", false);
        this->btree_isbudget = btree<Pair<int>>(pm_isbudget);
        std::shared_ptr<pagemanager> pm_course = std::make_shared<pagemanager>("course.index", false);
        this->btree_course = btree<Pair<unsigned int>>(pm_course);
        uint64_t mx;
        bool res = this->settings_manager->recover(1,mx);
        if(res){
            this->maxid = mx;
        }else{
            this->maxid=1;
        }

    }
    Database(std::string path) {
        this->record_manager = new pagemanager(path+"/database.bin");
        this->settings_manager = new pagemanager(path+"/settings.bin");
        std::shared_ptr<pagemanager> pm_name = std::make_shared<pagemanager>(path+"/name.index", false);
        this->btree_name = btree<Pair<varchar_32>>(pm_name);
        std::shared_ptr<pagemanager> pm_surname = std::make_shared<pagemanager>(path+"/surname.index", false);
        this->btree_surname = btree<Pair<varchar_32>>(pm_surname);
        std::shared_ptr<pagemanager> pm_isbudget = std::make_shared<pagemanager>(path+"/isbudget.index", false);
        this->btree_isbudget = btree<Pair<int>>(pm_isbudget);
        std::shared_ptr<pagemanager> pm_course = std::make_shared<pagemanager>(path+"/course.index", false);
        this->btree_course = btree<Pair<unsigned int>>(pm_course);
        uint64_t mx;
        bool res = this->settings_manager->recover(1,mx);
        if(res){
            this->maxid = mx;
        }else{
            this->maxid=1;
        }

    }
    ~Database(){
        delete record_manager;
        delete settings_manager;
    }
    std::optional<Student> getByPK(uint64_t id) {
        Student record;
        bool res = this->record_manager->recover(id, record);
        if (res == false || (res == true && record.id == 0)) {
            return std::nullopt;
        }
        return std::make_optional<Student>(record);

    }
    std::vector<Student> getByName(varchar_32 name) {
        std::vector<Pair<varchar_32>> ids = this->findAll<varchar_32>(this->btree_name, name);
        return this->getRecordsByIds<varchar_32>(ids);
    }
    std::vector<Student> getBySurname(varchar_32 surname) {
        std::vector<Pair<varchar_32>> ids = this->findAll<varchar_32>(this->btree_surname, surname);
        return this->getRecordsByIds<varchar_32>(ids);
    }
    std::vector<Student> getByIsBudget(int isbudget) {
        std::vector<Pair<int>> ids = this->findAll<int>(this->btree_isbudget, isbudget);
        return this->getRecordsByIds<int>(ids);
    }
    std::vector<Student> getByCourse(uint32_t course) {
        std::vector<Pair<uint32_t>> ids = this->findAll<uint32_t>(this->btree_course, course);
        return this->getRecordsByIds<uint32_t>(ids);
    }
    bool insertRecord(Student record) {
        if (!(this->getByPK(record.id).has_value())) {
            this->btree_name.insert(Pair<varchar_32>{ record.name, record.id });
            this->btree_surname.insert(Pair<varchar_32>{ record.surname, record.id });
            this->btree_isbudget.insert(Pair<int>{ record.isBudget, record.id });
            this->btree_course.insert(Pair<uint32_t>{ record.course, record.id });
            this->record_manager->save<Student>(record.id, record);
            if(record.id > this->maxid){
                this->maxid = record.id;
                this->settings_manager->erase<uint64_t>(1);
                this->settings_manager->save<uint64_t>(1,this->maxid);
            }
            return true;
        }
        else {
            return false;
        }
    }
    void removeByPK(uint64_t id) {
        //FIND A RECORD AND DELETE FROM ALL BTREES TOO
        std::optional<Student> record_opt = this->getByPK(id);
        if (!(record_opt.has_value())) return;
        this->deleteFromIndex<varchar_32>(this->btree_name, Pair<varchar_32>{record_opt.value().name, id});
        this->deleteFromIndex<varchar_32>(this->btree_surname, Pair<varchar_32>{record_opt.value().surname, id});
        this->deleteFromIndex<int>(this->btree_isbudget, Pair<int>{record_opt.value().isBudget, id});
        this->deleteFromIndex<uint32_t>(this->btree_course, Pair<uint32_t>{record_opt.value().course, id});
        this->record_manager->erase<Student>(id);
    }
    void removeByName(varchar_32 name) {
        std::vector<Pair<varchar_32>> ids = this->findAll<varchar_32>(this->btree_name, name);
        for (Pair<varchar_32> pair : ids) {
            this->removeByPK(pair.page_id);
        }
    }
    void removeBySurname(varchar_32 surname) {
        std::vector<Pair<varchar_32>> ids = this->findAll<varchar_32>(this->btree_surname, surname);
        for (Pair<varchar_32> pair : ids) {
            this->removeByPK(pair.page_id);
        }
    }
    void removeByIsBudget(int isbudget) {
        std::vector<Pair<int>> ids = this->findAll<int>(this->btree_isbudget, isbudget);
        for (Pair<int> pair : ids) {
            this->removeByPK(pair.page_id);
        }
    }
    void removeByCourse(uint32_t course) {
        std::vector<Pair<uint32_t>> ids = this->findAll<uint32_t>(this->btree_course, course);
        for (Pair<uint32_t> pair : ids) {
            this->removeByPK(pair.page_id);
        }
    }
    void updateByPK(uint64_t id, Student new_record) {
        this->removeByPK(id);
        this->insertRecord(new_record);
    }
    std::vector<Student> getAllRecords() {
        std::vector<Student> res;
        int border = this->maxid;
        for (int i = 1; i <= border; i++) {
            std::optional<Student> record_opt = this->getByPK(i);
            if(record_opt.has_value()){
                res.push_back(record_opt.value());
            }
        }
        return res;
    }
};
