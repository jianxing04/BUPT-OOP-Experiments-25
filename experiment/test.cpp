#include<bits/stdc++.h>
using namespace std;
class tst
{
    public:
        int x;
        tst(int k=1)
        {
            x=k;
            cout<<"\n普通构造:"<<this<<"\n";
        }
        tst(const tst &a)
        {
            x=2*a.x;
            cout<<"\n拷贝构造:"<<this<<" from "<<&a<<"\n";
        }
        ~tst()
        {
            cout<<"\n"<<this<<" 的析构函数已调用\n";
        }
        void print()
        {
            cout<<"\n"<<this<<" 的 x is "<<x<<"\n";
        }
        tst& operator = (const tst &tem)
        {
            cout<<"\n重载的=调用: "<<&tem<<" 给 "<<this<<" 赋值\n";
            x=tem.x;
            return *this;
        }
};
tst rt(tst x)
{
    tst tem(2);
    return x;
}
int main()
{
    {
    tst a(2);
    tst b(rt(a));
    //c=b=a=rt(a);
    a.print();
    b.print();
    //c.print();
    }
    return 0;
}