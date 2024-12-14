#include "utils.h"
#include <QDebug>
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxchartsheet.h"
#include "xlsxdocument.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

bool matthew::utils::is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}
std::optional<uint64_t> matthew::utils::verifyId(std::string id){
    if(matthew::utils::is_number(id) && std::stoll(id)>0){
        return {std::stoull(id)};
    }
    return std::nullopt;
}

std::optional<varchar_32> matthew::utils::verifyName(std::string name){
    if(name.length() <=32 && !matthew::utils::strContainsChar(name,' ') && !matthew::utils::strContainsChar(name,'\n')){
        return {varchar_32{name}};
    }
    return std::nullopt;
}
std::optional<varchar_32> matthew::utils::verifySurname(std::string surname){
    if(surname.length() <=32 && !matthew::utils::strContainsChar(surname,' ')&& !matthew::utils::strContainsChar(surname,'\n')){
        return {varchar_32{surname}};
    }
    return std::nullopt;
}
std::optional<int> matthew::utils::verifyIsBudget(std::string isbudget){
    if(isbudget=="true"){
        return{2};
    }else if(isbudget=="false"){
        return{1};
    }else{
        return std::nullopt;
    }
}
std::optional<uint32_t> matthew::utils::verifyCourse(std::string course){
    if(matthew::utils::is_number(course) && std::stoi(course)>0 && std::stoi(course)<6){
        return{std::stoul(course)};
    }
    return std::nullopt;
}
std::optional<Student> matthew::utils::verifyRecord(std::string str){
    std::vector<std::string> v;
    int start, end;
    start = end = 0;
    char dl = ' ';
    while ((start = str.find_first_not_of(dl, end))
           != std::string::npos) {
        end = str.find(dl, start);
        std::string sbs = str.substr(start, end - start);
        v.push_back(sbs);
    }
    if(v.size()==5 ){
        std::optional<uint64_t> id= verifyId(v[0]);
        std::optional<varchar_32> name= verifyName(v[1]);
        std::optional<varchar_32> surname= verifySurname(v[2]);
        std::optional<int> isbudget= verifyIsBudget(v[3]);
        std::optional<uint32_t> course= verifyCourse(v[4]);
        if(id.has_value()&&name.has_value()&&surname.has_value()&&isbudget.has_value()&&course.has_value()){
            return {Student{id.value(),name.value(),surname.value(),isbudget.value(),course.value()}};
        }
    }
    return std::nullopt;

}

bool matthew::utils::strContainsChar(std::string str,char symbol){
    return str.find(symbol) != std::string::npos;
}

bool matthew::utils::writeRecordsToXLSX(std::string records_string){
    std::vector<std::string> v;
    int start, end;
    start = end = 0;
    char dl = '\n';
    while ((start = records_string.find_first_not_of(dl, end))
           != std::string::npos) {
        end = records_string.find(dl, start);
        std::string sbs = records_string.substr(start, end - start);
        v.push_back(sbs);
    }
    std::vector<std::vector<std::string>> res;
    for(std::string elem:v){
        std::vector<std::string> prom_res;
        start = end = 0;
        dl = ' ';
        while ((start = elem.find_first_not_of(dl, end))
               != std::string::npos) {
            end = elem.find(dl, start);
            std::string sbs = elem.substr(start, end - start);
            prom_res.push_back(sbs);
        }
        res.push_back(prom_res);
    }
    QXlsx::Document xlsxW;
    for(int i=0;i<res.size();i++){
        for(int j=0;j<res[i].size();j++){
            xlsxW.write(i+1, j+1, QString::fromStdString(res[i][j]));
        }
    }
    if(xlsxW.saveAs("Saves.xlsx")){
        return true;
    }
    return false;

}

