/*
    Lecture-Scheduler - A program made for showing info about lectures.
    Copyright (C) 2025 Emirhan Kotan
    Original work: https://github.com/Authlantech/Lecture-Scheduler

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License version 3 for more details.

    You should have received a copy of the GNU General Public License version 3
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

struct Lecture
{
    std::string code;
    int section;
    std::string weekday;
    unsigned long long start;
    unsigned long long end;
    std::string classroom;
};

std::vector<Lecture> lectures;

int main()
{

    char copyright_notice[400] =
        "Lecture-Scheduler Copyright (C) 2025  Emirhan Kotan\n"
        "Original work's source code: https://github.com/Authlantech/Lecture-Scheduler\n"
        "This program comes with ABSOLUTELY NO WARRANTY;\n"
        "This is free software, and you are welcome to redistribute it under certain conditions\n"
        "For more details see the GNU General Public License version 3\n";

    std::cout << copyright_notice << std::endl;

    std::string file_name;

    // Extract the text content in pdf file to a txt file :

    std::ifstream file;
    do
    {
        std::cout << "\nEnter file path: ";
        std::cin >> file_name;
        file.open(file_name);
    }while(file.is_open() == false);

    std::string command("pdftotext -raw " + file_name);
    system(command.c_str());
    file.close();

    file_name = file_name.substr(0,file_name.find_last_of(".")) + std::string(".txt");
    file.open(file_name);

    // Read txt file and extract lecture info :

    std::string line;
    char buffer[500];

    std::vector<std::string> words;
    std::string word;

    do
    {
        line.clear();
        memset(buffer,0,sizeof(buffer));
        words.clear();
        word.clear();

        file.getline(buffer,500);
        line = buffer;

        if(line.find("DERS PROGRAM") != std::string::npos || line.find("DERS KODU") != std::string::npos ) continue;

        std::istringstream iss(line);
        while(iss >> word)
        {
            words.push_back(word);
        }

        if(words.size() <= 4) continue;

        Lecture lecture;
        lecture.code = words[2] + ' ' + words[3];
        lecture.section = std::stoi(words[1]);
        lecture.weekday = words[4];
        lecture.start = std::stoi(words[5].substr(0,2 )) * 60 + std::stoi(words[5].substr(3,2));
        lecture.end = std::stoi(words[6].substr(0,2 )) * 60 + std::stoi(words[6].substr(3,2));
        for(int a = 7;a < words.size();a++)
        {
            lecture.classroom += words[a];
        }

        lectures.push_back(lecture);

    }while(!file.eof());
    file.close();

    // Go into a loop and ask for user directives :

    std::cout << "Data Loaded!\n";
    bool exit = false;

    /*
     * COMMANDS :
     *
     *
     */

    do
    {

    }while(!exit);

}