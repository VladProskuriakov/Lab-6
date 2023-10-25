// ������ 4.cpp : ���� ���� �������� ������� "main". ����� ���������� � ������������� ���������� ���������.
//

#include <iostream>
#include <windows.h> 
#include <math.h>
using namespace std;
CRITICAL_SECTION cs;

double Integral; // ����� ����� ��������
double a = 1, b = 10; // ����� ������� ��������������
const int n = 129; // ����� ����� ��������� n
double h = (b - a) / n;// ����� ��� ��������������
double square = 0; // ���������� ��� �������, ������� ������ � ������� �������
int num_potok[n]; // ������ ������� �������

// �������� �������
double f(double x)
{
    return cos(2 * x);
}

// ����� ������� ��������� � i-� ������
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
    HANDLE hThread[n]; // ���������
    DWORD ThreadID[n]; // ��������������
    // ��������� ������ � �������� �������
    for (int i = 0; i < n; i++)
    {
        num_potok[i] = i;
    }
    // �������� ������� ��� �������� ������� ������� ���������
    for (int i = 0; i < n; i++)
    {
        hThread[i] = CreateThread(NULL, 0, part_square_f, &(num_potok[i]), 0, &(ThreadID[i]));
        if (hThread[i] == NULL) GetLastError();
    }
    WaitForMultipleObjects(n, hThread, true, INFINITE);
    for (int i = 0; i < n; i++) CloseHandle(hThread[i]);

    double sum = 0, k;
    // ��������, ���������� ������� �������� ����������� � �����
    for (int i = 0; i < n; i++)
    {
        k = 0.5 * h * (f(a + i * h) + f(a + (i + 1) * h));
        sum += k;
    }

    cout << "�� ������: " << sum << endl;
    cout << "������: " << square << "\n";
}

// ������ ���������: CTRL+F5 ��� ���� "�������" > "������ ��� �������"
// ������� ���������: F5 ��� ���� "�������" > "��������� �������"

// ������ �� ������ ������ 
//   1. � ���� ������������ ������� ����� ��������� ����� � ��������� ���.
//   2. � ���� Team Explorer ����� ������������ � ������� ���������� ��������.
//   3. � ���� "�������� ������" ����� ������������� �������� ������ ������ � ������ ���������.
//   4. � ���� "������ ������" ����� ������������� ������.
//   5. ��������������� �������� ������ ���� "������" > "�������� ����� �������", ����� ������� ����� ����, ��� "������" > "�������� ������������ �������", ����� �������� � ������ ������������ ����� ����.
//   6. ����� ����� ������� ���� ������ �����, �������� ������ ���� "����" > "�������" > "������" � �������� SLN-����.
