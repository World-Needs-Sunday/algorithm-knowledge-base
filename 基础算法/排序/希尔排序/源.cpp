#include<bits/stdc++.h>
using namespace std;
void shellsort(vector<int>& arr)
{
	int n = arr.size();
	for (int gap = n / 2; gap > 0; gap /= 2)
	{
		for (int i = gap; i < n; i++)
		{
			int j = i - gap;
			int tmp = arr[i];
			while (j >= 0 && tmp < arr[j])
			{
				arr[j + gap] = arr[j];
				j -= gap;
			}
			arr[j + gap] = tmp;
		}
	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n;
	cin >> n;
	vector<int> A(n);
	for (int i = 0; i < n; i++) cin >> A[i];
	shellsort(A);
	for (int i = 0; i < n; i++) cout << A[i] << ' ';
	return 0;
}
