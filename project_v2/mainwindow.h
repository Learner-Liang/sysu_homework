#pragma once

#include <QMainWindow>
#include <QString>

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
    // 压缩相关
    void on_btnBrowseCompressInput_clicked();   // 浏览被压缩文本
    void on_btnBrowseCompressOutput_clicked();  // 浏览压缩 zip 输出目录
    void on_btnCompress_clicked();              // 执行压缩

    // 解压相关
    void on_btnBrowseDecompressInput_clicked();   // 浏览被解压 zip
    void on_btnBrowseDecompressOutput_clicked();  // 浏览解压后文本输出目录
    void on_btnDecompress_clicked();              // 执行解压

private:
    Ui::MainWindow *ui;

    // 辅助函数：确保目录存在，不存在则创建
    bool ensureDirectoryExists(const QString &dirPath);
};
