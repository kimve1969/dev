#include<iostream>
// if rvalue has variable or formal parameter, then
// rvalue has behavor as lvalue

struct X{
  int m_a;
  X(int x=0):m_a(x){ std::cout<<"X(int)\n"; }
  X(const X& x):m_a(x.m_a){ std::cout<<"X(const X&)\n"; }
  X(X&& x){ std::cout<<"X(X&&)\n"; }
  X& operator=(const X& x){
    std::cout<<"X::operator(const X&)\n";
    m_a = x.m_a;
    return (*this);
  }
};

std::ostream& operator<<(std::ostream& out, const X& x){ return out<<x.m_a; }

X fval(){ return X(0); }
X glbx(10);
X& flval(){ return glbx; }
const X& fclval(){ return glbx; }
X&& frval(){ return std::move(glbx); }

void g_by_val(X x){}
void g_by_lval(X& x){}
void g_by_clval(const X& x){}
void g_by_rval(X&& x){}

void step(){
  static int s = 1;
  for(int i=0; i<s; ++i){
    std::cout<<".";
  }
  std::cout<<std::endl;
  s++;
}

int main(){
  // --------------------- variants type X
  std::cout<<"start\n";
  X x{0}; // call cnt X(int)
  step();
  X& lrefx = x; // do nothing...?
  step();
  lrefx = 1; // call X(1), call operator=(X(1))
  step();
  const X& clrefx = 5; // call X(X(5)) is equal X(5)
  step();
  X&& rrefx = X(6); // call X(X(5)) is equal X(5)
  step();
  std::cout<<"x = "<<x<<", lrefx = "<<lrefx<<", clrefx = "<<clrefx<<", rrefx = "<<rrefx<<std::endl;

  fval(); // call X(int)
  step();
  flval(); // do nothing
  step();
  fclval(); // do nothing
  step();
  frval();  // do nothing
  step();

  // ------- TEST to pass parameters to function------
  std::cout<<"\nTEST g_by_val()\n";
  
  g_by_val(x); // copy param by cnt X(const X&)
  step();
  g_by_val(lrefx); // copy param by cnt X(const X&)
  step();
  g_by_val(clrefx); // copy param by cnt X(const X&)
  step();
  g_by_val(rrefx); // copy param by cnt X(const X&)
  step();

  g_by_val( fval() ); // f() call X(int), g() call nothing 
  step();
  g_by_val( flval() ); // f() call nothing, g() call cnt
  step();
  g_by_val( fclval() ); // f() call nothing, g() call cnt
  step();
  g_by_val( frval() ); // f() call X(X&&), g() call nothing
  step();

  std::cout<<"\nTEST g_by_lval()\n";

  g_by_lval(x); // do nothing
  step();
  g_by_lval(lrefx); // do nothing
  step();
  //g_by_lval(clrefx); // error binding X& to const X
  step();
  g_by_lval(rrefx); // do nothing
  step();
  
  //g_by_lval( fval() ); // error binding X& to rvalue X
  step();
  g_by_lval( flval() ); // do nothing
  step();
  //g_by_lval( fclval() ); // error binding X& to const X
  step();
  //g_by_lval( frval() ); // error binding X& to rvalue X
  step();
  
  std::cout<<"\nTEST g_by_clval()\n";

  g_by_clval(x); // do nothing
  step();
  g_by_clval(lrefx); // do nothing
  step();
  g_by_clval(clrefx); // do nothing
  step();
  g_by_clval(rrefx); // do nothing
  step();
  
  g_by_clval( fval() ); // f() call X(int), g() do nothing
  step();
  g_by_clval( flval() ); // do nothing
  step();
  g_by_clval( fclval() ); // do nothing
  step();
  g_by_clval( frval() ); // do nothing
  step();
  
  std::cout<<"\nTEST g_by_rval()\n";

  //g_by_rval(x); // cannot bind rvalue X&& to lvalue X
  //g_by_rval(lrefx); // cannot bind rvalue X&& to lvalue X
  //g_by_rval(clrefx); // cannot bind rvalue X&& to const lvalue X
  //g_by_rval(rrefx); // cannot bind rvalue X&& to const lvalue X

  g_by_rval( fval() ); // f() call X(int), g() do nothing
  step();
  //g_by_rval( flval() ); // cannot bind rvalue X&& to const lvalue X
  //g_by_rval( fclval() ); // cannot bind rvalue X&& to const lvalue X
  g_by_rval( frval() ); // do nothing
  step();
  
  return 0;
}
