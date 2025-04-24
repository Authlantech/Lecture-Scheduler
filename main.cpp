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
#include <map>
#include <string>
#include <sstream>
#include <limits>
#include <regex>
#include <algorithm>
#include <utility>

// Program user-defined datatypes 
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

    static bool check_conflict(const LectureTime& b1, const LectureTime& e1, const LectureTime& b2, const LectureTime& e2)
    {
        if (b2 > e1 || e2 < b1) return false;
        else return true;
    }
};

struct lecture
{
    std::string code;
    int section;
    std::string weekday;
    LectureTime begining;
    LectureTime ending;
    std::string classroom;
};

//Program functions
std::vector<lecture> term_lectures;

const std::vector<lecture> find_lectures(std::vector<std::string>lecture_data)
{
    std::vector<lecture>desired_lectures;
    for(std::string& param : lecture_data)
    {
        std::vector<lecture> lecture_group;
        std::size_t pos = param.find(":");
        //Parameter consists course code and section :
        if(pos != std::string::npos)
        {
            try
            {
                // Retrieve the course code of lecture :
                std::string lecture_code = param.substr(0,pos);
                // Retrieve the section of the lecture :
                int section = std::stoi(param.substr(pos + 1));

                // Search the lectures with given course code and section :
                for(lecture l : term_lectures)
                {
                    if(l.code == lecture_code && l.section == section)
                    {
                        lecture_group.push_back(l);
                    }
                }
            }

            catch(std::invalid_argument const& ex)
            {
                std::cerr << std::endl << param << " : ";
                std::cerr<< ex.what() << std::endl;
            }

            catch(std::out_of_range const& ex)
            {
                std::cerr << std::endl << param << " : ";
                std::cerr << ex.what() << std::endl;
            }
        }

        // Parameter consists only course code :
        else
        {
            try
            {
                // Retrieve the course code of lecture :
                std::string lecture_code = param.substr(0,pos);

                // Search the lectures with given course code and section :
                for(lecture l : term_lectures)
                {
                    if(l.code == lecture_code)
                    {
                        lecture_group.push_back(l);
                    }
                }
            }

            catch(std::out_of_range const& ex)
            {
                std::cerr << std::endl << param << " : ";
                std::cerr << ex.what() << std::endl;
            }

        }

        if(lecture_group.size() > 0)
        {
            std::sort(lecture_group.begin(), lecture_group.end(),[](const lecture& l1,const lecture& l2) {return l1.section < l2.section;});
            for(auto l : lecture_group) desired_lectures.push_back(l);
        }

    }

    return desired_lectures;
}

int main()
{ 
    constexpr char copyright_notice[500] =
        "Lecture-Scheduler Copyright (C) 2025 Emirhan Kotan\n"
        "Original source code: https://github.com/Authlantech/Lecture-Scheduler\n"
        "This program comes with ABSOLUTELY NO WARRANTY.\n"
        "This is free software, and you are welcome to redistribute it\n"
        "under certain conditions which are stated in GNU General Public License version 3"
        "\nVisit: <https://www.gnu.org/licenses/gpl-3.0.html> for all details.\n";

    std::cout << copyright_notice << std::endl;

    /*
     *  COMMANDS :
     *
	 *  > exit                                      (exit the program)
     *  > rd <filename>                             (read the lecture database from pdf file)    
     * 
     * 
     *  > ll a                                      (list all the lectures on a week)
     *  > ll  <course code> ...  .                  (list all the lecture with given course code)
     *  > ll  <course code:section>... .            (list all the lectures with given course code and section)
     *  
     */

    // Go into a loop and ask for user directives :

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
                std::cout << "\n\nALL LECTURES\n------" << std::endl;
                for(auto& lecture : term_lectures)
                {
                    std::cout << lecture.code << " " << lecture.section << " " << lecture.classroom << " " << lecture.weekday
                            << " " << lecture.begining.hour() << ":" << lecture.begining.minute() << " - " << lecture.ending.hour()
                            <<":" << lecture.ending.minute() << std::endl;
                }
                std::cout << "\n------\nALL LECTURES\n\n";
            }

           // Display lectures with given parameters :
           else
           {
               // Retrieve the parameters from user :

               std::vector<std::string> params;
               params.push_back(input);
               do
               {
                   std::cin >> input;
                   params.push_back(input);
               }while(input != ".");

               // Retrieve the lectures from database :
               std::vector<lecture> lectures = find_lectures(params);

               //Display the lecture info :

               std::cout << "\n\n";
               for(auto& lecture : lectures)
               {
                   std::cout << lecture.code << " " << lecture.section << " " << lecture.classroom << " " << lecture.weekday
                   << " " << lecture.begining.hour() << ":" << lecture.begining.minute() << " - " << lecture.ending.hour()
                   <<":" << lecture.ending.minute() << std::endl;
               }
               std::cout << "\n\n";
           }
        }        

        else if (input == "rd")
        {
            // Extract the lecture database from pdf file :

            term_lectures.clear();
            std::string file_name;
            std::ifstream file;
            do
            {
                std::cin >> file_name;
                file.open(file_name);
                if (file.is_open() == false) std::cout << "\n\n> File could not be opened!\n\n> Re-enter file name: " << std::flush;
            } while (file.is_open() == false);

            std::string command("pdftotext -raw " + file_name);
            system(command.c_str());
            file.close();

            file_name = file_name.substr(0, file_name.find_last_of(".")) + std::string(".txt");
            file.open(file_name);

            // Read txt file and extract lecture info :

            std::string line;
            char buffer[500];

            std::vector<std::string> words;
            std::string word;

            do
            {
                line.clear();
                memset(buffer, 0, sizeof(buffer));
                words.clear();
                word.clear();

                file.getline(buffer, 500);
                line = buffer;

                if (line.find("DERS PROGRAM") != std::string::npos || line.find("DERS KODU") != std::string::npos) continue;

                std::istringstream iss(line);
                while (iss >> word)
                {
                    words.push_back(word);
                }

                if (words.size() <= 4) continue;

                lecture lecture;
                lecture.code = words[2] + words[3];
                lecture.section = std::stoi(words[1]);
                lecture.weekday = words[4];
                lecture.begining = LectureTime(std::stoi(words[5].substr(0, 2)), std::stoi(words[5].substr(3, 2)));
                lecture.ending = LectureTime(std::stoi(words[6].substr(0, 2)), std::stoi(words[6].substr(3, 2)));
                lecture.classroom = words[7];
                for (int a = 8; a < words.size(); a++)
                {
                    lecture.classroom += '_' + words[a];
                }
                term_lectures.push_back(lecture);

            } while (!file.eof());
            file.close();

            std::cout << "\n\n> File succesfully read!\n\n" << std::flush;
        }       

		else if (input == "help")
		{
			std::cout << "\n\n> Available commands :\n"
				<< "  > ll a                                               List all the lectures\n"			
				<< "  > ll <course code:section(optional)> ...  <.>        List the lectures with given course code and (optionally) section\n"
				<< "  > exit                                               exit the app\n"
				<< "  > rd <filename>                                      read the lecture database from pdf file\n"
				<< "  > clear                                              clear the console\n\n" << std::flush;
		}

        else if (input == "clear")
        {
            system("cls");
            std::cout << copyright_notice << "\n\n";
        }      

        else if (input == "exit")
        {
            return EXIT_SUCCESS;
        }

        else
        {
            std::cout << "\n\n> Invalid command!\n\n" << std::flush;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    }while(!exit);
}
