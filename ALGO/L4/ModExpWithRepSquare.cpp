#include <iostream>

using namespace std;

int binary[3][100];
int length = 1;

void calculateBinaryNumber(int e)
{
	int rem;
	while(1)
	{
		rem = e % 2;
		e = e / 2;
		//cout << rem ;
		binary[1][length] = rem;
		length++;
		if(e == 0)
			break;
	}
}

void repeatedSquaring(int b,int n)
{
	int prev;
	binary[2][1] = b%n;
	for(int i=2;i<length;i++)
	{
		prev = binary[2][i-1];
		binary[2][i] = (prev*prev) % n;

	}
}

void doModularExponentiation(int b,int e,int n)
{
	int final = 1;
	for(int i=1;i<length;i++)
	{
		if(binary[1][i]==1)
			final = ( (final*binary[2][i]) % n );
	}
	cout << "Modular Exponentiation using Repeated Squaring for " << b << "^" << e << " (mod " << n <<") is " << final % n << endl;
}
void print()
{
	for(int i=1;i<length;i++)
	{
		cout << binary[1][i] << "---" << binary[2][i] << endl;
	}
}

int main()
{
	int b,e,n;
	cout << "Input of the form : b^e (mod n)" << endl;
	cout << "Enter b : ";
	cin >> b;
	cout << "Enter e : ";
	cin >> e;
	cout << "Enter n : ";
	cin >> n;
	calculateBinaryNumber(e);
	repeatedSquaring(b,n);
	//print();
	doModularExponentiation(b,e,n);
	return 0;
}
