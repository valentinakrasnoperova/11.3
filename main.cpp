//1. Програма має дати користувачеві можливість фізично впорядкувати файл в першу
//чергу – за номером курсу, в другу чергу (для однакових курсів) – за значенням оцінки
//  з третього – профільного – предмету, в останню чергу (для однакових курсів і оцінок з
// третього предмету) – за прізвищем за спаданням – в зворотному до алфавітного
//порядку.
//2. Програма має будувати індексний файл, який забезпечує наступне індексне
//впорядкування: в першу чергу – за номером курсу, в другу чергу (для однакових
// курсів) – за значенням оцінки з третього – профільного – предмету, в останню чергу
//(для однакових курсів і оцінок з третього предмету) – за прізвищем.
//3. За допомогою бінарного пошуку визначити, чи навчається студент із вказаним
// користувачем прізвищем на вказаному курсі та вказаною оцінкою з третього
//предмету.
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstring>

using namespace std;

enum Specialty { KN, INF, ME, PI, TD };

string strSpecialty[] = { "Комп'ютерні науки", "Інформатика", "Математика та екноміка", " Фізика та інформатика", "Трудове навчання" };

struct Student {
    char surname[100];
    unsigned course;
    Specialty specialty;
    unsigned physics_grade;
    unsigned math_grade;
    unsigned inf_grade;
};

void create(const char* fName);
void print
(const char* fName);
void fWrite(fstream& f, const int i, Student x);
Student fRead(fstream& f, const int i);
void fChange(fstream& f, const int i, const int j);
void physical_sort(const char* fName);
void index_sort
(const char* fName, const char* gName);
void index_print

(const char* fName, const char* gName);
int binarySearch(const char* fName, const char* surname, const int course, const int inf_grade);


int main()
{
    char f[100];
    char g[6] = "g.dat";
    char surname[100];
    int course, inf_grade, binNum;

    int actions;
    do {
        cout << "Обрати дію з меню: " << endl;
        cout << "[1]  Ввести дані" << endl;
        cout << "[2]  Показати дані на екрані" << endl;
        cout << "[3]  Фізичне впорядкування" << endl;
        cout << "[4]  Індексне сортування та вивід даних " << endl;
        cout << "[5]  Бінарний пошук" << endl;
        cout << "[0]  Завершити роботу" << endl << endl;
        cout << "Вхідні дані: "; cin >> actions;
        switch (actions) {
            case 1:
                create(f);
                break;
            case 2:
                print(f);
                break;
            case 3:
                physical_sort(f);
                break;
            case 4:
                index_sort(f, g);
                index_print(f, g);
                break;
            case 5:
                cout << "Пошук студента за прізвищем, курсом і  оцінкою з третього предмету";
                cout << "Прізвище студента: "; cin >> surname;
                cout << "Курс студента: "; cin >> course;
                cout << "Оцінка з інформатики: "; cin >> inf_grade;
                binNum = binarySearch(f, surname, course, inf_grade);
                if (binNum == -1) {
                    cout << "Студента не знайдено " << endl;
                }
                else {
                    cout << "Номер студента в списку: " << binNum << endl;
                }
                break;
            case 0:
                break;
            default:
                cout << "Ви ввели неіснуюче значення" << endl;
        }
    } while (actions != 0);



    cin.get();
    return 0;
}

void create(const char* fName)
{
    ofstream f(fName, ios::binary);
    Student student;
    char ch;
    int specialty;
    do {

        cout << "Прізвище: ";
        cin >> student.surname;
        cout << "Курс: "; cin >> student.course;
        cout << "Спеціальність (0 - Комп'ютерні науки, 1 - Інформатика, 2 - Математика та екноміка, 3 - Фізика та інформатика, 4 - Трудове навчання): ";
        cin >> specialty;
        student.specialty = (Specialty)specialty;
        cout << "Оцінка з фізики: "; cin >> student.physics_grade;
        cout << "Оцінка з математики: "; cin >> student.math_grade;
        cout << "Оцінка з інформатики: "; cin >> student.inf_grade;
        cout << endl;
        if (!f.write((char*)&student, sizeof(Student))) {
            cerr << "Error writing file." << endl;
        }
        cout << "Continue or stop?(C/S)"; cin >> ch;
    } while (ch == 'c' || ch == 'C');
    f.close();
}

void print
(const char* fName)
{
    cout << endl << "\nfile \"" << fName << "\":\n";
    cout << "========================================================================================================================================" << endl;
    cout << "|  No  |    Прізвище    |  Курс  |      Спеціальність      |   Оцінка з фізкики   |   Оцінка з математики   |   Оцінка з інформатики   |" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------------------------------" << endl;

    ifstream f(fName, ios::binary);
    Student student;
    int i = 1;
    while (f.read((char*)&student, sizeof(Student))) {
        cout << "| " << setw(3) << right << i++ << "  ";
        cout << "| " << setw(15) << left << student.surname
             << "|   " << setw(3) << right << student.course << "  "
             << "| " << setw(25) << left << strSpecialty[student.specialty]
             << "| " << setw(20) << right << student.physics_grade << " "
             << "| " << setw(22) << right << student.math_grade << " "
             << "| " << setw(23) << right << student.inf_grade << " "
             << "|" << endl;
    }
    cout << "========================================================================================================================================" << endl;
    cout << endl;

}

void fWrite(fstream& f, const int i, Student x) {
    f.seekp(i * (long)sizeof(Student));
    f.write((char*)&x, sizeof(Student));
}

Student fRead(fstream& f, const int i) {
    Student x;
    f.seekg(i * (long)sizeof(Student));
    f.read((char*)&x, sizeof(Student));
    return x;
}

void fChange(fstream& f, const int i, const int j) {
    Student x = fRead(f, i);
    Student y = fRead(f, j);
    fWrite(f, i, y);
    fWrite(f, j, x);
}

void physical_sort(const char* fName) {
    fstream f(fName, ios::binary | ios::in | ios::out);

    f.seekg(0, ios::end);
    int size = f.tellg()/sizeof(Student);
    f.seekg(0, ios::beg);

    for (int i0 = 1; i0 < size; i0++) {
        for (int i1 = 0; i1 < size - i0; i1++) {
            Student a = fRead(f, i1);
            Student b = fRead(f, i1 + 1);
            if ((a.course < b.course) ||
                (a.course == b.course && a.inf_grade < b.inf_grade) ||
                (a.course == b.course && a.inf_grade == b.inf_grade && strcmp(a.surname, b.surname) == -1)) {
                fChange(f, i1, i1 + 1);
            }
        }
    }


    f.seekp(0, ios::end);
}

void index_sort (const char* fName, const char* gName) {

    fstream g(gName, ios::binary | ios::in | ios::out | ios::trunc);
    fstream f(fName, ios::binary | ios::in | ios::out);

    f.seekg(0, ios::end);
    int size = f.tellg() / sizeof(Student);
    f.seekg(0, ios::beg);

    for (int i = 0; i < size; i++)
        g.write((char*)&i, sizeof(int));
    g.seekg(0, ios::beg);

    int first_value, second_value, tmp;

    for (int i0 = 1; i0 < size; i0++) {
        for (int i1 = 0; i1 < size - i0; i1++) {;
            g.seekg(i1 * (long)sizeof(int));
            g.read((char*)&first_value, sizeof(int));
            g.read((char*)&second_value, sizeof(int));
            Student a = fRead(f, first_value);
            Student b = fRead(f, second_value);
            if ((a.course < b.course) ||
                (a.course == b.course && a.inf_grade < b.inf_grade) ||
                (a.course == b.course && a.inf_grade == b.inf_grade && strcmp(a.surname, b.surname) == -1)) {
                int tmp1;
                int tmp2;
                g.seekg(i1 * (long)sizeof(int));
                g.read((char*)&tmp1, sizeof(int));
                g.seekg((i1 + 1) * (long)sizeof(int));
                g.read((char*)&tmp2, sizeof(int));
                g.seekg(i1 * (long)sizeof(int));
                g.write((char*)&tmp2, sizeof(int));
                g.seekg((i1 + 1) * (long)sizeof(int));
                g.write((char*)&tmp1, sizeof(int));
            }
        }
    }

    g.seekg(0, ios::beg);
    f.seekp(0, ios::end);
    f.close();
    g.close();
}

void index_print

(const char* fName, const char* gName) {
    cout << "========================================================================================================================================" << endl;
    cout << "|  No  |  Прізвище студента    |  Курс  |      Спеціальність      |   Оцінка з фізкики   |   Оцінка з математики   |   Оцінка з інформатики   |" << endl;
    cout << "----------------------------------------------------------------------------------------------------------------------------------------" << endl;

    ifstream g(gName, ios::binary);
    fstream f(fName, ios::binary | ios::in | ios::out);
    Student student;

    f.seekg(0, ios::end);
    int size = f.tellg() / sizeof(Student);
    f.seekg(0, ios::beg);

    int ind, i = 1;

    while (g.read((char*)&ind, sizeof(int))) {
        f.seekp(ind * (long)sizeof(Student));
        f.read((char*)&student, sizeof(Student));
        cout << "| " << setw(3) << right << i++ << "  ";
        cout << "| " << setw(13) << left << student.surname
             << "|   " << setw(3) << right << student.course << "  "
             << "| " << setw(25) << left << strSpecialty[student.specialty]
             << "| " << setw(20) << right << student.physics_grade << " "
             << "| " << setw(22) << right << student.math_grade << " "
             << "| " << setw(22) << right << student.math_grade << "|" << endl;

    }
    cout << "========================================================================================================================================" << endl;
    cout << endl;

}

int binarySearch(const char* fName, const char* surname, const int course, const int inf_grade) {
    fstream f(fName, ios::binary | ios::in | ios::out);
    int P = 0, h;
    f.seekg(0, ios::end);
    int R = f.tellg() / sizeof(Student);
    f.seekg(0, ios::beg);
    do {
        h = (P + R) / 2;
        Student a = fRead(f, h);
        if (a.inf_grade == inf_grade && strcmp(a.surname, surname) == 0 && a.course == course) {
            return h + 1;
        }
        else if ((a.inf_grade < inf_grade) ||
                 (a.inf_grade == inf_grade && strcmp(a.surname, surname) == -1) ||
                 (a.inf_grade == inf_grade && strcmp(a.surname, surname) == 0 && a.course < course)) {
            R = h - 1;
        }
        else {
            P = h + 1;
        }
    } while (P <= R);
    return -1;
}

