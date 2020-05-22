#include "du.hpp"

#include <filesystem>
#include <map>

#include <iostream>
ThreadPool _pool(std::thread::hardware_concurrency());

std::map<std::filesystem::path, uint8_t> _active;

std::mutex _active_mutex;
std::mutex _result_mutex;
size_t x() {return 0;}

const size_t base_unit = 1024; 


void summarise(const std::filesystem::path path, size_t& result)
{
    std::vector<std::filesystem::path> files;
    
    for (const auto& sub_path : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(sub_path))
        {
            {
                std::lock_guard<std::mutex> lock(_active_mutex);
                _active[path] = 1;
            }
            _pool.enqueue([sub_path, &result]()
                      {
                          summarise(sub_path, result); 
                      });
        }
        else
        {
           files.push_back(sub_path);
        }
    }

    size_t folder_result = std::filesystem::file_size(path);
    for (const auto file : files)
    {
        if (!std::filesystem::is_directory(file))
        {
            folder_result += std::filesystem::file_size(file);
        }
        // TODO: Added stat for dir
    }

    std::lock_guard<std::mutex> lock(_result_mutex);
    result += folder_result;
    _active.erase(path);

    
}

size_t summarise(const std::filesystem::path path, const char unit)
{
    size_t result = 0;
    std::vector<std::filesystem::path> files;
    for (const auto& sub_path : std::filesystem::directory_iterator(path))
    {
        if (std::filesystem::is_directory(sub_path))
        {
            _pool.enqueue([sub_path, &result]()
                          {
                              summarise(sub_path, result); 
                          });
        }
        else
        {
            files.push_back(sub_path);
        }
    }



    size_t folder_result = 0; // TODO: ADD STAT std::filesystem::file_size(path);
    
    for (const auto& file : files)
    {
        if (!std::filesystem::is_directory(file))
        {
            folder_result += std::filesystem::file_size(file);
        }
        // TODO: add stat for dir 
    }    

    {
        std::lock_guard<std::mutex> lock(_result_mutex);
        result += folder_result; 
    }

    while(!_active.empty())
    {
        std::cout << _active.size() << std::endl; 
       
    }


    // TODO: find a better way 
    if (unit == 'b')
    {
        return result; 
    }
    else if (unit == 'k')
    {
        return result / base_unit;
    }
    else if (unit == 'm')
    {
        return result / (base_unit * base_unit);
    }
    else if (unit == 'g')
    {
        return result / (base_unit * base_unit * base_unit);
    }
    
    
    return 0; 
}
