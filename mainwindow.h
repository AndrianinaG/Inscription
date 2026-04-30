#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QWidget>
#include <QTableWidget>
#include"Inscription.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_inscription_pushButton_clicked();
    void on_rechercher_button_clicked();

    void on_actionquitter_triggered();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    Inscription subscription;
};
#endif // MAINWINDOW_H
