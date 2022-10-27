#include "runclasses.h"
#include "readers.h"
#include "timestamp.h"

#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>

// Test implementation to  get collection of basic runs:

// getCollection() goes through folder with .tcx files and returns a vector of BasicRuns wth ID between the two times.
std::vector<BasicRun> getCollection(const TimeStamp& stamp1, const TimeStamp& stamp2)
{
    std::vector<BasicRun> collection_vec;

    // directory as argument to getCollection() is probably better
    std::filesystem::path directory { "/users/erikhabbestad/cpp_prosjekter/RunData/ctx_files" }; 

    for(auto entry : std::filesystem::directory_iterator(directory))
    {
        std::string path_str = std::filesystem::absolute( entry.path() ).string();
        if(path_str.ends_with( "tcx" )) // ignore files that are not .tcx
        {
            std::ifstream infile { entry };
            if( !TimeStamp(getData(infile, "Id", "Id", true)).isBefore(stamp1) 
                &&  TimeStamp( getData(infile, "Id", "Id", true) ).isBefore(stamp2) )
            {
                BasicRun basic_run (infile);
                collection_vec.push_back(basic_run);
            }
        }
    }

    std::sort(collection_vec.begin(), collection_vec.end(), [](const BasicRun& r1, const BasicRun& r2){ 
                                                        return TimeStamp(r1.getId()).isBefore(TimeStamp(r2.getId())); } ); 

    return collection_vec;
}

int main()
{

    // Testing getCollection()

    std::vector<BasicRun> basicRunVec = getCollection(TimeStamp("2022-01-01",0), TimeStamp("2022-08-28",0));

    for(const auto& br : basicRunVec)
    {
        br.print();
    }

    // it works!

    return 0;
}