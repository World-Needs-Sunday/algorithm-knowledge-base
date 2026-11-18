#include<iostream>
using namespace std;
void write(unsigned int n)
{
	static char ch[40];
    int idx = 0;
    do 
	{
        ch[idx++] = (n & 1) + '0';
        n >>= 1;
    } while (n);
    while (idx) cout << ch[--idx];
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	unsigned int s;
	cin >> s;
	cout << "此数代表的2进制集合为: ";
	write(s); 
	cout << '\n';
	
	//主要代码 
	cout << "子集为: \n"; 
	for(unsigned int i = s;;i = (i - 1) & s)
	{
		write(i);
		cout << '\n';
		if(i == 0) break;
	}
	return 0;	
}