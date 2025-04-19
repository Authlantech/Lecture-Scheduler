/*
    Lecture-Scheduler - A program made for displaying lecture info.
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
#include <limits>
#include <regex>

enum weekdays
{
    monday,
    tuesday,
    wednesday,
    thursday,
    friday
};

class LectureTime
{
protected :
    unsigned int _hour;
    unsigned int _minute;
public:
    LectureTime(int hour,int minute) : _hour(hour), _minute(minute) {};
    LectureTime() {};
    ~LectureTime() {};

    unsigned int hour() const {return _hour;};
    unsigned int minute() const {return _minute;};

    bool operator==(const LectureTime &other) const
    {
        return this->_hour == other._hour && this->_minute == other._minute;
    }

    bool operator>(const LectureTime &other) const
    {
        return this->_hour > other._hour || (this->_hour == other._hour && this->_minute > other._minute);
    }

    bool operator<(const LectureTime &other) const
    {
        return this->_hour < other._hour || (this->_hour == other._hour && this->_minute < other._minute);
    }

    void operator=(const LectureTime &other)
    {
        this->_hour = other._hour;
        this->_minute = other._minute;
    }
};

struct Lecture
{
    std::string code;
    int section;
    std::string weekday;
    LectureTime begining;
    LectureTime ending;
    std::string classroom;
};

std::vector<Lecture> lectures;

int main()
{
    char copyright_notice[400] =
        "Lecture-Scheduler Copyright (C) 2025 Emirhan Kotan\n"
        "Original source code: https://github.com/Authlantech/Lecture-Scheduler\n"
        "This program comes with ABSOLUTELY NO WARRANTY.\n"
        "This is free software, and you are welcome to redistribute it\n"
        "under certain conditions which are stated in GNU General Public License version 3\nVisit: <https://www.gnu.org/licenses/gpl-3.0.html> for all details.\n";

    std::cout << copyright_notice << std::endl;

    std::string file_name;

    // Extract the text content in pdf file to a txt file :

    std::ifstream file;
    do
    {
        std::cout << "\n> Enter file path: " << std::flush;
        std::cin >> file_name;
        file.open(file_name);
        if(file.is_open() == false) std::cout << "\n> File could not be opened!" << std::flush;
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
        lecture.code = words[2] + words[3];
        lecture.section = std::stoi(words[1]);
        lecture.weekday = words[4];
        lecture.begining = LectureTime(std::stoi(words[5].substr(0,2 )), std::stoi(words[5].substr(3,2)));
        lecture.ending = LectureTime(std::stoi(words[6].substr(0,2 )) ,std::stoi(words[6].substr(3,2)));
        lecture.classroom = words[7];
        for(int a = 8;a < words.size();a++)
        {
            lecture.classroom += '_' + words[a];
        }
        lectures.push_back(lecture);

    }while(!file.eof());
    file.close();

    std::cout << "\n> File succesfully read!" << std::flush;

    /*
     *  COMMANDS :
     *
     *  (list lectures)
     *  > ll a                                      (list all of the lectures on a week)
     *  > ll  <code> ...  .                        (list all the lecture with given code)
     *  > ll  <code:section>... .                  (list all the lectures with given code and section)
     *
     *  (create program)
     *  > cp <code>:<section(optional)>... .        (create all possible weekly programs with the codes and (optionally) sections provided
     *                                              if you wish a specific section of a lecture you should add ':' after the lecture
     *                                              code and type de section you wish to put in your weekly program)
     *
     *                                              example usage : > cp MATH1100:5 MATH1132 .
     *                                              (this will generate all possible weekly programs with 5th section of MATH1100 and
     *                                              all sections of MATH1132)
     *
     * >
     */

    // Go into a loop and ask for user directives :

    bool exit = false;
    std::string input;

    do
    {
        std::cout << "\n> " << std::flush;
        std::cin >> input;

        if(input == "ll")
        {
            std::cin >> input;

            //Display all lectures :
            if(input == "a")
            {
                std::cout << "\nALL LECTURES\n------" << std::endl;
                for(auto& lecture : lectures)
                {
                    std::cout << lecture.code << " " << lecture.section << " " << lecture.classroom << " " << lecture.weekday
                            << " " << lecture.begining.hour() << ":" << lecture.begining.minute() << " - " << lecture.ending.hour()
                            <<":" << lecture.ending.minute() << std::endl;
                }
                std::cout << "\n------\nALL LECTURES";
            }

            // Display lectures with given parameters :
           else
           {
               std::vector<Lecture>desired_lectures;

               // Retrieve the parameters from user :

               std::vector<std::string> params;
               params.push_back(input);
               do
               {
                   std::cin >> input;
                   params.push_back(input);
               }while(input != ".");

               //Determine which lectures are asked to display :

               for(std::string& param : params)
               {
                   std::size_t pos = param.find(":");
                   //Parameter consists lecture code and section :
                   if(pos != std::string::npos)
                   {
                       try
                       {
                           // Retrieve the code of lecture :
                           std::string lecture_code = param.substr(0,pos);
                           //Retrieve the section of the lecture :
                           int section = std::stoi(param.substr(pos + 1));

                           // Search the lectures with given code and section :
                           for(Lecture l : lectures)
                           {
                               if(l.code == lecture_code && l.section == section)
                               {
                                   desired_lectures.push_back(l);
                               }
                           }
                       }

                       catch(std::invalid_argument const& ex)
                       {
                            std::cerr << std::endl << ex.what() << std::endl;
                       }

                       catch(std::out_of_range const& ex)
                       {
                          std::cerr << std::endl << ex.what() << std::endl;
                       }
                   }

                   // Parameter consists only lecture code :
                   else
                   {
                       try
                       {
                           // Retrieve the code of lecture :
                           std::string lecture_code = param.substr(0,pos);

                           // Search the lectures with given code and section :
                           for(Lecture l : lectures)
                           {
                               if(l.code == lecture_code)
                               {
                                   desired_lectures.push_back(l);
                               }
                           }
                       }

                       catch(std::out_of_range const& ex)
                       {
                           std::cerr << std::endl << ex.what() << std::flush;
                       }

                   }

               }

               //Display the specified lectures :

               for(Lecture&dl : desired_lectures)
               {
                   std::cout << std::endl << dl.code << " " << dl.section << " " << dl.classroom << " " <<
                       dl.weekday << " " << dl.begining.hour() << ":" << dl.begining.minute() <<
                           " " << dl.ending.hour() << ":" << dl.ending.minute() << std::endl;
               }
               desired_lectures.clear();
           }
        }

        else
        {
            std::cout << "\n> Invalid command!\n";
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    }while(!exit);
}
