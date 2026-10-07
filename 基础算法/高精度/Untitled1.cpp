#include<bits/stdc++.h>
using namespace std;

const int nmax = 200;
int A[nmax], B[nmax], tmp[nmax];

// 高精度比较：a > b 返回 1，a == b 返回 0，a < b 返回 -1
int cmp(const string& a, const string& b)
{
	if (a.size() != b.size()) return a.size() > b.size() ? 1 : -1;
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] == b[i]) continue;
		return a[i] > b[i] ? 1 : -1;
	}
	return 0;
}

// 高精度乘法
string mull(const string& a, const string& b)
{
	if (a == "0" || b == "0") return "0";
	int na = a.size();
	int nb = b.size();
	memset(A, 0, na * sizeof(int));
	memset(B, 0, nb * sizeof(int));
	memset(tmp, 0, (na + nb + 2) * sizeof(int));
	for (int i = na - 1; i >= 0; i--) A[na - 1 - i] = a[i] - '0';
	for (int i = nb - 1; i >= 0; i--) B[nb - 1 - i] = b[i] - '0';
	for (int i = 0; i < na; i++)
	{
		for (int j = 0; j < nb; j++)
		{
			tmp[i + j] += A[i] * B[j];
		}
	}
	for (int i = 0; i < na + nb; i++)
	{
		tmp[i + 1] += tmp[i] / 10;
		tmp[i] %= 10;
	}
	int idx = na + nb + 1;
	while (idx > 0 && !tmp[idx]) idx--;
	string s = "";
	s.reserve(idx + 1);
	for (int i = idx; i >= 0; i--) s += to_string(tmp[i]);
	return s;
}

// 高精度加法
string add(const string& a, const string& b)
{
	if (a == "0") return string(b);
	else if (b == "0") return string(a);
	int na = a.size();
	int nb = b.size();
	memset(A, 0, max(na, nb) * sizeof(int));
	memset(B, 0, max(na, nb) * sizeof(int));
	memset(tmp, 0, (max(na, nb) + 2) * sizeof(int));
	for (int i = na - 1; i >= 0; i--) A[na - 1 - i] = a[i] - '0';
	for (int i = nb - 1; i >= 0; i--) B[nb - 1 - i] = b[i] - '0';
	for (int i = 0; i < max(na, nb); i++) tmp[i] = A[i] + B[i];
	for (int i = 0; i < max(na,nb) ; i++)
	{
		tmp[i + 1] += tmp[i] / 10;
		tmp[i] %= 10;
	}
	int idx = max(na, nb) + 1;
	while (idx > 0 && !tmp[idx]) idx--;
	string s = "";
	s.reserve(idx + 1);
	for (int i = idx; i >= 0; i--) s += to_string(tmp[i]);
	return s;
}

// 高精度减法（要求 a >= b，结果为非负数）
string sub(const string& a, const string& b)
{
	int c = cmp(a, b);
	if (c == 0) return "0";
	if (c < 0) return "undef";
	int na = a.size();
	int nb = b.size();
	memset(A, 0, max(na, nb) * sizeof(int));
	memset(B, 0, max(na, nb) * sizeof(int));
	memset(tmp, 0, (max(na, nb) + 1) * sizeof(int));
	for (int i = na - 1; i >= 0; i--) A[na - 1 - i] = a[i] - '0';
	for (int i = nb - 1; i >= 0; i--) B[nb - 1 - i] = b[i] - '0';
	for (int i = 0; i < max(na, nb); i++)
	{
		if (A[i] < B[i])
		{
			A[i] += 10;
			A[i + 1] -= 1;
		}
		tmp[i] = A[i] - B[i];
	}
	int idx = max(na, nb);
	while (idx > 0 && !tmp[idx]) idx--;
	string s = "";
	s.reserve(idx + 1);
	for (int i = idx; i >= 0; i--) s += to_string(tmp[i]);
	return s;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	string a, b;
	cin >> a >> b;
	int t = cmp(a, b);
	if (t == 0) cout << a << " == " << b << '\n';
	else if(t == 1) cout << a << " > " << b << '\n';
	else if (t == -1)
	{
		cout << a << " < " << b << '\n';
		cout << "undef\n";
		swap(a, b);
	}
	cout << a << " + " << b << " = " << add(a, b) << '\n';
	cout << a << " * " << b << " = " << mull(a, b) << '\n';
	cout << a << " - " << b << " = " << sub(a, b) << '\n';
	return 0;
}
