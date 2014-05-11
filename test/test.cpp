#include<iostream>
#include<algorithm>
#include<cstdio>
#include<vector>

using namespace std;

int main()
{
   int n;
   cin >> n;
   vector<int> v;
   v.push_back(n);
   
   cout << v[n] << endl;
   
   return 0;
}

