#include "readers.h"

#include <fstream>
#include <string>
#include <vector>
#include <numeric>

bool headerMatch(const std::string& line, const std::string& header)
{
    return line.substr(line.find_first_not_of(' ')+1, header.size()) == header;
}

// This function throws away input until it finds the header.
void ignoreUntil(std::ifstream& infile, const std::string& header)
{
    std::string line;
    std::streampos prev { infile.tellg() };
    while(std::getline(infile,line))        // read line by line
    {
        if(headerMatch(line,header))   
        {
            infile.seekg(prev);
            return;                         // if match is found we stop (so the stream is currently on the next line)
        }

        prev = infile.tellg();
    }
}

std::string getData(std::ifstream& infile, const std::string& data_header,
                    const std::string& segment_header /* ="Activities"*/, 
                    bool rewind /* = false*/)
{
    auto sz_data_header { data_header.size() + 2 };          // length of the string "<data_name>"
    std::string a_string;

    auto initial_stream_position = infile.tellg();
    ignoreUntil(infile,segment_header);

    while(std::getline(infile,a_string))                      // Go through each line of the file..
    {   
        if(headerMatch(a_string, '/' + segment_header))       // ..but stop and return "NAN" if you reached the end of the segment
        {
            return "NAN";
        }
        else if(headerMatch(a_string,data_header))            // ..or stop and return wanted piece of data
        {
            if(rewind){ infile.seekg(initial_stream_position); };

            auto data_start_pos { a_string.find_first_not_of(' ') + sz_data_header }; // start position of data substring
            auto data_length { a_string.size() - data_start_pos - sz_data_header - 1}; // length of data substring
            return a_string.substr(data_start_pos, data_length); // this substring is now only PIECE_OF_DATA
        }

    }
    return "Done";
}
