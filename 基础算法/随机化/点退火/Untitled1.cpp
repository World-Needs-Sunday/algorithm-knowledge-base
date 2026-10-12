#include<bits/stdc++.h>
using namespace std;
mt19937 rd;
vector<double> x, y, w;
const double TL = 0.992;
double curx, cury, curE, bestE, bestx, besty;
double cala(double mx, double my, int n)
{
	double res = 0;
	for (int i = 1; i <= n; i++)
	{
		double dx = mx - x[i];
		double dy = my - y[i];
		double len = sqrt(dx * dx + dy * dy);

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
void sa(int n)
{
	curx = bestx;
	cury = besty;
	curE = bestE;
	static uniform_real_distribution<double> dist(-32767.0, 32767.0);
	static uniform_real_distribution<double> prob(0.0, 1.0);
	for (double t = 4000; t > 1e-12; t *= 0.9992)
	{
		double nx = curx + dist(rd) * t;
		double ny = cury + dist(rd) * t;
		double newE = cala(nx, ny, n);
		double delta = newE - curE;
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

		if ((double)clock() / CLOCKS_PER_SEC >= TL) break;
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	int n;
	cin >> n;
	init(n);
	double mx = 0, my = 0, mw = 0;
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
	while ((double)clock() / CLOCKS_PER_SEC < TL) sa(n);

	cout << fixed << setprecision(3) << bestx << ' ' << besty;
	
}