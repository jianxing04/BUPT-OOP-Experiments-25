#include<bits/stdc++.h>
using namespace std;
class point
{
    private:
        int x,y;
    public:
        point(int xx=0,int yy=0):x(xx),y(yy){}
        point& operator ++ ()
        {
            x++;y++;
            return *this;
        }
        point& operator -- ()
        {
            x--;y--;
            return *this;
        }
        point operator ++(int) 
        {
            point tem(*this);
            x++;y++;
            return tem;
        }
        point operator -- (int)
        {
            point tem(*this);
            x--;y--;
            return tem;
        }
        void print()
        {
            cout<<"\n"<<this<<" 的 x: "<<x<<" , y: "<<y<<"\n";
        }
};
int main()
{
    point a1(1,2);
    ++a1;
    a1.print();
    --a1;
    a1.print();
    a1++;
    a1.print();
    a1--;
    a1.print();
    return 0;
}