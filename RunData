#ifndef READERS_H
#define READERS_H

#include <fstream>
#include <string>
#include <vector>

// Almost every line in a .ctx file starts with some whitespace followed by "<xyz>". headerMatch() checks if a string (line) is of this form and
// if header == "xyz". 
bool headerMatch(const std::string& line, const std::string& header);

// This function throws away input until it finds the header.
void ignoreUntil(std::ifstream& infile, const std::string& header);

// The pieces of data in a .ctx file are written as "<data_header>PIECE_OF_DATA</data_header>" where data_header is a string describing the
// type of data (e.g. "Time", "DistanceMeters").  The function getData() ignores everything until segment_header (typically "Trackpoint", or "Lap")
// then finds the next occurence of data_header and returns the PIECE_OF_DATA (as a string) indicated by the data_header. In addition one can 
// return the ifstream where it started by choosing true (bool) as the last parameter. 
std::string getData(std::ifstream& infile, const std::string& data_header, 
            const std::string& segment_header = "Activities", 
            bool rewind = false);


#endif