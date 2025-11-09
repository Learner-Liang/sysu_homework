#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("单词频率统计工具");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// “浏览文件”按钮的点击事件
void MainWindow::on_browseButton_clicked()
{
    // 打开文件对话框让用户选择文件
    selectedFilePath = QFileDialog::getOpenFileName(this,
        tr("选择文本文件"), "", tr("Text Files (*.txt);;All Files (*)"));

    // 如果用户成功选择了文件，则在界面上显示路径
    if (!selectedFilePath.isEmpty()) {
        ui->filePathLabel->setText(selectedFilePath);
    }
}

// “开始统计”按钮的点击事件
void MainWindow::on_startButton_clicked()
{
    // 1. 从界面的输入框获取用户输入的、需要统计的词汇
    QString wordsToCountText = ui->wordsInput->toPlainText();
    if (wordsToCountText.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入待统计的词汇。");
        return;
    }

    // 2. 检查用户是否已选择文件
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个文本文件。");
        return;
    }

    // 3. 创建字典树实例，并将用户输入的词汇插入树中
    TireTree tireTree;
    std::stringstream wordsStream(wordsToCountText.toStdString());
    string s;
    while (wordsStream >> s) {
        tireTree.insert(s);
    }

    // 4. 打开并逐行读取文件，统计词频
    // 这里的逻辑严格仿照您的 project2.cpp
    QFile file(selectedFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件: " + selectedFilePath);
        return;
    }

    QTextStream in(&file);
    string line;
    for (int j = 1; !in.atEnd(); j++) { // j 表示行号
        line = in.readLine().toStdString();

        // 对每个待查单词，初始化其在本行的出现次数为0
        for (auto& x : tireTree.word_map) {
            x.second.push_back({j, 0});
        }

        // 遍历行中的每个字符，提取单词
        for (int i = 0; i < line.size(); i++) {
           // 跳过非字母字符
           if (!((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z'))) continue;
            
            string word;
            // 提取连续的字母组成单词
            for (; i < line.size() && ((line[i] >= 'A' && line[i] <= 'Z') || (line[i] >= 'a' && line[i] <= 'z')); i++) {
                word += line[i];
            }
            i--; // 外层循环会i++，所以这里回退一格，以避免跳过字符

            TireNode* node = tireTree.search(word);
            if (node != nullptr && node->is_vocabulary) { // 如果提取的单词是待查词汇
                tireTree.word_map[node->word].back().second++; // 该行该单词出现次数加1
            }
        }
    }
    file.close();

    // 5. 格式化统计结果并显示在界面的输出框中
    QString resultText;
    for (auto const& x : tireTree.word_map) {
        resultText += QString::fromStdString(x.first) + "\n";
        bool found = false;
        for (auto const& pos : x.second) {
            if (pos.second > 0) { // 只显示出现次数大于0的行
                found = true;
                resultText += QString("    Line %1: %2 times\n").arg(pos.first).arg(pos.second);
            }
        }
        if (!found) {
             resultText += "    未在文件中找到该单词。\n";
        }
        resultText += "\n";
    }

    ui->resultOutput->setPlainText(resultText);
}
