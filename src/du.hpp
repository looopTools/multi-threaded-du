#include <ThreadPool.h>

#include <cstdint>
#include <filesystem>

#include <thread>
#include <mutex>

size_t x();

void summarise(const std::filesystem::path, size_t& result);
size_t summarise(const std::filesystem::path, char unit='b');

    
