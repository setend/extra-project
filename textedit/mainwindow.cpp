#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isUntitled = true;
    curFile= tr("untitled.text");

    setWindowTitle(curFile);

    finddlg = new QDialog(this);
    finddlg->setWindowTitle(tr("find"));
    findLineEdit = new QLineEdit(finddlg);
    QPushButton *btn = new QPushButton(tr("find the next"),finddlg);
    QVBoxLayout *layout = new QVBoxLayout(finddlg);

    layout->addWidget(findLineEdit);
    layout->addWidget(btn);
    connect(btn,&QPushButton::clicked,this,&MainWindow::showFindText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    if(maybeSave())
    {
        isUntitled = true;
        curFile = ("untitled.txt");
        setWindowTitle(curFile);
        ui->textEdit->clear();
        ui->textEdit->setVisible(true);

    }
}

void MainWindow::open()
{
    if(maybeSave()){
        QString filename = QFileDialog::getOpenFileName(this);
        if(!filename.isEmpty()){
            loadFile(filename);
            ui->textEdit->setVisible(true);
        }
    }
}

bool MainWindow::maybeSave(){
    if(ui->textEdit->document()->isModified()){
        QMessageBox box;
        box.setWindowTitle(tr("warning"));
        box.setIcon(QMessageBox::Warning);
        box.setText(curFile+tr(" not been saved"));
        QPushButton *yesbtn = box.addButton(tr("yes"),QMessageBox::YesRole);
        box.addButton(tr("no"),QMessageBox::NoRole);
        QPushButton *cancelbtn = box.addButton(tr("cancel"),QMessageBox::RejectRole);

        box.exec();
        if(box.clickedButton() == yesbtn)
            return save();
        else if(box.clickedButton() == cancelbtn)
            return false;
    }
    return true;
}

bool MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("save as"),curFile);
    if(filename.isEmpty()) return false;
    return saveFile(filename);
}

bool MainWindow::save(){
    if(isUntitled)
        return saveAs();
    return saveFile(curFile);
}

bool MainWindow::saveFile(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){

        QMessageBox::warning(this,tr("多文档编辑器"),tr("无法写入文件 %1 :/n %2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<ui->textEdit->toPlainText();

    QApplication::restoreOverrideCursor();
    isUntitled = false;
    curFile = QFileInfo(filename).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

bool MainWindow::loadFile(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,tr("多文档编辑器"),tr("无法读取文件 %1 \n %2").arg(filename).arg(file.errorString()));
        return false;
    }
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    ui->textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    curFile = QFileInfo(filename).canonicalFilePath();
    setWindowTitle(curFile);
    return true;
}

void MainWindow::on_actionxi_triggered()
{
    newFile();
}

void MainWindow::on_actionopen_O_triggered()
{
    open();
}

void MainWindow::on_actionclose_c_triggered()
{
    if(maybeSave())
        ui->textEdit->setVisible(false);
}

void MainWindow::on_actionstore_S_triggered()
{
    save();
}

void MainWindow::on_actionanother_store_A_triggered()
{
    saveAs();
}

void MainWindow::on_actionquit_Q_triggered()
{
    on_actionclose_c_triggered();
    qApp->quit();
}

void MainWindow::on_actiond_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actiond_2_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionf_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionf_2_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::showFindText()
{
    QString str = findLineEdit->text();
    if(!ui->textEdit->find(str,QTextDocument::FindBackward))
    {
        QMessageBox::warning(this,tr("find"),tr("not find %1").arg(str));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
        event->accept();
    else
        event->ignore();
}

void MainWindow::on_actionc_triggered()
{
    finddlg->show();
}
