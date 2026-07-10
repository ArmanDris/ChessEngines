#include <filesystem>
#include <mach-o/dyld.h>
#include <stdexcept>

namespace fs = std::filesystem;

fs::path get_binary_path() {
  uint32_t size = 0;

  _NSGetExecutablePath(nullptr, &size);

  std::string buffer(size, '\0');

  if (_NSGetExecutablePath(buffer.data(), &size) != 0) {
    throw std::runtime_error("Failed to get executable path");
  }

  return fs::canonical(buffer).parent_path();
}
