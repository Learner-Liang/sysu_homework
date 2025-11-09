#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
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

void MainWindow::on_browseButton_clicked()
{
    selectedFilePath = QFileDialog::getOpenFileName(this, "选择文本文件", "", "Text Files (*.txt);;All Files (*)");
    if (!selectedFilePath.isEmpty()) {
        ui->filePathLineEdit->setText(selectedFilePath);
    }
}

void MainWindow::on_startButton_clicked()
{
    if (selectedFilePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请先选择一个要分析的文本文件。");
        return;
    }

    QString wordsToSearch = ui->wordsLineEdit->text();
    if (wordsToSearch.trimmed().isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入至少一个待统计的单词。");
        return;
    }

    tree = TireTree();
    std::stringstream ss(wordsToSearch.toStdString());
    std::string word;
    while (ss >> word) {
        tree.insert(word);
    }

    QFile file(selectedFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "错误", "无法打开文件: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    int line_num = 1;
    while (!in.atEnd()) {
        QString line = in.readLine();
        std::string stdLine = line.toStdString();
        std::string current_word;
        for (size_t i = 0; i < stdLine.length(); ++i) {
            char c = stdLine[i];
            if (isalpha(c)) {
                current_word += c;
            } else {
                if (!current_word.empty()) {
                    TireNode* p = &tree.root;
                    for(size_t j = 0; j < current_word.length(); j++){
                        int ind;
                        if(current_word[j]>='A'&&current_word[j]<='Z'){ ind=current_word[j]-'A'; }
                        else if(current_word[j]>='a'&&current_word[j]<='z'){ ind=current_word[j]-'a'; }
                        else continue;
                        if(p->children[ind]==nullptr) break;
                        p=p->children[ind];
                        if(p->is_vocabulary){
                            bool found = false;
                            for(auto& pair : tree.word_map[p->word]){
                                if(pair.first == line_num){
                                    pair.second++;
                                    found = true;
                                    break;
                                }
                            }
                            if(!found){
                                tree.word_map[p->word].push_back({line_num, 1});
                            }
                        }
                    }
                    current_word.clear();
                }
            }
        }
        if (!current_word.empty()) {
             TireNode* p = &tree.root;
            for(size_t j = 0; j < current_word.length(); j++){
                int ind;
                if(current_word[j]>='A'&&current_word[j]<='Z'){ ind=current_word[j]-'A'; }
                else if(current_word[j]>='a'&&current_word[j]<='z'){ ind=current_word[j]-'a'; }
                else continue;
                if(p->children[ind]==nullptr) break;
                p=p->children[ind];
                if(p->is_vocabulary){
                    bool found = false;
                    for(auto& pair : tree.word_map[p->word]){
                        if(pair.first == line_num){
                            pair.second++;
                            found = true;
                            break;
                        }
                    }
                    if(!found){
                        tree.word_map[p->word].push_back({line_num, 1});
                    }
                }
            }
        }
        line_num++;
    }
    file.close();

    QString resultText;
    for(auto const& [key, val] : tree.word_map) {
        if (!val.empty()) {
            resultText += QString::fromStdString(key) + " 出现情况:\n";
            for (const auto& p : val) {
                resultText += "  在第 " + QString::number(p.first) + " 行, 出现 " + QString::number(p.second) + " 次\n";
            }
        } else {
             resultText += QString::fromStdString(key) + " 未在文本中出现。\n";
        }
        resultText += "\n";
    }

    if (resultText.isEmpty()) {
        resultText = "所有指定的单词均未在文件中找到。";
    }

    ui->resultTextEdit->setText(resultText);
}
