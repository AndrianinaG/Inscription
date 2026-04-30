#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QWidget>
#include"Etudiant.h"
#include"Inscription.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"Prénom", "Âge"});
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
};

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_inscription_pushButton_clicked()
{
    Etudiant student;
    QString valeur_prenom;
    int valeur_age;
    int newRow;

    valeur_prenom = ui->prenom_lineEdit->text();
    valeur_age = ui->age_lineEdit->text().toInt();
    ui->tableWidget->setRowCount(0);
    student.setPrenom(valeur_prenom);
    student.setAge(valeur_age);
    subscription.ajouter(student);

    for(int i = 0 ; i < subscription.getSize() ; i++)
    {
        newRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(newRow);

        ui->tableWidget->setItem(newRow, 0, new QTableWidgetItem(subscription.liste[i].getPrenom()));
        ui->tableWidget->setItem(newRow, 1, new QTableWidgetItem(QString::number(subscription.liste[i].getAge())));

        ui->prenom_lineEdit->clear();
        ui->age_lineEdit->clear();
    }
}

void MainWindow::on_rechercher_button_clicked()
{
    Etudiant student;
    QString valeur_prenom;
    int i, newRow;

    valeur_prenom = ui->recherche_lineEdit->text();
    ui->tableWidget->setRowCount(0);
    for (i = 0; i < subscription.getSize(); i++)
    {
        if(valeur_prenom == subscription.liste[i].getPrenom())
        {
            newRow = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(newRow);
            ui->tableWidget->setItem(newRow, 0, new QTableWidgetItem(subscription.liste[i].getPrenom()));
            ui->tableWidget->setItem(newRow, 1, new QTableWidgetItem(QString::number(subscription.liste[i].getAge())));
        }
    }
}


void MainWindow::on_actionquitter_triggered()
{
    QApplication::quit();
}

