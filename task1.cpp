#include <iostream>
#include <cmath>
#include <string>
using namespace std;

const double notNumberConst = 751653.264781; //<<notNumber>> const

class StringEffective {
	string content;
	int size;
	double digit;
public:
	StringEffective (const string str) {
		content = str;
		size = str.size();
		digit = makeToDigit();
		if (digit == ::notNumberConst) digit = ::notNumberConst;
	}

	StringEffective (double number) {
		content = to_string(number);
		size = content.size();
		digit = number;
	}

	double makeToDigit () {
		double number = 0;
		int degree = 0, dotPosition = 0;
		int dotIsFound = 0, isNegative = 0;
		
		if (content[0] == '-') 
			isNegative = 1;
		for (int i = size - 1; i >= isNegative; --i) {
			if (content[i] == '.' || content[i] == ',') {
				if (dotIsFound == 1) {
					number = ::notNumberConst;
					//cout << "Error of translating into digital\n";
					dotPosition = 0;
					isNegative = 0;
					break;
				}
				dotIsFound = 1;
				dotPosition = size - i - 1;
				continue;
			} else if (content[i] <= '9' && content[i] >= '0')
				number += (content[i] - '0') * pow(10, degree++);
			else {
				number = ::notNumberConst;
				//cout << "Error of translating into digital\n";
				dotPosition = 0;
				isNegative = 0;
				break;
			}
		}
		if (isNegative == 1)
			number *= -1;
		if (number == ::notNumberConst) return ::notNumberConst;
		return number / pow(10.0, dotPosition);
	}

	int length () {
		return size;
	}
	
	operator float () {
		return digit;
	}

	operator double () {
		return digit;
	}
	
	operator int () {
		return (int) digit;
	}
	
	string get () {
		return content;
	}
	double getDigit () {
		return digit;
	}
	string getws () {
		return content + ' ';
	}
	string getwsp () {
		return ' ' + content;
	}
	string getwn () {
		return content + '\n';
	}
	string getwnp () {
		return '\n' + content;
	}

	string getFormatted () {
		string formatted = content;
		if (content[size - 1] != '.')
			formatted = content + '.';
		else
			formatted = content;
		if (formatted[0] >= 'a' && formatted[0] <= 'z')
			formatted[0] = formatted[0] + ('Z' - 'z');
		return formatted;
	}

	StringEffective operator+ (StringEffective other) {
		StringEffective* novo;
		if (digit != ::notNumberConst && other.digit != ::notNumberConst)
			novo = new StringEffective(digit + other.digit);
		else novo = new StringEffective(get() + other.get());
		return *novo;
	}

	StringEffective concatenate(StringEffective other) {
		StringEffective novo(get() + other.get());
		return novo;
	}

	bool operator< (StringEffective other) {
		if (content == other.content)
			return false;
		int n = min(size, other.size);
		for (int i = 0; i < n; i++) {
			if (content[i] > other.content[i])
				return false;
			else if (content[i] < other.content[i])
				return true;
		}
		return size == n;
	}

	bool operator<= (StringEffective other) {
		if (content == other.content || *this < other) return 1;
		return 0;
	}

	bool operator> (StringEffective other) {
		return other < *this;
	}

	bool operator>= (StringEffective other) {
		return other <= *this;
	}

	bool operator== (StringEffective other) {
		return content == other.content;
	}
};

int main() {
	string str, str2;
	cout << "If you enter two numbers, you'll get some additional information about them\n" << "Enter two strings:\n";
	cin >> str >> str2;
	StringEffective stef(str), stre(str2);
	double number = stef.getDigit(), sum = number;
	if (number == ::notNumberConst)
		cout << "First one cannot be translated into number\n" << "Formatted view: " << stef.getFormatted();
	else
		cout << "First is a number " << number << endl;
	number = stre.getDigit();
	if (number == ::notNumberConst)
		cout << "Second one cannot be translated into number\n" << "Formatted view: " << stre.getFormatted();
	else {
		cout << "Second is a number " << number << endl;
		if (sum != ::notNumberConst) {
			sum = (stef + stre).getDigit();
			cout << "Sum of numbers is " << sum << endl;
			cout << "Concatenation is " << stef.concatenate(stre).getwn();
		}		
	}
	cout << "As strings they are:\n";
	if (stef < stre) 
		cout << stef.get() << " < " << stre.getwn();
	if (stef >= stre)
		cout << stef.getws() << ">= " << stre.getwn();
	if (stef == stre)
		cout << stef.getwnp() << " =" << stre.getwsp() << endl;
}