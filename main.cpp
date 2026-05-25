#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "Student.hpp"

using namespace std;

char getLetterGrade(double average)
{
    if (average >= 90) return 'A';
    else if (average >= 80) return 'B';
    else if (average >= 70) return 'C';
    else if (average >= 60) return 'D';
    else return 'F';
}

void updateDistribution(char grade, int& a, int& b, int& c, int& d, int& f)
{
    if (grade == 'A') a++;
    else if (grade == 'B') b++;
    else if (grade == 'C') c++;
    else if (grade == 'D') d++;
    else f++;
}

void printSection(ostream& out, Student** list, int size, string course)
{
    out << "\n" << course << " Students\n";
    out << "------------------------------------------------------------\n";
    out << left << setw(25) << "Name"
        << setw(15) << "Final Exam"
        << setw(15) << "Average"
        << "Grade\n";

    for (int i = 0; i < size; i++)
    {
        if (list[i]->getCourse() == course)
        {
            list[i]->printReport(out);
            out << getLetterGrade(list[i]->computeFinalAverage()) << endl;
        }
    }
}

int main()
{
    string inputFileName;
    string outputFileName;

    cout << "Enter input file name: ";
    cin >> inputFileName;

    cout << "Enter output file name: ";
    cin >> outputFileName;

    ifstream in(inputFileName);
    ofstream out(outputFileName);

    if (!in)
    {
        cout << "Error opening input file.\n";
        return 1;
    }

    if (!out)
    {
        cout << "Error opening output file.\n";
        return 1;
    }

    int size;
    in >> size;
    in.ignore();

    Student** list = new Student*[size];

    for (int i = 0; i < size; i++)
    {
        string nameLine;
        string course;

        getline(in, nameLine);

        int commaPosition = nameLine.find(',');
        string lastName = nameLine.substr(0, commaPosition);
        string firstName = nameLine.substr(commaPosition + 2);

        in >> course;

        if (course == "Biology")
        {
            int lab, t1, t2, t3, finalExam;
            in >> lab >> t1 >> t2 >> t3 >> finalExam;

            list[i] = new BiologyStudent(firstName, lastName,
                                         lab, t1, t2, t3, finalExam);
        }
        else if (course == "Theater")
        {
            int participation, midterm, finalExam;
            in >> participation >> midterm >> finalExam;

            list[i] = new TheaterStudent(firstName, lastName,
                                         participation, midterm, finalExam);
        }
        else if (course == "Computer")
        {
            string secondWord;
            in >> secondWord;

            int programs[6];
            int test1, test2, finalExam;

            for (int j = 0; j < 6; j++)
                in >> programs[j];

            in >> test1 >> test2 >> finalExam;

            list[i] = new ComputerScienceStudent(firstName, lastName,
                                                 programs, test1, test2, finalExam);
        }

        in.ignore();
    }

    int countA = 0, countB = 0, countC = 0, countD = 0, countF = 0;

    for (int i = 0; i < size; i++)
    {
        char grade = getLetterGrade(list[i]->computeFinalAverage());
        updateDistribution(grade, countA, countB, countC, countD, countF);
    }

    out << "Student Grade Summary Report\n";

    printSection(out, list, size, "Biology");
    printSection(out, list, size, "Theater");
    printSection(out, list, size, "Computer Science");

    out << "\nGrade Distribution\n";
    out << "A: " << countA << endl;
    out << "B: " << countB << endl;
    out << "C: " << countC << endl;
    out << "D: " << countD << endl;
    out << "F: " << countF << endl;

    for (int i = 0; i < size; i++)
        delete list[i];

    delete[] list;

    return 0;
}