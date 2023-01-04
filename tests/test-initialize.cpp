#include <statsig.h>

int main(int argc, char **argv) {
  statsig::Statsig statsig;
  statsig::Options options;
  statsig.initialize("secret-JbIloEJ4QmSif9cZ6nclNTIA7tXDlsC3raYKbk9f0Ma", options);
  statsig::User user;
  user.userID = "kenny";
  bool fail = statsig.checkGate(user, "disabled_gate");
  std::cout << fail << std::endl;
  bool pass = statsig.checkGate(user, "kenny_only");
  std::cout << pass << std::endl;
  statsig.shutdown();

  // std::multimap<std::string, JSON::any> body = {
  //     {"statsigMetadata", statsig::StatsigMetadata},
  //     {"sinceTime", 0},
  // };
  // std::string bodyString = statsig::Utils::mapToJsonString<std::string, JSON::any>(body);
  // std::cout << bodyString << std::endl;
}
