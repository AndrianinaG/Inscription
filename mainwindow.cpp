#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QWidget>
#include "Etudiant.h"
#include "Inscription.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Table
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"Prénom", "Âge"});
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    this->setWindowTitle("Inscription universitaire");

    connect(ui->tableWidget, &QTableWidget::customContextMenuRequested, this, &MainWindow::afficherMenu);

    // État initial des boutons undo/redo
    updateUndoRedoState();
}

MainWindow::~MainWindow()
{
    // Libérer les commandes restantes dans les piles
    qDeleteAll(m_undoStack);
    qDeleteAll(m_redoStack);

    delete ui;
}

// Gestion des commandes (cœur du système undo/redo)

void MainWindow::pushCommand(Command *cmd)
{
    cmd->redo();                    // exécuter l'action
    m_undoStack.push(cmd);

    // Supprimer et libérer les commandes "futures" devenues obsolètes
    qDeleteAll(m_redoStack);
    m_redoStack.clear();

    updateUndoRedoState();
}

/** Active/désactive les boutons undo et redo selon l'état des piles. */
void MainWindow::updateUndoRedoState()
{
    // Les actions doivent exister dans votre toolbar (objectName: actionUndo / actionRedo)
    ui->actionundo->setEnabled(!m_undoStack.isEmpty());
    ui->actionredo->setEnabled(!m_redoStack.isEmpty());
}

// Inscription (ajout d'un étudiant)
void MainWindow::on_inscription_pushButton_clicked()
{
    QString prenom = ui->prenom_lineEdit->text().trimmed();
    int age = ui->age_lineEdit->text().toInt();

    if (prenom.isEmpty())
    {
        QMessageBox::warning(this, "Champ vide", "Veuillez saisir un prénom.");
        return;
    }

    Etudiant student;
    student.setPrenom(prenom);
    student.setAge(age);

    // Commande encapsulant l'ajout (undo/redo pris en charge)
    pushCommand(new AjouterCommand(subscription, ui->tableWidget, student));

    ui->prenom_lineEdit->clear();
    ui->age_lineEdit->clear();
}

// Recherche
void MainWindow::on_rechercher_button_clicked()
{
    QString prenom = ui->recherche_lineEdit->text().trimmed();

    ui->tableWidget->setRowCount(0);

    for (int i = 0; i < subscription.getSize(); i++)
    {
        if (prenom == subscription.liste[i].getPrenom())
        {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(subscription.liste[i].getPrenom()));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(subscription.liste[i].getAge())));
        }
    }
}

// Menu contextuel (clic droit sur le tableau)
void MainWindow::afficherMenu(const QPoint &pos)
{
    QTableWidgetItem *item = ui->tableWidget->itemAt(pos);

    if (!item)
    {
        return;
    }

    int row = item->row();

    QMenu menu(this);
    QAction *actionModifier = menu.addAction("Modifier");
    QAction *actionSupprimer = menu.addAction("Supprimer");

    QAction *choix = menu.exec(ui->tableWidget->viewport()->mapToGlobal(pos));

    if (choix == actionModifier)
    {
        modifierEtudiant(row);
    }
    else if (choix == actionSupprimer)
    {
        supprimerEtudiant(row);
    }
}

// Supprimer un étudiant
void MainWindow::supprimerEtudiant(int row)
{
    // Sauvegarder l'étudiant AVANT suppression pour pouvoir l'annuler
    Etudiant sauvegarde = subscription.liste[row];

    pushCommand(new SupprimerCommand(subscription, ui->tableWidget, row, sauvegarde));
}

// Modifier un étudiant
void MainWindow::modifierEtudiant(int row)
{
    Etudiant ancien = subscription.liste[row];

    bool ok;

    // Demander le nouveau prénom
    QString nouveauPrenom = QInputDialog::getText(
        this,
        "Modifier le prénom",
        "Nouveau prénom :",
        QLineEdit::Normal,
        ancien.getPrenom(),
        &ok
        );

    if (!ok)
    {
        // annulé → aucune commande créée
        return;
    }

    if (nouveauPrenom.isEmpty())
    {
        nouveauPrenom = ancien.getPrenom();
    }

    // Demander le nouvel âge
    int nouvelAge = QInputDialog::getInt(
        this,
        "Modifier l'âge",
        "Nouvel âge :",
        ancien.getAge(),
        0,
        120,
        1,
        &ok
        );

    if (!ok)
    {
        return;   // annulé → aucune commande créée
    }

    // Ne créer une commande que si quelque chose a réellement changé
    if (nouveauPrenom == ancien.getPrenom() && nouvelAge == ancien.getAge())
    {
        return;
    }

    Etudiant nouveau;
    nouveau.setPrenom(nouveauPrenom);
    nouveau.setAge(nouvelAge);

    pushCommand(new ModifierCommand(subscription, ui->tableWidget, row, ancien, nouveau));
}

// Sauvegarde (code factorisé dans une méthode privée)
bool MainWindow::sauvegarderFichier()
{
    QString chemin = QFileDialog::getSaveFileName(this);

    if (chemin.isEmpty())
    {
        return false;
    }

    QFile fichier(chemin);

    if (!fichier.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier en écriture.");
        return false;
    }

    QTextStream flux(&fichier);
    flux << "Prénom,Âge\n";

    for (int i = 0; i < subscription.getSize(); ++i)
    {
        Etudiant e = subscription.liste[i];
        flux << e.getPrenom() << "," << e.getAge() << "\n";
    }

    fichier.close();
    QMessageBox::information(this, "Succès", "La liste a été sauvegardée avec succès.");

    return true;
}

void MainWindow::on_sauvegarder_button_clicked()
{
    sauvegarderFichier();
}

void MainWindow::on_actionsauvegader_fichier_triggered()
{
    sauvegarderFichier();
}

// Ouvrir un fichier CSV
void MainWindow::on_actionouvrir_fichier_triggered()
{
    QString chemin = QFileDialog::getOpenFileName(this);

    if (chemin.isEmpty())
    {
        return;
    }

    QFile fichier(chemin);

    if (!fichier.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier en lecture.");
        return;
    }

    // Nettoyer les anciennes données ET l'historique undo/redo
    subscription.liste.clear();
    ui->tableWidget->setRowCount(0);

    qDeleteAll(m_undoStack);
    m_undoStack.clear();

    qDeleteAll(m_redoStack);
    m_redoStack.clear();

    updateUndoRedoState();

    QTextStream flux(&fichier);
    bool premiereLigne = true;

    while (!flux.atEnd())
    {
        QString ligne = flux.readLine().trimmed();

        if (ligne.isEmpty())
        {
            continue;
        }

        if (premiereLigne)
        {
            // ignorer l'en-tête
            premiereLigne = false;
            continue;
        }

        QStringList champs = ligne.split(',');

        if (champs.size() < 2)
        {
            continue;
        }

        QString prenom = champs[0].trimmed();
        int age = champs[1].trimmed().toInt();

        Etudiant e;
        e.setPrenom(prenom);
        e.setAge(age);

        subscription.ajouter(e);

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(prenom));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(age)));
    }

    fichier.close();
    QMessageBox::information(this, "Succès", "Fichier chargé avec succès.");
}

// Fermeture de la fenêtre
void MainWindow::closeEvent(QCloseEvent *event)
{
    auto reply = QMessageBox::question(
        this,
        "Confirmation",
        "Voulez-vous enregistrer avant de quitter ?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
        );

    if (reply == QMessageBox::Yes)
    {
        if (sauvegarderFichier())
        {
            QApplication::quit();
        }
        // Si l'utilisateur annule la boîte de dialogue de fichier → rester ouvert
    }
    else if (reply == QMessageBox::No)
    {
        QApplication::quit();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::on_actionundo_triggered()
{
    if (m_undoStack.isEmpty())
    {
        return;
    }

    Command *cmd = m_undoStack.pop();
    cmd->undo();
    m_redoStack.push(cmd);

    updateUndoRedoState();
}

void MainWindow::on_actionredo_triggered()
{
    if (m_redoStack.isEmpty())
    {
        return;
    }

    Command *cmd = m_redoStack.pop();
    cmd->redo();
    m_undoStack.push(cmd);

    updateUndoRedoState();
}
