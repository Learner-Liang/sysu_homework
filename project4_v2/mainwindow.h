#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_browseInputFile_clicked();          // 选择待压缩/解压的文本或zip
    void on_browseOutputDir_clicked();          // 选择压缩文件/解压结果的保存目录
    void on_compressButton_clicked();           // 执行压缩
    void on_decompressButton_clicked();         // 执行解压

private:
    Ui::MainWindow *ui;

    // 工具函数
    bool ensureDirExists(const QString &dirPath); // 若目录不存在则创建
    bool compressFile(const QString &inputPath, const QString &outputZipPath);
    bool decompressFile(const QString &zipPath, const QString &outputTextPath);
};
