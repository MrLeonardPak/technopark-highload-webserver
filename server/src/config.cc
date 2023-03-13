#include "server/config.hh"

#include <fstream>

namespace server {

std::unordered_map<std::string, std::string> ConfigGen(std::string_view path) {
  std::ifstream ifs(path.data());
  std::unordered_map<std::string, std::string> res;
  while (ifs) {
    std::string key, val;
    ifs >> key >> val;
    res.insert_or_assign(key, val);
  }
  return res;
}

template <>
std::string Config(std::string_view key) {
  static auto configMap = ConfigGen(CONFIG_PATH);
  return configMap[key.data()];
}

template <>
int Config(std::string_view key) {
  static auto configMap = ConfigGen(CONFIG_PATH);
  return std::stoi(configMap[key.data()]);
}

}  // namespace server
