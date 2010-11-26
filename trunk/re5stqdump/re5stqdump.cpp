/**
 *  Resident Evil 5 STQ Dump
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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

int main(int argc, char *argv[])
{
    std::cout << "Resident Evil 5 STQ Dump" << std::endl;

    if (argc < 2)
    {
        std::cout << "Error: STQ file not found!" << std::endl;
        std::cout << "Usage: re5stqdump filename" << std::endl;
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
    stq_filename_out << stq_filename << ".txt";

    std::cout << "Dumping data to " << stq_filename_out.str() << "...";

    std::fstream output_file;
    output_file.open(stq_filename_out.str().c_str(), std::ios::out);

    output_file << stq_filename << std::endl;

    output_file << "Number of files: " << stq_num_files << std::endl;
    output_file << std::endl;

    stq_file.seekg(68, std::ios::beg);

    unsigned int i = 0;
    for (i = 0; i < stq_num_files; i++)
    {
        output_file << i + 1 << ":" << std::endl;

        output_file << "Filename:   " << v_stq_filenames[i] << std::endl;

        output_file << std::endl;

        output_file << "Offset:     " << stq_file.tellg()                  << " dec" << std::endl
                    << "            " << ul_to_hex_short(stq_file.tellg()) << " hex" << std::endl;

        unsigned long duration = 0;
        stq_file.read((char*)&duration, 4);

        output_file << "Duration:   " << duration                << " dec"     << std::endl
                    << "            " << ul_to_hex(duration)     << " hex"     << std::endl
                    << "            " << ul_to_rev_hex(duration) << " rev hex" << std::endl;

        output_file << std::endl;

        stq_file.seekg(4, std::ios::cur); // unknown 4 bytes

        output_file << "Offset:     " << stq_file.tellg()                  << " dec" << std::endl
                    << "            " << ul_to_hex_short(stq_file.tellg()) << " hex" << std::endl;

        unsigned long loop_start = 0;
        stq_file.read((char*)&loop_start, 4);

        output_file << "Loop Start: " << loop_start                << " dec"     << std::endl
                    << "            " << ul_to_hex(loop_start)     << " hex"     << std::endl
                    << "            " << ul_to_rev_hex(loop_start) << " rev hex" << std::endl;

        output_file << std::endl;

        output_file << "Offset:     " << stq_file.tellg()                  << " dec" << std::endl
                    << "            " << ul_to_hex_short(stq_file.tellg()) << " hex" << std::endl;

        unsigned long loop_end = 0;
        stq_file.read((char*)&loop_end, 4);

        output_file << "Loop End:   " << loop_end                << " dec"     << std::endl
                    << "            " << ul_to_hex(loop_end)     << " hex"     << std::endl
                    << "            " << ul_to_rev_hex(loop_end) << " rev hex" << std::endl;

        stq_file.seekg(4, std::ios::cur); // unknown 4 bytes

        stq_file.seekg(4, std::ios::cur); // unknown 4 bytes

        output_file << std::endl;
    }

    std::cout << "Done!" << std::endl;

    output_file.close();

    stq_file.close();

    return 0;
}
