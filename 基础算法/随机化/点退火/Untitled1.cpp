#include<bits/stdc++.h>
using namespace std;
mt19937 rd;
vector<long double> x, y, w;
const long double TL = 0.9;
long double curx, cury, curE, bestE, bestx, besty;
long double cala(long double mx, long double my, int n)
{
	long double res = 0;
	for (int i = 1; i <= n; i++)
	{
		long double dx = mx - x[i];
		long double dy = my - y[i];
		long double len = sqrt(dx * dx + dy * dy);

		res += len * w[i];
	}
	return res;
}
void init(int n)
{
	x.assign(n + 1, 0);
	y.assign(n + 1, 0);
	w.assign(n + 1, 0);
	rd.seed(static_cast<unsigned int>(std::time(nullptr)));
}
void sa(int n, long double t, long double a)
{
	curx = bestx;
	cury = besty;
	curE = bestE;
	static uniform_real_distribution<long double> dist(-1.0, 1.0);
	static uniform_real_distribution<long double> prob(0.0, 1.0);
	for (; t > 1e-8; t *= a)
	{
		long double nx = curx + dist(rd) * t;
		long double ny = cury + dist(rd) * t;
		long double newE = cala(nx, ny, n);
		long double delta = newE - curE;
		if (delta < 0 || exp(-delta / t) > prob(rd))
		{
			curx = nx;
			cury = ny;
			curE = newE;
		}
		if (curE < bestE)
		{
			bestE = curE;
			bestx = curx;
			besty = cury;
		}
		if ((long double)clock() / CLOCKS_PER_SEC >= TL) break;

	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	int n;
	cin >> n;
	init(n);
	long double mx = 0, my = 0, mw = 0;
	for (int i = 1; i <= n; i++)
	{
		cin >> x[i] >> y[i] >> w[i];
		mx += x[i] * w[i];
		my += y[i] * w[i];
		mw += w[i];
	}
	mx /= mw;
	my /= mw;

	curx = mx, cury = my, curE = cala(mx, my, n);
	bestE = curE, bestx = mx, besty = my;

	long double t = 10000, a = 0.99;
	while ((long double)clock() / CLOCKS_PER_SEC < TL)
	{
		sa(n, t, a);
		t *= 0.8;
		a = 1 - 0.5 * (1 - a);
	}

	cout << fixed << setprecision(3) << bestx << ' ' << besty;

}