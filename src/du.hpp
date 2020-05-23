#include <ThreadPool.h>

#include <cstdint>
#include <queue>
#include <map>

#include <filesystem>

#include <thread>
#include <mutex>

const size_t BASE_UNIT = 1024;

void _find_files(const std::filesystem::path& base, std::queue<std::filesystem::path>& paths);

std::uintmax_t _directory_size(std::filesystem::path path);

void summarise(std::queue<std::filesystem::path>& paths, std::uintmax_t& result,
               const uint8_t thread_id, std::mutex& path_mutex, std::mutex& result_mutex, std::map<uint8_t, bool>& complete);

std::uintmax_t summarise(const std::filesystem::path, char unit='b');


    
