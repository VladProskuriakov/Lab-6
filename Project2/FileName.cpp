// Секции 4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h> 
#include <math.h>
using namespace std;
CRITICAL_SECTION cs;

double Integral; // здесь будет интеграл
double a = 1, b = 10; // задаём отрезок интегрирования
const int n = 129; // задаём число разбиений n
double h = (b - a) / n;// задаём шаг интегрирования
double square = 0; // переменная для площади, которую найдем с помощью потоков
int num_potok[n]; // массив номеров потоков

// заданная функция
double f(double x)
{
    return cos(2 * x);
}

// поиск площади разбиения в i-м потоке
DWORD WINAPI part_square_f(LPVOID p)
{
    int* i_potok = (int*)p;
    int i = *i_potok;
    Integral = 0.5 * h * (f(a + i * h) + f(a + (i + 1) * h));
    EnterCriticalSection(&cs);
    square += Integral;
    LeaveCriticalSection(&cs);
    return 0;
}

int main()
{
    setlocale(LC_ALL, "ru");
    InitializeCriticalSection(&cs);
    HANDLE hThread[n]; // указатели
    DWORD ThreadID[n]; // идентификаторы
    // заполняем массив с номерами потоков
    for (int i = 0; i < n; i++)
    {
        num_potok[i] = i;
    }
    // создание потоков для подсчета площади каждого разбиения
    for (int i = 0; i < n; i++)
    {
        hThread[i] = CreateThread(NULL, 0, part_square_f, &(num_potok[i]), 0, &(ThreadID[i]));
        if (hThread[i] == NULL) GetLastError();
    }
    WaitForMultipleObjects(n, hThread, true, INFINITE);
    for (int i = 0; i < n; i++) CloseHandle(hThread[i]);

    double sum = 0, k;
    // проверка, нахождение площади трапеции разбиениями в цикле
    for (int i = 0; i < n; i++)
    {
        k = 0.5 * h * (f(a + i * h) + f(a + (i + 1) * h));
        sum += k;
    }

    cout << "Не потоки: " << sum << endl;
    cout << "Потоки: " << square << "\n";
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
