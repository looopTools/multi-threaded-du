#include "du.hpp"

#include <chrono>
#include <iostream>
#include <cstdlib>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void _find_files(const std::filesystem::path& base, std::queue<std::filesystem::path>& paths)
{
    paths.push(base); 
    for (const auto& entry : std::filesystem::directory_iterator(base))
    {
        if (std::filesystem::is_directory(entry))
        {
            _find_files(entry, paths);
        }
        else
        {
            paths.push(entry);
        }
    }
}

std::uintmax_t _directory_size(std::filesystem::path path)
{
    FILE* fp = NULL;

    fp = fopen(path.string().c_str(), "r");
    if (fp == NULL)
    {
        std::cout << "Unable to get size of direcotry file: " << path.string() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    int fd = fileno(fp);
    struct stat buf;
    fstat(fd, &buf);
    auto result = static_cast<std::uintmax_t>(buf.st_size);
    if (fclose(fp) != 0)
    {
        std::cout << "Unable to close directory file: " << path.string() << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return result; 
    
}

void summarise(std::queue<std::filesystem::path>& paths, std::uintmax_t& result,
               const uint8_t thread_id, std::mutex& path_mutex, std::mutex& result_mutex, std::map<uint8_t, bool>& complete)
{

    std::vector<std::filesystem::path> t_paths;
    {
        std::lock_guard<std::mutex> lock(path_mutex);
        if (paths.size() == 0)
        {
            complete[thread_id] = true;
        }
        else if (paths.size() >= 4)
        {
            for (uint8_t i = 0; i < 4; ++i)
            {
                t_paths.push_back(paths.front());
                paths.pop();
            }
            
        }
        else
        {
            for (uint8_t i = 0; i < paths.size(); ++i)
            {
                t_paths.push_back(paths.front());
                paths.pop();
            }
        }
    }

    if (t_paths.size() == 0)
    {
        return; 
    }

    std::uintmax_t local_result = 0;

    for (uint8_t i = 0; i < t_paths.size(); ++i)
    {
        if (!std::filesystem::is_directory(t_paths.at(i)))
        {
            local_result += std::filesystem::file_size(t_paths.at(i));
        }
        else
        {
            local_result += _directory_size(t_paths.at(i));
            // implement stat check
        }
    }

    {
        std::lock_guard<std::mutex> lock(result_mutex);
        result += local_result;
    }
    {
        std::lock_guard<std::mutex> lock(path_mutex);
        if (paths.size() == 0)
        {
            complete[thread_id] = true;
        }
    }

    bool all_complete = true;

    for (auto it = complete.begin(); it != complete.end(); ++it)
    {
        if (it->second == false)
        {
            all_complete = false;
            break; 
        }
    }
    summarise(paths, result, thread_id, path_mutex, result_mutex, complete);    
}




size_t summarise(const std::filesystem::path base, const char unit)
{

    std::queue<std::filesystem::path> paths;
    _find_files(base, paths);

    
    ThreadPool pool(std::thread::hardware_concurrency());

    std::mutex path_mutex;
    std::mutex result_mutex; 
    
    std::map<uint8_t, bool> complete;  
        
    std::uintmax_t result = 0;

    for (uint8_t i = 0; i < std::thread::hardware_concurrency(); ++i)
    {
        complete[i] = false; 
        pool.enqueue([&paths, &result, i, &path_mutex, &result_mutex, &complete](){
                         summarise(paths, result, i, path_mutex, result_mutex, complete);
                     });
    }

    bool all_complete = false;
    while(!all_complete)
    {
        bool _complete = true;
        for (auto it = complete.begin(); it != complete.end(); ++it)
        {
            if (it->second == false)
            {
                _complete = false;
                break; 
            }
        }
        if (!_complete)
        {
            std::this_thread::sleep_for(std::chrono::microseconds{100}); 
        }
        all_complete = _complete;            
    }
    
    // TODO: find a better way 
    if (unit == 'b')
    {
        return result; 
    }
    else if (unit == 'k')
    {
        return result / BASE_UNIT;
    }
    else if (unit == 'm')
    {
        return result / (BASE_UNIT * BASE_UNIT);
    }
    else if (unit == 'g')
    {
        return result / (BASE_UNIT * BASE_UNIT * BASE_UNIT);
    }
    
    return 0; 
}
