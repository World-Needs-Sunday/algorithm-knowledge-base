#include <bits/stdc++.h>
using namespace std;

void bubbleSort(int v[], int n)
{
    for(int i=1;i<=n;i++){
        for(int j=i+1;j<=n;j++){
            if(v[i]>v[j])swap(v[i],v[j]);
        }
    }
}

void selectionSort(int a[], int n)
{
    for (int i=1;i<n;i++) {
        int zz=i;
        for (int j=i+1;j<=n;j++) {
          if(a[j]<a[zz]) zz=j;
        }
        swap(a[i],a[zz]);
      }
}

int main()
{
    int n;cin>>n;
    int v[n+10], a[n+10];
    for(int i=1;i<=n;i++){
        cin>>v[i];
        a[i]=v[i];
    }
    bubbleSort(v, n);
    selectionSort(a, n);
    for(int i=1;i<=n;i++){
        cout<<v[i]<<" ";
    }
    cout<<'\n';
    for(int i=1;i<=n;i++){
        cout<<a[i]<<" ";
    }
    return 0;
}
