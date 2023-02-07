#include<iostream>
#include<gtest/gtest.h>
#include<gmock/gmock.h>

class MylibInterface {
public:
  virtual ~MylibInterface() = default;
  virtual void setVoltage(int) = 0;
};

class Mylib : public MylibInterface {
public:
  virtual void setVoltage(int v) override{
    // complex logic
  }
};

class MylibMock : public MylibInterface {
public:
  ~MylibMock() override = default;
  MOCK_METHOD(void, setVoltage, (int v), (override));
};

class Myapp {
  MylibInterface *mylib_;

public:
  explicit Myapp(MylibInterface *mylib) : mylib_(mylib){};
  
  void run(const std::string& cmd) {
    if (cmd == "ON") {
      mylib_->setVoltage(220);
    } else if (cmd == "OFF") {
      mylib_->setVoltage(0);
    }
  }
};

TEST(MylibTestSuite, mock_mylib_setVoltage) {
  MylibMock mylib_mock;
  Myapp myapp_mock(&mylib_mock);

  EXPECT_CALL(mylib_mock, setVoltage(220)).Times(1);

  myapp_mock.run("ON");
};

int main(int argc, char* argv[]){
  ::testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
  /*
    
   */
}
