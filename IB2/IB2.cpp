// IB1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <locale.h>
#include <conio.h>
#include <typeinfo>
#include "fstream"
#include "string"
#include "map"

#define N 6

using namespace std;

map <char, int> bookCrypt;
int polybiusMas[N][N];
bool cryptMode = false, cryptReady = false, fileReady = false, fileDecrypt = false;
int* cryptMas = new int;
string masCr;
string masDeC;
string abc;

void PolybiusSquare() {
	int asciiCodeWord = -64;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			//if(bookCrypt[static_cast<char>(asciiCodeWord)] == 0)
			polybiusMas[i][j] = bookCrypt[abc[i * (N)+j]];
			//if(bookCrypt[static_cast<char>(asciiCodeWord)] == 0)
			//asciiCodeWord++;
			cout << i << j << " " << polybiusMas[i][j] << endl;
		}
	}


}

int GetCodeFromMas(int x) {
	for (int a = 0; a < N; a++) {
		for (int b = 0; b < N; b++) {
			if (polybiusMas[a][b] == x)return a * 10 + b;//to_string(a) + to_string(b);
		}
	}
	return 66;
}


void FindOtherSymbols() {
	std::string lineOther;
	std::ifstream inOther("key.txt");
	int lineNumberx = 1;
	int itx = 0;
	bool md1 = true, md2 = true, md3 = true;

	if (inOther.is_open())
	{
		while (getline(inOther, lineOther)) {
			while (itx < lineOther.length()) {
				if (lineOther[itx] == ' ' && md1) {
					if (itx + 1 > 9)bookCrypt[' '] = lineNumberx * 100 + itx + 1;
					else if (itx + 1 < 10) bookCrypt[' '] = lineNumberx * 10 + itx + 1;
					abc.push_back(' ');
					md1 = false;
					lineNumberx = 0;
					inOther.seekg(0L, std::ios_base::beg);
					itx = 0;
					break;
				}
				else if (lineOther[itx] == '.' && md2) {
					if (itx + 1 > 9)bookCrypt['.'] = lineNumberx * 100 + itx + 1;
					else if (itx + 1 < 10) bookCrypt['.'] = lineNumberx * 10 + itx + 1;
					abc.push_back('.');
					md2 = false;
					lineNumberx = 0;
					inOther.seekg(0L, std::ios_base::beg);
					itx = 0;
					break;
				}
				else if (lineOther[itx] == ':' && md3) {
					if (itx + 1 > 9)bookCrypt[':'] = lineNumberx * 100 + itx + 1;
					else if (itx + 1 < 10) bookCrypt[':'] = lineNumberx * 10 + itx + 1;
					abc.push_back(':');
					md3 = false;
					lineNumberx = 0;
					inOther.seekg(0L, std::ios_base::beg);
					itx = 0;
					break;
				}
				if (itx == lineOther.length())break;

				itx++;
			}
			lineNumberx++;
			itx = 0;
		}


	}
	inOther.close();
}

void BookMethod() {
	std::string lineBook;
	std::ifstream inBook("key.txt");
	int lineNumber = 1;
	int it = 0;
	int asciiCodeWord = -64;


	if (inBook.is_open())
	{
		while (getline(inBook, lineBook))
		{

			while (it < lineBook.length() - 1 && asciiCodeWord < -32) {
				if (lineBook[it] == static_cast<char>(asciiCodeWord) || lineBook[it] == static_cast<char>(asciiCodeWord + 32))
				{
					if (it + 1 > 9)bookCrypt[static_cast<char>(asciiCodeWord)] = lineNumber * 100 + it + 1;
					else if (it + 1 < 10) bookCrypt[static_cast<char>(asciiCodeWord)] = lineNumber * 10 + it + 1;
					abc.push_back(static_cast<char>(asciiCodeWord));
					asciiCodeWord++;
					lineNumber = 0;
					inBook.seekg(0L, std::ios_base::beg);
					it = 0;
					break;
				}

				if (asciiCodeWord < -32 && inBook.eof()) { // 32 ASCII  - пробел
					bookCrypt[static_cast<char>(asciiCodeWord)] = asciiCodeWord;
					abc.push_back(static_cast<char>(asciiCodeWord));
					asciiCodeWord++;
					lineNumber = 0;
					inBook.seekg(0L, std::ios_base::beg);
					it = 0;
					break;
				}

				it++;
			}

			lineNumber++;
			it = 0;

		}
	}
	inBook.close();

	FindOtherSymbols();

	auto itf = bookCrypt.begin();
	for (; itf != bookCrypt.end(); itf++)
		cout << (*itf).first << ' ' << itf->second << '\n';

}

void Clearscr() {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0, 0 };
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
	FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
	SetConsoleCursorPosition(hStdOut, coord);
}

int polybiusDecrypt(string lp) {
	string temp;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			temp = to_string(i);
			temp = temp + to_string(j);
			if (temp == lp)return polybiusMas[i][j];
		}

	}
	return 0;
}

void CryptFile() {
	std::string line;

	std::ifstream in("open.txt");
	int tmp = 0;
	if (in.is_open())
	{
		while (getline(in, line))
		{
			for (int i = 0; i < line.length() - 1; i++) {

				for (auto itf = bookCrypt.begin(); itf != bookCrypt.end(); itf++) {
					if (line[i] == (*itf).first) {
						//cryptMas[i] = GetCodeFromMas(itf->second);

						string str = to_string(GetCodeFromMas(itf->second));
						if (GetCodeFromMas(itf->second) <= 5 && GetCodeFromMas(itf->second) >= 0)masCr.push_back('0');
						//cout << GetCodeFromMas(itf->second)<<" ";
						//f(GetCodeFromMas(itf->second))
						masCr.append(str);
						tmp = i;
						break;
					}
				}
			}
		}
	}
	in.close();
	std::ofstream out;
	//out.open("crypt.txt", ios::app);
	out.open("crypt.txt", std::ofstream::out | std::ofstream::trunc);
	if (out.is_open())
	{
		out << *&masCr;
	}

	out.close();


	//for (int i = 0; i < tmp; i++)cout << cryptMas[i] << " ";
	//cout << "\n\n" << masCr;
	//getline()
}

void decryptFile() {
	string l;
	std::ofstream dit;

	dit.open("decrypt.txt", std::ofstream::out | std::ofstream::trunc);
	if (dit.is_open()) {

		for (int i = 0; i < masCr.length(); i += 2) {
			int cd = polybiusDecrypt(masCr.substr(i, 2));
			for (auto itf = bookCrypt.begin(); itf != bookCrypt.end(); itf++) {
				if (itf->second == cd)dit << *&(*itf).first;
			}
		}


	}
	dit.close();



}



void Interface() {
	while (true) {
		cout << "\nВыберите действие: \n\n";
		if (!fileReady) {
			if (!cryptMode && !cryptReady)cout << "1 - Закодировать алфавит книжным шифром : \n\n";
			if (cryptReady && !cryptMode)cout << "2 - Закодировать алфавит квадратом Полибия : \n\n";
			if (cryptMode)cout << "3 - Зашифровать файл open.txt : \n\n";
			cout << "4 - Завершить работу программы : \n\n";
			switch (_getche()) {
			case '1': {
				if (!cryptReady && !cryptMode) {
					Clearscr();  BookMethod(); cryptReady = true; break;
				};
				break;
			}
			case '2': {
				if (cryptReady && !cryptMode) {
					Clearscr();  PolybiusSquare(); cryptMode = true; break;
				}
				break;
			};
			case '3': {
				if (cryptMode) {
					Clearscr(); CryptFile(); fileReady = true; break;
				}
				break;
			};
			case '4': {exit(0); break; }
			default: Clearscr();
			}
		}
		else if (fileReady) {
			cout << "1 - Открыть файл open.txt : \n\n";
			cout << "2 - Открыть файл crypt.exe : \n\n";
			if (!fileDecrypt)cout << "3 - Расшифровать файл crypt.exe : \n\n";
			else cout << "3 - Открыть файл decrypt.exe : \n\n";
			cout << "4 - Завершить работу программы : \n\n";
			switch (_getche()) {
			case '1': {Clearscr(); system("notepad.exe open.txt"); break; };
			case '2': {Clearscr();  system("notepad.exe crypt.txt"); break; };
			case '3': {
				if (!fileDecrypt) {
					Clearscr();
					decryptFile();
					fileDecrypt = true;
					break;
				}
				else {
					Clearscr();
					system("notepad.exe decrypt.txt");
					break;
				}
			};
			case '4': {exit(0); break; }
			}
		}

	}
}




int main()
{
	setlocale(LC_ALL, "RUS");
	Interface();
	_getch();
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
