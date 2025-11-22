#include "haffumantree.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {
    ifstream file("example_txt01.txt");
    if (!file.is_open()) {
        cerr << "无法打开文件 example_txt01.txt" << std::endl;
        return 1;
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

    ofstream outFile("test_compress.zip", ios::binary);
        if (!outFile.is_open()) {
            cerr << "无法创建文件 test_compress.zip" << std::endl;
            return 1;
        }
    string ans;
    for(auto it=haffmanTree.codeMap.begin();it!=haffmanTree.codeMap.end();it++){
        string line="";
        if(it->first=='\n') line+="\\n";
        else line+=it->first;
        line+=it->second;
        line+='\n';
        outFile.write(line.c_str(), line.size());
    }
    ans="compress\n";
    outFile.write(ans.c_str(), ans.size());

    file.open("example_txt01.txt");
    if (!file.is_open()) {
        cerr << "无法打开文件 example_txt01.txt" << std::endl;
        return 1;
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

    HaffumanTree haffmanTree2;
    file.open("test_compress.zip");
    if (!file.is_open()) {
        cerr << "无法打开文件 test_compress.zip" << endl;
        return 1;
    }
    outFile.open("test_depress.txt");
    if (!outFile.is_open()) {
        cerr << "无法创建文件 test_depress.txt" << endl;
        return 1;
    }
    ans.clear();
    line.clear();
    while(getline(file, line)&&line!="compress"){
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
    return 0;
}

