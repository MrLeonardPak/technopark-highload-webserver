#include "server/file.hh"

#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>

#include "spdlog/spdlog.h"

namespace server {

void FileSend(int aSocket, std::filesystem::path const& aPath) {
  auto fd = open(aPath.c_str(), O_RDONLY);
  if (fd == -1) {
    goto exit;
  }

  {
    auto sizeToSend = std::filesystem::file_size(aPath);
    for (off_t offset = 0; sizeToSend > 0;) {
      auto res = sendfile(aSocket, fd, &offset, sizeToSend);
      if (res == -1) {
        if (errno == EPIPE) {
          spdlog::debug("EPIPE sendfile");
          goto exit;
        }
        throw std::runtime_error("Sendfile error: " + std::to_string((errno)));
      }
      sizeToSend -= res;
      offset += res;
    }
  }

exit:
  close(fd);
}
}  // namespace server
