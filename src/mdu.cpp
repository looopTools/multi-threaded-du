#include "du.hpp"
#include <string>

#include <filesystem>

#include <iostream>
int main(int argc, char** argv)
{
    std::cout << argc << std::endl;

    char function;
    char option;
    std::string dir; 

    if (argc < 2)
    {
        std::cout << "You must provide at least a directory" << std::endl;
        return -1; 
    }
    if (argc == 2)
    {
        dir = std::string(argv[1]);
    }

    if (argc > 2)
    {
        if (std::string(argv[2]).size() > 3)
        {
            std::cout << "provide a function + 0 or 1 option" << std::endl;
            return -1; 
        }
        for (const auto& c : std::string(argv[2]))
        {
            if (std::string(argv[1]).size() == 3)
            {
                function = argv[1][1];
                option = argv[1][2];                
            }
            else if (std::string(argv[1]).size() == 2)
            {
                char tmp = argv[1][0];
                if (tmp == '-')
                {
                    function = argv[1][1];
                    option = 'b';  
                }
                else
                {
                    function = argv[1][0];                    
                    option = argv[1][1];  
                }
                
                
            }
            else
            {
                function = argv[1][0];
                option = 'b';  
            }
        }
        dir = std::string(argv[argc - 1]);
    }

    std::cout << "du on: " << dir << " with function " << function << " and option " << option << std::endl;

    if (function == 's')
    {
        std::cout << summarise(std::filesystem::path(dir), option) << std::endl; 
        
    }

    
        
    return 0; 
}
