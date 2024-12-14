#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <optional>
#include "database.h"
#include "utils.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browseButton_clicked();
    void on_addButton_clicked(); // Слот для обработки нажатия кнопки
    void on_searchButton_clicked();
    void on_deleteButton_clicked();
    void on_updateButton_clicked();
    void on_getAllButton_clicked();
    void on_loadToXLSXButton_clicked();
private:
    void incorrectEnter();
    bool checkDB();
    Database* db=NULL;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
