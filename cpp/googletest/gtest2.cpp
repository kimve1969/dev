#include<iostream>
#include<gtest/gtest.h>

TEST(TestGroupName, Subtest_1){
  ASSERT_TRUE(1==1);
}

TEST(TestGroupName, Subtest_2){
  ASSERT_FALSE('b' == 'b');
  std::cout<<"continue test after failure"<<std::endl;
}

int main(int argc, char* argv[]){
  ::testing::InitGoogleTest(&argc, argv);

  //return RUN_ALL_TESTS();
  int res = RUN_ALL_TESTS();
  std::cout<<"code result of RUN_ALL_TESTS() is "<<res<<"\n";
  return 0;
}
