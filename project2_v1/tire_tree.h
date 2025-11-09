//用字典树完成模式串匹配与单词数量统计
#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;
typedef pair<int,int> PII;//左int表示行号，右int表示该行中的单词数量
typedef struct TireNode{
    TireNode* children[26]={nullptr};//字典树的子节点，当指向nullptr时表示没有该子节点，同时大小写字母等价
    bool is_vocabulary=false;//判断当前节点是否是一个单词的结尾，false表示不是单词结尾，true表示是单词结尾
    string word="";//存储以该节点结尾的单词
}TireNode;
class TireTree{
    private:
        TireNode root;//字典树的根节点
    public:
        //存储模式串匹配结果的哈希表，key为模式串，value为该模式串在文本中出现的位置及对应的单词编号
        unordered_map<string,vector<PII>> word_map;
        TireTree(){};//字典树的构造函数
        ~TireTree();//字典树的析构函数
        void clear_children(TireNode* node);//递归释放字典树节点的内存
        void insert(const string& word);//向字典树中插入一个单词
        TireNode* search(const string& word);//在字典树中查找一个单词
};
