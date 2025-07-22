#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "phonebook.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_searchButton_clicked();
    void on_deleteButton_clicked();
    void on_editButton_clicked();
    void on_saveButton_clicked();

    // Menu actions
    void on_actionSave_triggered();
    void on_actionLoad_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();

    void updateContactsTable();

private:
    Ui::MainWindow *ui;
    Phonebook phonebook;
    void clearInputFields();
    void showContactDetails(const Contact& contact);
};
#endif // MAINWINDOW_H
