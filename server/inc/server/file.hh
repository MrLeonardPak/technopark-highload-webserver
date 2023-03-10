#ifndef SERVER_FILE_HH
#define SERVER_FILE_HH

#include <filesystem>

namespace server {

void FileSend(int aSocket, std::filesystem::path const& aPath);

}  // namespace server

#endif  // SERVER_FILE_HH
