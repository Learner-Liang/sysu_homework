#include "tire_tree.h"

void TireTree::clear_children(TireNode* node){//清除当前节点的子树，释放内存
    for(int i=0;i<26;i++){
        if(node->children[i]!=nullptr){
            clear_children(node->children[i]);//递归释放子节点
        }
    }
    delete node; // 替换 free(node)
    return ;
}

TireTree::~TireTree(){//析构函数，释放字典树内存
   for(int i=0;i<26;i++){
        if(root.children[i]!=nullptr){
            clear_children(root.children[i]);//释放根节点的子节点
        }
   }
   return ;
}

void TireTree::insert(const string &word){//在字典树中插入一个单词
    word_map[word]=vector<PII>();//初始化模式串在文本中出现位置的记录
    TireNode *p = &root;
    for (int i = 0,ind; word[i];i++){
        //大小写字母等价处理
        if(word[i]>='A'&&word[i]<='Z'){
            ind=word[i]-'A';
        }
        else if(word[i]>='a'&&word[i]<='z'){
            ind=word[i]-'a';
        }
        else{
            continue;//跳过非字母字符
        }
        //如果子节点不存在则创建新节点
        if(p->children[ind]==nullptr){
            p->children[ind]= new TireNode(); // 替换 malloc
        }
        p=p->children[ind];
    }
    p->is_vocabulary=true;//标记单词结尾
    p->word=word;//存储以该节点结尾的单词
    return ;
}

TireNode* TireTree::search(const string &word){//在字典树中查找一个单词
   TireNode *p=&root;
   for (int i = 0; word[i];i++){
       int ind;
       if(word[i]>='A'&&word[i]<='Z'){
           ind=word[i]-'A';
       }
       else if(word[i]>='a'&&word[i]<='z'){
           ind=word[i]-'a';
       }
       else{
           continue;//跳过非字母字符,比如-连接符号
       }
         if(p->children[ind]==nullptr){
              return nullptr;//未找到该单词
         }
        p=p->children[ind];
   }
    return p;//直接返回该单词对应的节点
}
