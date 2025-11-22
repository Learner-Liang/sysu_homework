#include "haffumantree.h"

int cnt=-1;//全局变量，用于非叶子节点的data值

bool cmp(const HaffmanNode* a,const HaffmanNode* b){//比较函数，用于set排序
    if(a->fre!=b->fre) return a->fre<b->fre;//堆的第一个元素是频率最小的节点
    else return a->data<b->data;//频率相同则按data排序
}

HaffmanNode* HaffumanTree::getNewNode(int data,int fre){
    HaffmanNode* newNode=new HaffmanNode();//创建新节点
    newNode->data=data;
    newNode->fre=fre;
    newNode->LeftChild=NULL;
    newNode->RightChild=NULL;
    return newNode;//返回节点地址
}

void HaffumanTree::deleteTree(HaffmanNode* node){
    if(node==NULL)return;//递归终止条件
    //分别向左子树和右子树递归删除节点
    deleteTree(node->LeftChild);
    deleteTree(node->RightChild);
    delete node;//删除当前节点
}

HaffumanTree::~HaffumanTree(){
    deleteTree(root);//调用删除函数删除哈夫曼树
}

void HaffumanTree::culculateFrequency(const string& input){
    for(int i=0;i<input.size();i++){
        frequencyMap[input[i]]+=1;//统计每个字符出现的频率
    }
    return ;
}

void HaffumanTree::buildHaffmanTree(){
    set<HaffmanNode*,bool(*)(const HaffmanNode* a,const HaffmanNode* b)> 
    nodeSet(cmp);//创建节点集合，按频率从小到大排序
    for(auto it=frequencyMap.begin();it!=frequencyMap.end();it++){
        HaffmanNode* newNode=getNewNode((int)(it->first),it->second);//frequencyMap的key是字符，需要转换为整型
        nodeSet.insert(newNode);//将每个字符节点插入集合
    }
    while(nodeSet.size()>1){
        HaffmanNode* a=*(nodeSet.begin());//取出频率最小的两个节点
        nodeSet.erase(nodeSet.begin());
        HaffmanNode* b=*(nodeSet.begin());
        nodeSet.erase(nodeSet.begin());
        HaffmanNode* newNode=getNewNode(cnt--,a->fre+b->fre);//创建新节点，data设为cnt(负数）表示非叶子节点
        newNode->LeftChild=a;//将频率较小的节点设为左子树
        newNode->RightChild=b;//将频率较大的节点设为右子树
        nodeSet.insert(newNode);//将新节点插入集合
    }
    root=*(nodeSet.begin());//最后集合中剩下的节点即为哈夫曼树的根节点
    return ;
}

//使用dfs遍历哈夫曼树生成编码
void HaffumanTree::generateCodes(HaffmanNode* root,string code){
    if(root==NULL)return;//哈夫曼树为空，直接返回
    if(root->LeftChild==NULL&&root->RightChild==NULL){//没有左右子树，说明是叶子节点
        codeMap[(char)(root->data)]=code;//只有一个字符，直接将其编码设为当前code
        return ;
    }
    generateCodes(root->LeftChild,code+"0");//向左子树递归，编码加0
    generateCodes(root->RightChild,code+"1");//向右子树递归，编码加1
    return ;
}

string HaffumanTree::compress(const string &input){
    string ans;
    for(int i=0;i<input.size();i++){
        ans+=codeMap[input[i]];//将每个字符替换为对应的哈夫曼编码
        //ans+="";//在压缩文本中，用空格分隔每个编码
        //由于哈夫曼编码每个字符的编码不为前缀，因而直接读取即可，无需用空格隔开
    }
    return ans;//返回压缩后的字符串
}

void HaffumanTree::_decompress(const string& input){//读取压缩文本中的编码表
    string temp;
    char ch;
    int  i=1;
    if(input[0]=='\\'&&input[1]=='n') {
        ch='\n';//处理换行符
        i=2;
    }
    else ch=input[0];
    for(;i<input.size();i++){
       temp+=input[i];//跳过开头的目标字符，读取编码表
    }
    charMap[temp]=ch;//将编码与字符对应存入charMap
    return ;
}

string HaffumanTree::decompress(const string &input){
    string ans;
    string temp;
    for(int i=0;i<input.size();i++){
        temp+=input[i];//逐位读取压缩文本，构建当前编码
        if(charMap.find(temp)!=charMap.end()){//如果当前编码在charMap中存在
            ans+=charMap[temp];//将对应字符加入解压结果
            temp="";//清空当前编码，继续读取下一个编码
        }
    }
    return ans;//返回解压后的字符串
}

