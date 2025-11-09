#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tire_tree.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 槽函数，用于响应界面操作
    void on_browseButton_clicked();
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    QString selectedFilePath; // 用于存储用户选择的文件路径
};
#endif // MAINWINDOW_H
