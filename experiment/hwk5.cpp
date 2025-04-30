#include<bits/stdc++.h>
using namespace std;
int main()
{
    srand(time(0));
    int x=rand()%100;
    int k;
    cout<<"猜一个0~99的数\n";
    while(1)
    {
        cin>>k;
        if (k<0||k>99)
            cout<<"超出范围\n";
        else if (k==x)
        {
            cout<<"答对了！";
            break;
        }
        else if (k>x)
            cout<<"大了\n";
        else
            cout<<"小了\n";
    }
    return 0;
}