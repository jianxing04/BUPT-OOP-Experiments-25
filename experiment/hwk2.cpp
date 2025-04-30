#include<bits/stdc++.h>
using namespace std;
class point
{
    private:
        double x,y;
    public:
        point(double xx=0,double yy=0)
        {
            x=xx;
            y=yy;
            cout<<"\npoint类 构造 函数调用\n";
        }
        ~point()
        {
            cout<<"\npoint类 析构 函数调用\n";
        }
        double dis(const point &tem) const
        {
            return sqrt((x-tem.x)*(x-tem.x)+(y-tem.y)*(y-tem.y));
        }
        double dis() const
        {
            return sqrt(x*x+y*y);
        }
        double get_x()
        {
            return x;
        }
        double get_y()
        {
            return y;
        }
};
class circle:public point
{
    private:
        double r;
    public:
        circle(double xx,double yy,double rr):point(xx,yy)
        {
            r=rr;
            cout<<"\ncircle类 构造 函数调用\n";
        }
        ~circle()
        {
            cout<<"\ncircle类 析构 函数调用\n";
        }
        double get_r()
        {
            return r;
        }
        double dis(const circle &tem) const
        {
            return point::dis(tem);
        }
        bool is_intersect(const circle &tem) const
        {
            double d=dis(tem);
            return d<=r+tem.r&&d>=max(r,tem.r)-min(r,tem.r);
        }
};
int main()
{
    {
        double x1,y1,x2,y2,r1,r2;
        cout<<"\n请输入第一个圆的坐标和半径:\n";
        cin>>x1>>y1>>r1;
        cout<<"\n请输入第二个圆的坐标和半径:\n";
        cin>>x2>>y2>>r2;
        circle c1(x1,y1,r1),c2(x2,y2,r2);
        if (c1.is_intersect(c2))
            cout<<"\n相交\n";
        else
            cout<<"\n不相交\n";
    }
    return 0;
}
/*
2 2 2
6 2 2
*/