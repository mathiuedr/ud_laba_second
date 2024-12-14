#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this); // Настраиваем UI
    //connect(ui->browseButton,&QPushButton::clicked,this,&MainWindow::on_browseButton_clicked);
    //connect(ui->checkButton,&QPushButton::clicked,this,&MainWindow::on_checkButton_clicked);
    connect(ui->action,&QAction::triggered,this,&MainWindow::on_browseButton_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
    delete db;    // Освобождаем память
}

void MainWindow::incorrectEnter(){
    QMessageBox::information(this,"Ошибка", "Ввод не соответствует формату");
}

void MainWindow::on_browseButton_clicked() {
    // Открываем диалог выбора папки
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выберите папку", QString());

    if (!folderPath.isEmpty()) {
        this->db = new Database(folderPath.toStdString());
    }
}
void MainWindow::on_addButton_clicked(){
    if(this->checkDB() == false) return;
    std::optional<Student> st_opt = matthew::utils::verifyRecord(ui->lineEdit_add->text().toStdString());
    if(st_opt.has_value()){
        bool res = this->db->insertRecord(st_opt.value());
        if(res){
            QMessageBox::information(this,"Успех!","Запись успешно добавлена в базу данных!");
        }else{
            QMessageBox::information(this,"Ошибка","Запись с таким id уже существует");
        }

    }else{
        this->incorrectEnter();
    }
}
void MainWindow::on_searchButton_clicked(){
    if(this->checkDB() == false) return;
    std::string search_res="id name surname isBudget Course\n";
    switch(this->ui->comboBox_search->currentIndex()){
        case 0: {
            std::optional<uint64_t> id = matthew::utils::verifyId(ui->lineEdit_search->text().toStdString());
            if(id.has_value()){
                std::optional<Student> student = this->db->getByPK(id.value());
                if(student.has_value()){
                    search_res+=student.value().toString();
                }
            }else{
                this->incorrectEnter();
            }
            break;
        }
        case 1: {
            std::optional<varchar_32> name = matthew::utils::verifyName(ui->lineEdit_search->text().toStdString());
            if(name.has_value()){
                std::vector<Student> students = this->db->getByName(name.value());
                for(Student student: students){
                    search_res+=student.toString()+"\n";
                }
            }else{
                this->incorrectEnter();
            }
            break;
        }
        case 2: {
            std::optional<varchar_32> surname = matthew::utils::verifySurname(ui->lineEdit_search->text().toStdString());
            if(surname.has_value()){
                std::vector<Student> students = this->db->getBySurname(surname.value());
                for(Student student: students){
                    search_res+=student.toString()+"\n";
                }
            }else{
                this->incorrectEnter();
            }
            break;
        }
        case 3: {
            std::optional<int> isBudget = matthew::utils::verifyIsBudget(ui->lineEdit_search->text().toStdString());
            if(isBudget.has_value()){
                qDebug()<<"GOIDA3";
                std::vector<Student> students = this->db->getByIsBudget(isBudget.value());
                qDebug()<<"GOIDA4";
                for(Student student: students){
                    search_res+=student.toString()+"\n";
                }
            }else{
                this->incorrectEnter();
            }
            break;
        }
        case 4: {
            std::optional<uint32_t> course = matthew::utils::verifyCourse(ui->lineEdit_search->text().toStdString());
            if(course.has_value()){
                std::vector<Student> students = this->db->getByIsBudget(course.value());
                for(Student student: students){
                    search_res+=student.toString()+"\n";
                }
            }else{
                this->incorrectEnter();
            }
            break;
        }
    }
    ui->textBrowser_search->setText(QString::fromStdString(search_res));
}
void MainWindow::on_deleteButton_clicked(){
    if(this->checkDB() == false) return;
    switch(this->ui->comboBox_delete->currentIndex()){
        case 0: {
            std::optional<uint64_t> id = matthew::utils::verifyId(ui->lineEdit_delete->text().toStdString());
            if(id.has_value()){
                this->db->removeByPK(id.value());
            }else{
                this->incorrectEnter();
            }
            break;
        }
        case 1: {
            std::optional<varchar_32> name = matthew::utils::verifyName(ui->lineEdit_delete->text().toStdString());
            if(name.has_value()){
                this->db->removeByName(name.value());
            }else{
                this->incorrectEnter();
            }
            break;
        }
        case 2: {
            std::optional<varchar_32> surname = matthew::utils::verifySurname(ui->lineEdit_delete->text().toStdString());
            if(surname.has_value()){
                this->db->removeBySurname(surname.value());
            }else{
                this->incorrectEnter();
            }
            break;
        }
        case 3: {
            std::optional<int> isBudget = matthew::utils::verifyIsBudget(ui->lineEdit_delete->text().toStdString());
            if(isBudget.has_value()){
                this->db->removeByIsBudget(isBudget.value());
            }else{
                this->incorrectEnter();
            }
            break;
        }
        case 4: {
            std::optional<uint32_t> course = matthew::utils::verifyCourse(ui->lineEdit_delete->text().toStdString());
            if(course.has_value()){
                this->db->removeByCourse(course.value());
            }else{
                this->incorrectEnter();
            }
            break;
        }
    }
}
void MainWindow::on_updateButton_clicked(){
    if(this->checkDB() == false) return;
    std::optional<uint64_t> id = matthew::utils::verifyId(ui->lineEdit_update_id->text().toStdString());
    std::optional<Student> student = matthew::utils::verifyRecord(ui->lineEdit_update_record->text().toStdString());
    if(id.has_value()&& student.has_value()){
        this->db->updateByPK(id.value(),student.value());
    }else{
        this->incorrectEnter();
    }
}

void MainWindow::on_getAllButton_clicked(){
    if(this->checkDB() == false) return;
    std::vector<Student> records= this->db->getAllRecords();
    std::string search_res="id name surname isBudget Course\n";
    for(Student record:records){
        search_res+=record.toString()+"\n";
    }
    ui->textBrowser_search->setText(QString::fromStdString(search_res));
}


void MainWindow::on_loadToXLSXButton_clicked(){
    if(this->checkDB() == false) return;
    if(matthew::utils::writeRecordsToXLSX(ui->textBrowser_search->toPlainText().toStdString())){
        QMessageBox::information(this,"Успех!", "Содержимое поискового экрана записано в файл Saves.xlsx");
    }else{
        QMessageBox::information(this,"Ошибка", "Содержимое поискового экрана не удалось записать в файл");
    }

}


bool MainWindow::checkDB(){
    if(this->db == NULL){
        QMessageBox::information(this,"Ошибка", "База данных не подключена");
        return false;
    }
    return true;
}


