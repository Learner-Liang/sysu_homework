#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;


typedef struct HaffmanNode{//哈夫曼树节点
    int data;//当前叶子节点代表的字符
    int fre=0;//字符出现的频率
    HaffmanNode *LeftChild=NULL;//哈夫曼树的左节点
    HaffmanNode *RightChild=NULL;//哈夫曼树的右节点
}HaffmanNode;

bool cmp(HaffmanNode* a,HaffmanNode* b);//比较函数，用于set排序

class HaffumanTree {
    public:
    unordered_map<char,int> frequencyMap;//统计字符频率
    unordered_map<char,string> codeMap;//存储字符对应的编码，用于压缩
    HaffmanNode* root=NULL;//哈夫曼树根节点
    unordered_map<string,char> charMap;//存储编码对应的字符，用于解压

    HaffmanNode* getNewNode(int data,int fre);//创建新节点
    ~HaffumanTree();//析构函数
    void buildHaffmanTree();//构建哈夫曼树
    void generateCodes(HaffmanNode* root, string code);//生成哈夫曼编码
    void culculateFrequency(const string &input);//计算字符频率
    void deleteTree(HaffmanNode* node);//删除哈夫曼树

    string compress(const string &input);//压缩函数
    void _decompress(const string &input);//解压函数,用于读取编码表
    string decompress(const string &input);//解压函数,用于解码
};
