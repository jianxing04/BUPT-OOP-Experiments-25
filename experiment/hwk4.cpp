#include<bits/stdc++.h>
using namespace std;
class shape
{
    public:
        shape()
        {
            cout<<"\n这是shape的构造函数\n";
        }
        ~shape()
        {
            cout<<"\n这是shape的析构函数\n";
        }
        double area_is()
        {
            cout<<"\narea_is 函数在shape\n";
            return 0;
        }
    private:
        double area;
};
class matrix:public shape
{
    public:
        matrix(double c,double k):shape()
        {
            cout<<"\n这是matrix的构造函数\n";
            chang=c;
            kuan=k;
        }
        ~matrix()
        {
            cout<<"\n这是matrix的析构函数\n";
        }
        double area_is()
        {
            cout<<"\nmatrix里的area_is函数\n";
            return chang*kuan;
        }
    private:
        double chang,kuan;
};
class circle:public shape
{
    public:
        circle(double R):shape()
        {
            cout<<"\n这是circle的构造函数\n";
            r=R;
        }
        ~circle()
        {
            cout<<"\n这里是circle的析构函数\n";
        }
        double area_is()
        {
            cout<<"\ncircle里的area_is函数\n";
            return 3.14*r*r;
        }
    private:
        double r;
};
class square:public matrix
{
    public:
        square(double LEN):matrix(LEN,LEN)
        {
            cout<<"\n这是square的构造函数\n";
            len=LEN;
        }
        ~square()
        {
            cout<<"\n这是square的析构函数\n";
        }
        double area_is()
        {
            cout<<"\narea_is 在square里\n";
            return len*len;
        }
    private:
        double len;
};
int main()
{
    {
        matrix m(2,3);
        cout<<m.area_is();
        circle c(1);
        cout<<c.area_is();
        square s(2);
        cout<<s.area_is();
    }
    return 0;
}