 #include <QApplication>
#include <QWidget> 
#include <QLineEdit> 
#include <QPushButton> 
#include <QLabel> 
#include <QTextEdit>
#include <QFileDialog>
#include <QVBoxLayout> 
#include <QHBoxLayout> 
#include <QMessageBox> 
#include <QDir>

   #include <fstream> #include <string> #include <vector> #include <utility>
   #include <unordered_map>

   #include "../project2/tire_tree.h"

   static bool isAlpha(char c) {  return (c>='A'&&c<='Z') || (c>='a'&&c<='z'); }

   int main(int argc, char* argv[]) {  QApplication app(argc, argv);

     QWidget w;
     w.setWindowTitle(QStringLiteral("文本统计器（字典树）"));

     auto* inputEdit = new QLineEdit;
     inputEdit->setPlaceholderText(QStringLiteral("输入待统计词汇，空格分隔（可多个）"));

     auto* chooseBtn = new QPushButton(QStringLiteral("选择文本文件"));
     auto* runBtn = new QPushButton(QStringLiteral("完成"));
     auto* fileLabel = new QLabel(QStringLiteral("未选择文件"));
     fileLabel->setWordWrap(true);

     auto* outputBox = new QTextEdit;
     outputBox->setReadOnly(true);

     QString selectedFile;

     QObject::connect(chooseBtn, &QPushButton::clicked, [&] {
         const QString path = QFileDialog::getOpenFileName(&w,
             QStringLiteral("选择文本文件"),
             QString(),
             QStringLiteral("文本文件 (*.txt);;所有文件 (*.*)"));
         if (!path.isEmpty()) {
             selectedFile = path;
             fileLabel->setText(QStringLiteral("文件：") + QDir::toNativeSeparators(path));
         }
     });

     QObject::connect(runBtn, &QPushButton::clicked, [&] {
         if (inputEdit->text().trimmed().isEmpty()) {
             QMessageBox::warning(&w, QStringLiteral("提示"), QStringLiteral("请先输入待统计词汇（空格分隔）。"));
             return;
         }
         if (selectedFile.isEmpty()) {
             QMessageBox::warning(&w, QStringLiteral("提示"), QStringLiteral("请先选择要读取的文本文件。"));
             return;
         }

         TireTree tireTree;
         const QStringList words = inputEdit->text().split(QRegExp("\\s+"), Qt::SkipEmptyParts);
         for (const QString& qw : words) {
             tireTree.insert(qw.toStdString());
         }

         std::ifstream file(selectedFile.toStdString());
         if (!file.is_open()) {
             QMessageBox::critical(&w, QStringLiteral("错误"), QStringLiteral("无法打开文件：") + selectedFile);
             return;
         }

         std::string line;
         int j = 0;
         while (std::getline(file, line)) {
             ++j;
             for (auto& x : tireTree.word_map) {
                 x.second.push_back({j, 0});
             }
             for (int i = 0; i < static_cast<int>(line.size()); i++) {
                 if (!isAlpha(line[i])) continue;
                 std::string word;
                 for (; i < static_cast<int>(line.size()) && line[i] != ' '; i++) {
                     word += line[i];
                 }
                 TireNode* node = tireTree.search(word);
                 if (node != nullptr && node->is_vocabulary) {
                     tireTree.word_map[node->word].back().second++;
                 }
             }
         }
         file.close();

         QString result;
         for (const auto& kv : tireTree.word_map) {
             result += QString::fromStdString(kv.first) + "\n";
             for (const auto& pos : kv.second) {
                 if (pos.second == 0) continue;
                 result += QString("    Line %1: %2 times\n").arg(pos.first).arg(pos.second);
             }
             result += "\n";
         }
         outputBox->setPlainText(result.isEmpty() ? QStringLiteral("未找到任何匹配结果。") : result);
     });

     auto* top = new QVBoxLayout;
     auto* row1 = new QHBoxLayout;
     row1->addWidget(new QLabel(QStringLiteral("输入词汇：")));
     row1->addWidget(inputEdit);

     auto* row2 = new QHBoxLayout;
     row2->addWidget(chooseBtn);
     row2->addWidget(fileLabel, 1);
     row2->addWidget(runBtn);

     top->addLayout(row1);
     top->addLayout(row2);
     top->addWidget(outputBox, 1);

     w.setLayout(top);
     w.resize(760, 460);
     w.show();
     return app.exec();

   }
