#include "tire_tree.h"
#include <fstream>
int main(){
    TireTree tireTree;
    string s;
    while(cin>>s)tireTree.insert(s);
    ifstream file("test.txt");
    string line;
    if (!file.is_open()) {
        cout<< "failed to open test.txt" <<endl;
    } else {
        for (int j = 1; getline(file, line); j++) {// j表示行号
            // 按引用遍历，确保写回原 map
            for (auto& x : tireTree.word_map) {
                x.second.push_back({j, 0}); // 初始化每行该单词出现的次数为0
            }
            cout << line << endl;
            for (int i = 0; i<line.size();i++){
               if(!((line[i]>='A'&&line[i]<='Z')||(line[i]>='a'&&line[i]<='z'))) continue;
                string word;
                for (;i<line.size()&&line[i]!=' ';i++){//以空格为单词分隔符
                    word+=line[i];
                }
                cout << word << " ";
                TireNode* node=tireTree.search(word);
                if(node!=nullptr&&node->is_vocabulary){//找到了该单词
                    tireTree.word_map[node->word].back().second++;//该行该单词出现次数加1
                }
            }
            cout << endl;
        }
        file.close();
    }
    for(auto x:tireTree.word_map){
        cout<<x.first<<endl;
        for(auto pos:x.second){
            if(pos.second==0) continue;//该行未出现该单词，跳过
            cout<<"    Line "<<pos.first<<": "<<pos.second<<" times"<<endl;
        }
        cout << endl;
    }
    return 0;
}
