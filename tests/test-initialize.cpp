#include <statsig.h>

int main(int argc, char **argv) {
  statsig::Statsig statsig;
  statsig::Options options;
  statsig.initialize("secret-JbIloEJ4QmSif9cZ6nclNTIA7tXDlsC3raYKbk9f0Ma", options);

  statsig::User user;
  user.userID = "kenny";
  
  bool fail = statsig.checkGate(user, "disabled_gate");
  bool pass = statsig.checkGate(user, "kenny_only");

  std::cout << std::boolalpha;
  std::cout << "expect false: " << fail << std::endl;
  std::cout << "expect true: " << pass << std::endl;

  statsig::DynamicConfig testConfig = statsig.getConfig(user, "test_config");
  statsig::DynamicConfig testConfig2 = statsig.getConfig(user, "test_config_2");

  std::cout << "test_config: " << testConfig << std::endl;
  std::cout << "test_config_2: " << testConfig2 << std::endl;
  statsig.shutdown();
}
