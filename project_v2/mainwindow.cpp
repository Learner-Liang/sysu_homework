#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include <fstream>
#include <string>
#include "../project4/haffumantree.h"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 设置默认输出目录为当前路径下 project_v2_output
    QString defaultOutputDir = QDir::currentPath() + "/project_v2_output";
    ui->lineEditCompressOutputDir->setText(defaultOutputDir);
    ui->lineEditDecompressOutputDir->setText(defaultOutputDir);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 辅助函数：确保目录存在，不存在则创建
bool MainWindow::ensureDirectoryExists(const QString &dirPath)
{
    if (dirPath.isEmpty()) return false;
    QDir dir(dirPath);
    if (dir.exists()) return true;
    return dir.mkpath("."); // 创建多级目录
}

// ============ 压缩相关槽函数 ============

// 浏览被压缩文本
void MainWindow::on_btnBrowseCompressInput_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择要压缩的文本文件"),
        QDir::currentPath(),
        tr("Text Files (*.txt);;All Files (*)"));
    if (!filePath.isEmpty()) {
        ui->lineEditCompressInput->setText(filePath);
    }
}

// 浏览压缩 zip 输出路径（选择输出目录）
void MainWindow::on_btnBrowseCompressOutput_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("选择压缩文件输出目录"),
        QDir::currentPath());
    if (!dir.isEmpty()) {
        ui->lineEditCompressOutputDir->setText(dir);
    }
}

// 执行压缩（仿照 project4/main.cpp）
void MainWindow::on_btnCompress_clicked()
{
    QString inputPath = ui->lineEditCompressInput->text().trimmed();
    QString outputDir = ui->lineEditCompressOutputDir->text().trimmed();
    QString outputFileName = ui->lineEditCompressOutputFileName->text().trimmed();

    if (inputPath.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请选择被压缩的文本文件"));
        return;
    }

    if (outputDir.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请输入或选择压缩文件输出目录"));
        return;
    }

    // 目录不存在则直接创建
    if (!ensureDirectoryExists(outputDir)) {
        QMessageBox::warning(this, tr("错误"), tr("无法创建输出目录：%1").arg(outputDir));
        return;
    }

    if (outputFileName.isEmpty()) {
        // 默认文件名
        outputFileName = "test_compress.zip";
    }

    QString outputPath = QDir(outputDir).filePath(outputFileName);

    // ------- 以下逻辑严格仿照 project4/main.cpp，只是把文件名改为用户选择的路径 -------

    ifstream file(inputPath.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, tr("错误"),
                              tr("无法打开文件: %1").arg(inputPath));
        return;
    }

    HaffumanTree haffmanTree;
    string line;
    while (getline(file, line)) {
        line+='\n'; // 保留换行符
        haffmanTree.culculateFrequency(line);
        line.clear();//清空line以便下一次读取
    }
    haffmanTree.buildHaffmanTree();
    haffmanTree.generateCodes(haffmanTree.root, "");
    file.close();

    ofstream outFile(outputPath.toStdString(), ios::binary);
    if (!outFile.is_open()) {
        QMessageBox::critical(this, tr("错误"),
                              tr("无法创建文件: %1").arg(outputPath));
        return;
    }

    string ans;
    for(auto it=haffmanTree.codeMap.begin();it!=haffmanTree.codeMap.end();it++){
        string line2="";
        if(it->first=='\n') line2+="\\n";
        else line2+=it->first;
        line2+=it->second;
        line2+='\n';
        outFile.write(line2.c_str(), line2.size());
    }
    ans="compress\n";
    outFile.write(ans.c_str(), ans.size());

    file.open(inputPath.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, tr("错误"),
                              tr("无法重新打开文件: %1").arg(inputPath));
        return;
    }
    ans.clear();
    while (getline(file, line)) {
        line+='\n'; // 保留换行符
        haffmanTree.culculateFrequency(line);
        ans+=haffmanTree.compress(line);
        line.clear();//清空line以便下一次读取
    }
    outFile.write(ans.c_str(), ans.size());

    outFile.close();
    file.close();

    QMessageBox::information(this, tr("完成"),
                             tr("压缩完成！输出文件：%1").arg(outputPath));
}

// ============ 解压相关槽函数 ============

// 浏览被解压 zip
void MainWindow::on_btnBrowseDecompressInput_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择要解压的 zip 文件"),
        QDir::currentPath(),
        tr("Zip Files (*.zip);;All Files (*)"));
    if (!filePath.isEmpty()) {
        ui->lineEditDecompressInput->setText(filePath);
    }
}

// 浏览解压输出目录
void MainWindow::on_btnBrowseDecompressOutput_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("选择解压后文本输出目录"),
        QDir::currentPath());
    if (!dir.isEmpty()) {
        ui->lineEditDecompressOutputDir->setText(dir);
    }
}

// 执行解压（仿照 project4/main.cpp 下半部分）
void MainWindow::on_btnDecompress_clicked()
{
    QString inputZipPath = ui->lineEditDecompressInput->text().trimmed();
    QString outputDir = ui->lineEditDecompressOutputDir->text().trimmed();
    QString outputFileName = ui->lineEditDecompressOutputFileName->text().trimmed();

    if (inputZipPath.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请选择被解压的 zip 文件"));
        return;
    }

    if (outputDir.isEmpty()) {
        QMessageBox::warning(this, tr("警告"), tr("请输入或选择解压后文本输出目录"));
        return;
    }

    // 目录不存在则直接创建
    if (!ensureDirectoryExists(outputDir)) {
        QMessageBox::warning(this, tr("错误"), tr("无法创建输出目录：%1").arg(outputDir));
        return;
    }

    if (outputFileName.isEmpty()) {
        outputFileName = "test_depress.txt";
    }

    QString outputTextPath = QDir(outputDir).filePath(outputFileName);

    HaffumanTree haffmanTree2;
    ifstream file(inputZipPath.toStdString());
    if (!file.is_open()) {
        QMessageBox::critical(this, tr("错误"),
                              tr("无法打开文件: %1").arg(inputZipPath));
        return;
    }
    ofstream outFile(outputTextPath.toStdString());
    if (!outFile.is_open()) {
        QMessageBox::critical(this, tr("错误"),
                              tr("无法创建文件: %1").arg(outputTextPath));
        return;
    }

    string ans;
    string line;
    while(getline(file, line) && line!="compress"){
        haffmanTree2._decompress(line);
        line.clear();
    }
    while (getline(file, line)) {
        ans+=line;
        line.clear();//清空line以便下一次读取
    }
    string decompressedText = haffmanTree2.decompress(ans);
    outFile.write(decompressedText.c_str(), decompressedText.size());
    outFile.close();
    file.close();

    QMessageBox::information(this, tr("完成"),
                             tr("解压完成！输出文件：%1").arg(outputTextPath));
}
