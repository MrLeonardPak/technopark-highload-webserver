#include "server/file.hh"

#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>

namespace server {

void FileSend(int aSocket, std::filesystem::path const& aPath) {
  auto fd = open(aPath.c_str(), O_RDONLY);
  if (fd == -1) {
    throw 500;
  }

  auto sizeToSend = std::filesystem::file_size(aPath);
  for (off_t offset = 0; sizeToSend > 0;) {
    auto res = sendfile(aSocket, fd, &offset, sizeToSend);
    if (res == -1) {
      throw 500;
    }
    sizeToSend -= res;
    offset += res;
  }
  close(fd);
}
}  // namespace server
