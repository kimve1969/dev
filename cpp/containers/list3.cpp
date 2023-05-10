#include<iostream>

struct node{
  int val;
  node *next;
  node(int v):val(v),next(nullptr){
    //std::cout<<"cnt node("<<val<<")\n";
  }
  node(const node& other):next(nullptr){ // cnt copy
    (*this).val = other.val;
    //std::cout<<"cnt copy node(const node& other) node.val = "<<val<<"\n";
  }

  bool operator==(const node &other){ return (*this).val == other.val; }
  bool operator!=(const node &other){ return (*this).val != other.val; }
  ~node(){
    //std::cout<<"dst ~node() node.val = "<<val<<"\n";
  }
};

class list{
private:
  node *head;
  node *tail;
  int m_size;
  
public:
  list():head(nullptr), tail(nullptr),  m_size(0){}
  list(node& n): head(&n), tail(nullptr),  m_size(1){}
  
  void push(const node& n){
    node *p = new node(n);
    !head ? head = tail = p : tail = (*tail).next =  p;
    ++m_size;
  }

  // Example 1 from Linus Torvalds - beginner level
  void remove_1(const node& n){
    node *prev = nullptr;
    node *walk = head;
    
    while( (*walk) != n  ){
      prev = walk;
      walk = walk->next;
    }

    if(!prev)
      head = walk->next;
    else
      prev->next = walk->next;

    --m_size;
  }

  // Example 2 from Linus Torvalds - professional level, inderect
  void remove_2(const node& n){

    node **inderect = &head;
    
    while( (**inderect) != n  ){
      inderect = &(*inderect)->next;
    }

    (*inderect) = (*inderect)->next; // (*inderect) == head or == 
    --m_size;
  }
  
  ~list(){
    for(auto it = begin(); it != end(); ){
      iterator tmp = it;
      ++it;
      tmp = nullptr;
    }
  }
  
  struct iterator{
    node *ptr;
    iterator(node*p): ptr(p){} // cnt
    iterator(const iterator& other): ptr(other.ptr){} // cnt copy
    iterator& operator=(const iterator& other){ ptr = other.ptr; return *this; } // this_iter = other_iter
    iterator& operator=(node* p){
      delete ptr;
      ptr = p;
      return *this;
    }
    iterator& operator++(){ ptr = ptr->next; return *this; } // ++iter
    node& operator*(){ return *ptr; } // *iter
    bool operator!=(const iterator& other){ return (*this).ptr != other.ptr; } // this_iter != other_iter
  };

  iterator begin(){ return iterator(head); }
  iterator end(){ return ++iterator(tail); }

  int size(){ return m_size; }
  
};

int main(int argc, char* argv[]){
  list l;

  auto prn = [](list& l){
    std::cout<<"size of list == "<<l.size()<<"\n";
    for(auto &e: l) std::cout<<e.val<<", ";
    std::cout<<std::endl;
  };
  
  for(int i=0; i<10 /* if i<3 UB Segmentation fault?!*/; ++i){
    l.push(node(i));
  }
  
  prn(l);

  l.remove_1(node(3));
  std::cout<<"after list::remove_1( node(3)) from list\n";

  prn(l);

  l.remove_2(node(4));
  std::cout<<"after list::remove_2( node(4)) from list\n";

  prn(l);
  
  return 0;
}
