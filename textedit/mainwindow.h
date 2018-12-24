#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
namespace Ui {
class MainWindow;
}

class QLineEdit;
class QDialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void newFile(); //新建
    void open();//打开文件
    bool maybeSave(); //判断是否需要保存
    bool save(); //保存
    bool saveAs(); //另存为
    bool saveFile(const QString &fileName); //保存文件
    bool loadFile(const QString &filename);

protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void on_actionxi_triggered();

    void on_actionopen_O_triggered();

    void on_actionclose_c_triggered();

    void on_actionstore_S_triggered();

    void on_actionanother_store_A_triggered();

    void on_actionquit_Q_triggered();

    void on_actiond_triggered();

    void on_actiond_2_triggered();

    void on_actionf_triggered();

    void on_actionf_2_triggered();

    void showFindText();

    void on_actionc_triggered();

private:
    Ui::MainWindow *ui;
    bool isUntitled;
    QString curFile;
    QLineEdit *findLineEdit;
    QDialog *finddlg;
};

#endif // MAINWINDOW_H
