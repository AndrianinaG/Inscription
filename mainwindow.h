#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QWidget>
#include <QTableWidget>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QCloseEvent>
#include <QInputDialog>
#include <QStack>
#include "Inscription.h"
#include "Command.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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
    void on_sauvegarder_button_clicked();
    void on_actionouvrir_fichier_triggered();
    void on_actionsauvegader_fichier_triggered();
    void closeEvent(QCloseEvent *event) override;
    void afficherMenu(const QPoint &pos);
    void on_actionundo_triggered();
    void on_actionredo_triggered();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    Inscription subscription;

    //Piles Undo / Redo
    QStack<Command *> m_undoStack;
    QStack<Command *> m_redoStack;

    // Exécute la commande, la pousse sur undoStack, vide redoStack
    void pushCommand(Command *cmd);
    // Met à jour l'état activé/désactivé des boutons toolbar
    void updateUndoRedoState();

    void supprimerEtudiant(int row);
    void modifierEtudiant(int row);

    // Aide à la sauvegarde (code partagé)
    bool sauvegarderFichier();
};

#endif // MAINWINDOW_H
