#include<iostream>

struct node {
  int val;
  node *pnext;
  node():val(0),pnext(nullptr){}
  node(int v, node* p=nullptr):val(v),pnext(p){}
};

int main(int argc, char* argv[]){

  node *phead = new node(1);
  node *pcurr = phead;
  for(int i=2; i<=10; ++i){
    node* ptmp = new node(i);
    pcurr->pnext = ptmp;
    pcurr = ptmp;
  }

  auto prn = [](const node* phead){
	       const node *p = phead;
	       while(p){
		 std::cout<<p->val<<"\t";
		 p=p->pnext;
	       }
	       std::cout<<std::endl;
	     };

  std::cout<<"forward list from 1 to 10\n";
  prn(phead);

  auto reverse_list = [](node *phead){
			node *pprev, *pcurr, *pnext;
			pprev = nullptr;
			pcurr = phead;
			while(pcurr){
			  //std::cout<<pcurr->val<<",\t";
			  pnext = pcurr->pnext;
			  pcurr->pnext = pprev;
			  pprev = pcurr;
			  pcurr = pnext;
			}
			//std::cout<<std::endl;
			return pprev;
		      };

  node *prhead = reverse_list(phead);
  std::cout<<"reverse list from 10 to 1\n";
  prn(prhead);
  
  return 0;
}
