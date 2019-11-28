#include <iostream>
#include "Calculator.h"
using namespace std;
int main()
{
	string s;
	cout << "请输入表达式：" << endl;
	cin >> s;
	cout << Calculator::Calculate(s) << endl;
	getchar();
	getchar();
}
