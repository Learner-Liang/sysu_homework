#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "haffumantree.h"

#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 信号槽（如果你在 .ui 里已经通过 Designer 绑定，也可以省略这里）
    connect(ui->browseInputFileButton, &QPushButton::clicked,
            this, &MainWindow::on_browseInputFile_clicked);
    connect(ui->browseOutputDirButton, &QPushButton::clicked,
            this, &MainWindow::on_browseOutputDir_clicked);
    connect(ui->compressButton, &QPushButton::clicked,
            this, &MainWindow::on_compressButton_clicked);
    connect(ui->decompressButton, &QPushButton::clicked,
            this, &MainWindow::on_decompressButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseInputFile_clicked()
{
    // 既可以选择文本文件，也可以选择 zip 文件
    QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("选择输入文件（待压缩文本或待解压zip）"),
        QString(),
        tr("所有文件 (*.*);;文本文件 (*.txt);;压缩文件 (*.zip)")
    );
    if (!filePath.isEmpty()) {
        ui->inputFileLineEdit->setText(filePath);
    }
}

void MainWindow::on_browseOutputDir_clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(
        this,
        tr("选择输出目录"),
        QString(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (!dirPath.isEmpty()) {
        ui->outputDirLineEdit->setText(dirPath);
    }
}

bool MainWindow::ensureDirExists(const QString &dirPath)
{
    if (dirPath.isEmpty()) return false;
    QDir dir(dirPath);
    if (dir.exists()) return true;
    return dir.mkpath("."); // 若不存在则创建
}

void MainWindow::on_compressButton_clicked()
{
    QString inputPath = ui->inputFileLineEdit->text().trimmed();
    QString outputDir = ui->outputDirLineEdit->text().trimmed();

    if (inputPath.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入或选择待压缩的文本文件路径"));
        return;
    }

    if (outputDir.isEmpty()) {
        // 支持用户直接输入一个路径；若为空，则用输入文件所在目录
        QFileInfo fi(inputPath);
        outputDir = fi.absolutePath();
        ui->outputDirLineEdit->setText(outputDir);
    }

    if (!ensureDirExists(outputDir)) {
        QMessageBox::warning(this, tr("错误"), tr("无法创建输出目录：%1").arg(outputDir));
        return;
    }

    QFileInfo inInfo(inputPath);
    QString outZipPath = QDir(outputDir).filePath(inInfo.completeBaseName() + "_compress.zip");

    if (compressFile(inputPath, outZipPath)) {
        QMessageBox::information(this, tr("完成"),
                                 tr("压缩完成！\n输出文件：%1").arg(outZipPath));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("压缩失败，请检查输入文件和路径。"));
    }
}

void MainWindow::on_decompressButton_clicked()
{
    QString zipPath = ui->inputFileLineEdit->text().trimmed();
    QString outputDir = ui->outputDirLineEdit->text().trimmed();

    if (zipPath.isEmpty()) {
        QMessageBox::warning(this, tr("提示"), tr("请输入或选择待解压的压缩文件路径"));
        return;
    }

    if (outputDir.isEmpty()) {
        QFileInfo fi(zipPath);
        outputDir = fi.absolutePath();
        ui->outputDirLineEdit->setText(outputDir);
    }

    if (!ensureDirExists(outputDir)) {
        QMessageBox::warning(this, tr("错误"), tr("无法创建输出目录：%1").arg(outputDir));
        return;
    }

    QFileInfo zipInfo(zipPath);
    QString outTxtPath = QDir(outputDir).filePath(zipInfo.completeBaseName() + "_decompress.txt");

    if (decompressFile(zipPath, outTxtPath)) {
        QMessageBox::information(this, tr("完成"),
                                 tr("解压完成！\n输出文件：%1").arg(outTxtPath));
    } else {
        QMessageBox::warning(this, tr("错误"), tr("解压失败，请检查压缩文件和路径。"));
    }
}

// 下面两个函数完全仿照你原来 main.cpp 的逻辑，只是换成 Qt 的文件读写接口。
// 算法部分仍然使用 haffumantree.h / haffumantree.cpp 中的类与接口。

bool MainWindow::compressFile(const QString &inputPath, const QString &outputZipPath)
{
    QFile inFile(inputPath);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&inFile);
    in.setCodec("UTF-8"); // 视需要调整编码

    HaffumanTree haffmanTree;
    QString line;
    // 第一次读取：统计频率，构建树和编码表
    while (in.readLineInto(&line)) {
        line += '\n';
        haffmanTree.culculateFrequency(line.toStdString());
        line.clear();
    }
    haffmanTree.buildHaffmanTree();
    haffmanTree.generateCodes(haffmanTree.root, "");

    inFile.close();

    QFile outFile(outputZipPath);
    if (!outFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    QByteArray outData;
    // 写编码表
    for (auto it = haffmanTree.codeMap.begin(); it != haffmanTree.codeMap.end(); ++it) {
        std::string lineStr;
        if (it->first == '\n') lineStr += "\\n";
        else lineStr += it->first;
        lineStr += it->second;
        lineStr += '\n';
        outData.append(lineStr.c_str(), static_cast<int>(lineStr.size()));
    }
    std::string mark = "compress\n";
    outData.append(mark.c_str(), static_cast<int>(mark.size()));

    // 第二次读取：按编码表进行压缩
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        outFile.close();
        return false;
    }
    in.setDevice(&inFile);

    std::string ans;
    while (in.readLineInto(&line)) {
        line += '\n';
        ans += haffmanTree.compress(line.toStdString());
        line.clear();
    }
    outData.append(ans.c_str(), static_cast<int>(ans.size()));

    outFile.write(outData);
    outFile.close();
    inFile.close();
    return true;
}

bool MainWindow::decompressFile(const QString &zipPath, const QString &outputTextPath)
{
    QFile inFile(zipPath);
    if (!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream in(&inFile);
    in.setCodec("UTF-8");

    QFile outFile(outputTextPath);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        inFile.close();
        return false;
    }
    QTextStream out(&outFile);
    out.setCodec("UTF-8");

    HaffumanTree haffmanTree2;
    QString line;
    std::string ans;

    // 先读取编码表直到行内容为 "compress"
    while (in.readLineInto(&line) && line != "compress") {
        haffmanTree2._decompress(line.toStdString());
        line.clear();
    }

    // 继续读取剩余内容，拼接成一个长字符串
    while (!in.atEnd()) {
        QString rest = in.readLine();
        ans += rest.toStdString();
        // 原 main.cpp 在这里是 getline(file, line) 后 ans+=line，
        // 没有再加 '\n'，所以这里也不加。
    }

    std::string decompressedText = haffmanTree2.decompress(ans);
    out << QString::fromStdString(decompressedText);
    outFile.close();
    inFile.close();
    return true;
}
