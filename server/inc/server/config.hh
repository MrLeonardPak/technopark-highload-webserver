#ifndef SERVER_CONFIG_HH
#define SERVER_CONFIG_HH

#include <string>
#include <string_view>
#include <unordered_map>

namespace server {

using config_t = std::unordered_map<std::string, std::string>;

std::string_view constexpr CONFIG_PATH = "/etc/httpd.conf";

config_t ConfigGen(std::string_view path);

template <typename T>
extern T Config(std::string_view key);

}  // namespace server

#endif  // SERVER_CONFIG_HH
