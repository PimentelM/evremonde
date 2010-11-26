/**
 *  Resident Evil 5 STQ Dump JSON
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <json/json.h>

std::string ul_to_hex(unsigned long number)
{
    std::stringstream hex_buffer;
    hex_buffer << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << number;

    return hex_buffer.str();
}

std::string ul_to_hex_short(unsigned long number)
{
    std::stringstream hex_buffer;
    hex_buffer << std::hex << std::uppercase << number;

    return hex_buffer.str();
}

std::string ul_to_rev_hex(unsigned long number)
{
    std::string hex_buffer = ul_to_hex(number);

    std::stringstream rev_hex_buffer;
    rev_hex_buffer << hex_buffer.substr(6, 2); // << " ";
    rev_hex_buffer << hex_buffer.substr(4, 2); // << " ";
    rev_hex_buffer << hex_buffer.substr(2, 2); // << " ";
    rev_hex_buffer << hex_buffer.substr(0, 2);

    return rev_hex_buffer.str();
}

void write_file_contents(std::string filename, std::string file_contents)
{
    std::fstream file;
	file.open(filename.c_str(), std::ios::out);

    file << file_contents;

    file.close();
}

int main(int argc, char *argv[])
{
    std::cout << "Resident Evil 5 STQ Dump JSON" << std::endl;

    if (argc < 2)
    {
        std::cout << "Error: STQ file not found!" << std::endl;
        std::cout << "Usage: re5stqdumpjson filename" << std::endl;
        return 1;
    }

    std::string stq_filename(argv[1]);

    std::fstream stq_file;
    stq_file.open(stq_filename.c_str(), std::ios::in | std::ios::binary);

    if (!stq_file.good() || stq_file.fail())
    {
        std::cout << "Error: STQ file not good or failed!" << std::endl;
        return 1;
    }

    std::string stq_file_contents = std::string
    (
        std::istreambuf_iterator<char>(stq_file),
        std::istreambuf_iterator<char>()
    );

    std::cout << stq_filename << std::endl;

    stq_file.seekg(8, std::ios::beg);

    unsigned long stq_num_files = 0;
    stq_file.read((char*)&stq_num_files, 4);

    std::cout << "Number of files: " << stq_num_files << std::endl;
    std::cout << std::endl;

    std::vector<std::string>           v_stq_filenames;
    std::vector<std::string>::iterator v_stq_filenames_it;

    std::string::size_type filenames_position = stq_file_contents.find("sound");

    stq_file.seekg(filenames_position, std::ios::beg);

    std::stringstream bytes;

    char byte;
    while (!stq_file.eof())
    {
        stq_file >> byte;

        if (byte != '\0')
            bytes << byte;
        else
        {
            v_stq_filenames.push_back(bytes.str());

            bytes.clear();
            bytes.str("");
        }
    }

    stq_file.clear();

    for (v_stq_filenames_it = v_stq_filenames.begin(); v_stq_filenames_it != v_stq_filenames.end(); ++v_stq_filenames_it)
        std::cout << *v_stq_filenames_it << std::endl;

    std::stringstream stq_filename_out;
    stq_filename_out << stq_filename << ".json";

    std::cout << "Dumping data to " << stq_filename_out.str() << "...";

    Json::FastWriter j_writer;

    Json::Value j_root;
    j_root["filename"]        = stq_filename;
    j_root["number_of_files"] = (int)stq_num_files;

    Json::Value& j_files = (j_root["files"] = Json::Value());

    stq_file.seekg(68, std::ios::beg);

    unsigned int i = 0;
    for (i = 0; i < stq_num_files; i++)
    {
        Json::Value j_file;
        j_file["file_number"] = i + 1;
        j_file["filename"]    = v_stq_filenames[i];

        unsigned long duration = 0;
        stq_file.read((char*)&duration, 4);

        Json::Value& j_duration        = (j_file["duration"]   = Json::Value());
        Json::Value& j_duration_offset = (j_duration["offset"] = Json::Value());

        j_duration["dec"]     = (int)duration;
        j_duration["hex"]     = ul_to_hex(duration);
        j_duration["rev_hex"] = ul_to_rev_hex(duration);

        j_duration_offset["dec"] = (int)stq_file.tellg();
        j_duration_offset["hex"] = ul_to_hex_short(stq_file.tellg());

        stq_file.seekg(4, std::ios::cur); // unknown 4 bytes

        unsigned long loop_start = 0;
        stq_file.read((char*)&loop_start, 4);

        Json::Value& j_loop_start        = (j_file["loop_start"]   = Json::Value());
        Json::Value& j_loop_start_offset = (j_loop_start["offset"] = Json::Value());

        j_loop_start["dec"]     = (int)loop_start;
        j_loop_start["hex"]     = ul_to_hex(loop_start);
        j_loop_start["rev_hex"] = ul_to_rev_hex(loop_start);

        j_loop_start_offset["dec"] = (int)stq_file.tellg();
        j_loop_start_offset["hex"] = ul_to_hex_short(stq_file.tellg());

        unsigned long loop_end = 0;
        stq_file.read((char*)&loop_end, 4);

        Json::Value& j_loop_end        = (j_file["loop_end"]   = Json::Value());
        Json::Value& j_loop_end_offset = (j_loop_end["offset"] = Json::Value());

        j_loop_end["dec"]     = (int)loop_end;
        j_loop_end["hex"]     = ul_to_hex(loop_end);
        j_loop_end["rev_hex"] = ul_to_rev_hex(loop_end);

        j_loop_end_offset["dec"] = (int)stq_file.tellg();
        j_loop_end_offset["hex"] = ul_to_hex_short(stq_file.tellg());

        stq_file.seekg(4, std::ios::cur); // unknown 4 bytes

        stq_file.seekg(4, std::ios::cur); // unknown 4 bytes

        j_files.append(j_file);
    }

    std::string stq_file_output = j_writer.write(j_root);
	write_file_contents(stq_filename_out.str(), stq_file_output);

    std::cout << "Done!" << std::endl;

    stq_file.close();

    return 0;
}
