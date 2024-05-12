#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

int Char_num, Test_num, f[70], sum;
char c[70];

struct HuffmanNode{
    int weight;
    struct HuffmanNode* Left;
    struct HuffmanNode* Right;
};
struct MinHeapNode{
    int size;
    struct HuffmanNode* data[70];
};

/* Function Area */
struct MinHeapNode* CreateMinHeapNode(){
    struct MinHeapNode* MinHeap = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    MinHeap->size = 0;
    MinHeap->data[0] = (struct HuffmanNode*)malloc(sizeof(struct HuffmanNode));
    MinHeap->data[0]->weight = -1;
    MinHeap->data[0]->Left = NULL;
    MinHeap->data[0]->Right = NULL;
    // data[0]是不会被用到的，根从data[1]开始
    return MinHeap;
}
struct HuffmanNode* CreateHuffmanNode(){
    struct HuffmanNode* Huffman = (struct HuffmanNode*)malloc(sizeof(struct HuffmanNode));
    Huffman->weight = 0;
    Huffman->Left = NULL;
    Huffman->Right = NULL;
    return Huffman;
}
void Insert(MinHeapNode* heap, HuffmanNode* node){
    heap->size++;
    int k = heap->size;
    while (node->weight < heap->data[k>>1]->weight){
        heap->data[k] = heap->data[k>>1];
        k = k>>1;
    }
    heap->data[k] = node;
    // node[k>>1] 是 node[k] 的父节点
    // 如果要插入的值比他的父节点大，那么就插入到当前位置
    // 否则继续往上找，移动父节点到当前节点位置。
}
struct HuffmanNode* GetMinAndDelete(struct MinHeapNode* heap){
    struct HuffmanNode* Minterm = heap->data[1];
    struct HuffmanNode* temp = heap->data[heap->size --];
    // 等于要把temp插入到新的树中的某一个位置
    int parent = 1; // 从头开始找
    while(parent * 2 <= heap->size){
        int Leftchild = parent * 2; 
        int Rightchild = parent * 2 + 1;
        int Smallerchild = Leftchild;
        if(Leftchild != heap->size){        // 第一个条件表示如果当前节点还有右孩子
            if(heap->data[Leftchild]->weight > heap->data[Rightchild]->weight) // 第二个条件表示如果左孩子的值比右孩子大
                Smallerchild = Rightchild;  // 则把右孩子设置为更小的孩子
        }    
        if(temp->weight <= heap->data[Smallerchild]->weight)    // 如果要插入的值比当前节点小一些的孩子还小
            break;  //即可插入到当前节点位置
        else    
            heap->data[parent] = heap->data[Smallerchild];      // 否则把更小的孩子替换到当前节点
        parent = Smallerchild;
    }
    heap->data[parent]=temp;
    return Minterm;
}
struct HuffmanNode* buildHuffman(struct MinHeapNode* heap){
    struct HuffmanNode* node;
    int times = heap->size;
    for(int i = 1;i < times;i ++){
        node = CreateHuffmanNode();
        node->Left = GetMinAndDelete(heap);
        node->Right = GetMinAndDelete(heap);
        node->weight = node->Left->weight + node->Right->weight;
        Insert(heap, node);
        // 将堆中的最小和次小融合成一个点放回堆中，每次点的总数减一
        // 因此，经过size - 1次操作后一定会将size个点融合为一个点
    }
    return GetMinAndDelete(heap);   // 那么最后剩下的就是我们要的霍夫曼树
}
int WPL(struct HuffmanNode* node,int depth){
    if(node->Left == NULL && node->Right == NULL) return depth * node->weight;
    else return (WPL(node->Left,depth+1) + WPL(node->Right,depth+1)) ;
}
bool judge(){
    struct HuffmanNode* node,* pos;
    char ch;
    string code;
    bool flag = true;
    int j, fpos, sum_up=0;
    node = CreateHuffmanNode();
    for(int i = 0;i < Char_num;i++){
        cin >> ch >> code;
        pos = node;
        if(code.length() >= Char_num) flag = false;
        else if(flag){
            for(j=0;ch!=c[j];j++);  // 找到ch在表中对应的位置
            fpos=f[j];
            for(j=0;j<code.length();j++){
                if(code[j]=='0'){
                    if(pos->Left==NULL) pos->Left=CreateHuffmanNode();
                    pos = pos->Left;
                }
                if(code[j]=='1'){
                    if(pos->Right==NULL) pos->Right=CreateHuffmanNode();
                    pos = pos->Right;
                }
                if(pos->weight != 0) flag = false;
            }
            if(pos->Left || pos->Right) flag = false;
            else pos->weight = fpos;
            sum_up += code.length() * pos->weight;
        }
    }
    if(sum_up != sum) flag = false;
    return flag;
}

int main(){
    cin >> Char_num;
    struct HuffmanNode* tmp;
    struct MinHeapNode* MinHeap = CreateMinHeapNode();
    for(int i = 0;i < Char_num;i ++){
        cin >> c[i] >> f[i];
        tmp = CreateHuffmanNode();
        tmp->weight = f[i];
        Insert(MinHeap, tmp);
    }
    struct HuffmanNode* root = buildHuffman(MinHeap);
    sum = WPL(root,0);
    cin >> Test_num;
    while(Test_num > 0){
        Test_num--;
        if(judge()) cout << "Yes" << endl;
        else cout << "No" << endl;
    }
}