#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
using namespace std;
int nVal;
int nPointer;
struct node {
    bool leaf;
    bool isRoot;
    node *par;
    vector<int>value;
    vector<node*>child;

    node *last;
};
struct pNode{
    node *tNode;
    bool nl;
    pNode(node *Node, bool b){
        tNode = Node;
        nl = b;
    }
    pNode(){

    }
};
node *Root=NULL;
queue<pNode>q;

node* getTargetNode(node *tNode, int val);
node* getNewNode(bool isLeaf,bool isRoot);

void insertInParentNode(node *n, int selectedNode, node *newsister);
void insertInLeafNode(node *leafNode, int k, node *p);
void insert2(int k, node *p);
void valueOfNodeInBox(node* tNode);
void bfsTraverse(node *tNode);
void phDelete(node* N, int k, node* p);
bool tooFewEntry(node *N);
void deleteEntry(node* N, int k, node* p);
void delet(int k, node* p);
bool isCoalesce(node *N, node *newsister);



int main(){
    printf("You can set the number of values in each node in input text file in this directory . By defult it's 4\n");
    FILE *fp;
    fp = fopen("input.txt","r");
    fscanf(fp,"%d",&nPointer);
    nVal = nPointer-1;


    printf("nPointer = %d\n",nPointer);
    int k;
    while(fscanf(fp,"%d",&k)!=EOF){
        printf("val: %d\n",k);
        insert2(k,NULL);
    }
    while(true){
        printf("Action: \npress 1 to insert\npress 2 to print in tree structure\npress 3 for delete\npress 0 for exit\n");
        int choice;
        scanf("%d",&choice);
        if(choice==1){
            int value;
            scanf("%d",&value);
            insert2(value,NULL);
        }else if(choice==2){
            printf("\n\n\n");
            bfsTraverse(Root);
            printf("\n\n\n");
        }else if(choice==3){
            int delV;
            scanf("%d",&delV);
            delet(delV,NULL);
        }else if(choice==0) break;
    }
    fclose(fp);
    return 0;
}
node* getTargetNode(node *tNode, int val){
    if(tNode->leaf) return tNode;
    int i;
    for(i=0;i<tNode->value.size();i++){
        if(tNode->value[i]>val) break;
    }
    return getTargetNode(tNode->child[i],val);
}
node* getNewNode(bool isLeaf,bool isRoot){
    node* tmp = new node;
    tmp->isRoot = isRoot;
    tmp->leaf = isLeaf;
    tmp->last = NULL;
    return tmp;
}
void insertInParentNode(node *n, int selectedNode, node *newsister){

    if(n->isRoot){
        Root = getNewNode(false,true);
        n->isRoot=false;

        Root->child.push_back(n);
        Root->child.push_back(newsister);
        Root->value.push_back(selectedNode);
        n->par = Root;
        newsister->par = Root;
    }else{
   
        node *p = n->par;

   
        int i;
        for(i=0;i<p->value.size();i++){
            if(p->value[i]>selectedNode) break;
        }
        int tmpK;
        node *tmpP;

        for(int j = i; j<p->value.size(); j++){
            tmpK = p->value[j];
            tmpP = p->child[j+1];

            p->value[j] = selectedNode;
            p->child[j+1] = newsister;

            selectedNode = tmpK;
            newsister = tmpP;
        }
        p->value.push_back(selectedNode);
        p->child.push_back(newsister);
        newsister->par = p;


        if(p->child.size()>nPointer){
            node *pprime = getNewNode(false,false);
            int nbytwoceil = (nPointer+1)/2;
            int kdoubleprime = p->value[nbytwoceil-1];
            for(i = nbytwoceil; i<p->value.size();i++){
                pprime->child.push_back(p->child[i]);
                p->child[i]->par = pprime;
                pprime->value.push_back(p->value[i]);
            }
            pprime->child.push_back(p->child[i]);
            p->child[i]->par = pprime;


            p->value.erase(p->value.begin()+nbytwoceil-1,p->value.end());
            p->child.erase(p->child.begin()+nbytwoceil,p->child.end());

            insertInParentNode(p,kdoubleprime,pprime);
        }
    }

}


void insertInLeafNode(node *leafNode, int k, node *p){
    int i;
    for(i=0;i<leafNode->value.size();i++){
        if(k<leafNode->value[i]) break;
    }
    int tmpK;
    node *tmpP;

    for(int j = i; j<leafNode->value.size(); j++){
        tmpP = leafNode->child[j];
        tmpK = leafNode->value[j];

        leafNode->child[j] = p;
        leafNode->value[j] = k;

        p = tmpP;
        k = tmpK;
    }
    leafNode->child.push_back(p);
    leafNode->value.push_back(k);

}


void insert2(int k, node *p){
    node *leafNode;
    if(Root==NULL){
        Root = getNewNode(true,true);
        leafNode = Root;
    }else leafNode = getTargetNode(Root,k);

 
    int keyValueCount = leafNode->value.size();
    if(keyValueCount<nVal) insertInLeafNode(leafNode,k,p);
    else{


        node* leafNode2 = getNewNode(true,false);
        insertInLeafNode(leafNode,k,p);

  

        leafNode2->last = leafNode->last;
        leafNode2->par = leafNode->par;

        leafNode->last=leafNode2;
        int nbytwoceil = (nPointer+1)/2;

        for(int i = nbytwoceil; i<nPointer ; i++){
            leafNode2->child.push_back(leafNode->child[i]);
            leafNode2->value.push_back(leafNode->value[i]);
        }
        leafNode->value.erase(leafNode->value.begin()+nbytwoceil,leafNode->value.end());
        leafNode->child.erase(leafNode->child.begin()+nbytwoceil,leafNode->child.end());

        int selectedNode = leafNode2->value[0];
        insertInParentNode(leafNode,selectedNode,leafNode2);
    }
}
void valueOfNodeInBox(node* tNode){
    printf(" [");
    int i ;
     for(i=0; i<tNode->value.size()-1;i++){
        printf("%d|",tNode->value[i]);
    }
    if(tNode->value.size()>0) printf("%d]",tNode->value[i]);

}




void bfsTraverse(node *tNode){

    q.push(pNode(tNode,true));
    while(!q.empty()){
        pNode p = q.front();

        node *temp = p.tNode;
        q.pop();
        valueOfNodeInBox(temp);

        if(p.nl) printf("\n");
        int i;
        if(!temp->leaf){
        for(i=0;i<temp->child.size()-1;i++){
            q.push(pNode(temp->child[i],false));
        }

        if(p.nl) q.push(pNode(temp->child[i],true));
        else q.push(pNode(temp->child[i],false));

        }

    }
}

void phDelete(node* N, int k, node* p){
    int i;
    for(i=0;i<N->value.size();i++){
        if(N->value[i]==k) break;
    }
    N->value.erase(N->value.begin()+i);
    if(!N->leaf) N->child.erase(N->child.begin()+i+1);
    printf("new Stracture: \n");
    if(N->value.size()>0)bfsTraverse(N);
}


bool tooFewEntry(node *N){

    if(N->leaf){
        int minV = nPointer/2;
        if(N->value.size()<minV) return true;
    }else{
        int minC = (nPointer+1)/2;
        if(N->child.size()<minC) return true;
    }
    return false;
}

void deleteEntry(node* N, int k, node* p){
    phDelete(N,k,p);
    if(N->isRoot && N->child.size()==1){
      Root = N->child[0];
      Root->par = NULL;
      Root->isRoot = true;
    }
    if(tooFewEntry(N) && !N->isRoot){
        printf("too few entry\n");
        node *parN = N->par;
        int i;
        for(i=0;i<parN->child.size();i++){
            if(parN->child[i]==N) break;
        }
        printf("*~*: The node is %dth child of its parent\n",i);
        printf("*~*: parent has %d childs\n",parN->child.size());

        node *leftSib, *rightSib, *newsister;
        bool left;
        if(i>0) leftSib = parN->child[i-1];
        else leftSib = NULL;

        if(i<parN->child.size()-1) rightSib = parN->child[i+1];
        else rightSib = NULL;

        if(leftSib==NULL) left=false;
        else if(rightSib==NULL) left = true;
        else if(leftSib->value.size()>rightSib->value.size()) left=false;
        else left = true;
        int selectedNode;

        if(left){
            newsister = leftSib;
            selectedNode = parN->value[i-1];
            printf("Left sibling selected\n");
        }
        else {
            newsister = rightSib;
            selectedNode = parN->value[i];
            printf("Right sibling selected\n");
        }
        printf("selectedNode %d\n",selectedNode);


        if(isCoalesce(N,newsister)){
                printf("Coalesce\n");
                if(!left){
                    node *tmp = N;
                    N = newsister;
                    newsister = tmp;
                }

                if(!N->leaf){
                    newsister->value.push_back(selectedNode);
                    int j;
                    for(j = 0; j<N->value.size();j++){
                        newsister->child.push_back(N->child[j]);
                        newsister->value.push_back(N->value[j]);
                        N->child[j]->par = newsister;
                    }
                    newsister->child.push_back(N->child[j]);
                    N->child[j]->par = newsister;

                }else{
              );
                     printf("Content of newsister(before)\n");
                     bfsTraverse(newsister);



                    for(int j = 0; j<N->value.size();j++){
                        newsister->value.push_back(N->value[j]);
          
                    }
          
                     newsister->last = N->last;
      
                }
                printf("newsister content(after): \n");
                bfsTraverse(newsister);
                deleteEntry(parN,selectedNode,N);

        }else{
                printf("*~*: Redistribute\n");

                printf("*~*: content of newsister\n");
                bfsTraverse(newsister);

                if(left){
                    printf("*~*: redistribute for left\n");

                    if(!N->leaf){
                        int m = newsister->child.size()-1;
                        int tmpV = newsister->value[m-1];
                        node *tmpP = newsister->child[m];
                        newsister->child.erase(newsister->child.begin()+m);
                        newsister->value.erase(newsister->value.begin()+m-1);
                        N->child.insert(N->child.begin(),tmpP);
                        N->value.insert(N->value.begin(),selectedNode);
                        parN->value[i-1] = tmpV;

                    }else{
                        int m = newsister->value.size()-1;
                        int tmpV = newsister->value[m];

                        newsister->value.erase(newsister->value.begin()+m);

                        N->value.insert(N->value.begin(),tmpV);
                        parN->value[i-1] = tmpV;
                    }
                }else{

                    if(!N->leaf){
                        int m = 0;
                        int tmpV = newsister->value[m];
                        node *tmpP = newsister->child[m];

                        newsister->child.erase(newsister->child.begin()+m);
                        newsister->value.erase(newsister->value.begin()+m);

                        N->child.push_back(tmpP);
                        N->value.push_back(selectedNode);

 

                        parN->value[i-1] = tmpV;
                    }else{
                        int m = 0;
                        int tmpV = newsister->value[m];
      
                        newsister->value.erase(newsister->value.begin());
     ;
                        N->value.push_back(tmpV);
                        parN->value[i] = newsister->value[0];
                    }

                }
        }


    }else{
        printf("Enough Entry\n");
    }

}

void delet(int k, node* p){
    node *L = getTargetNode(Root,k);
    printf("content: \n");
    bfsTraverse(L);
    deleteEntry(L,k,p);

}


bool isCoalesce(node *N, node *newsister){
    if(N->leaf){
        if(nVal>=(N->value.size()+newsister->value.size())) return true;
        return false;
    }
    if(nPointer>=(N->child.size()+newsister->child.size())) return true;
    return false;
}
