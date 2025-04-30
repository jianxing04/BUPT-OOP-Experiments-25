#include<bits/stdc++.h>
using namespace std;
class matrix
{
    private:
        int row,col,**num;
    public:
        matrix(int r=109,int c=109)
        {
            row=r;
            col=c;
            num=new int*[row];
            for (int i=0;i<row;i++)
                num[i]=new int[col]{0};
            cout<<"\n"<<this<<" 成功调用 构造 函数\n";
        }
        matrix(const matrix &tem)
        {
            row=tem.row;
            col=tem.col;
            num=new int*[row];
            for (int i=0;i<row;i++)
                num[i]=new int[col];
            for (int i=0;i<row;i++)
                for (int j=0;j<col;j++)
                    num[i][j]=tem.num[i][j];
            cout<<"\n成功调用 拷贝构造 函数：用 "<<&tem<<" 初始化 "<<this<<"\n";
        }
        ~matrix()
        {
            for (int i=0;i<row;i++)
                delete[] num[i];
            delete[] num;
            cout<<"\n"<<this<<" 成功调用 析构 函数\n";
        }
        void init()
        {
            for (int i=0;i<row;i++)
                for (int j=0;j<col;j++)
                    cin>>num[i][j];
        }
        void print()
        {
            cout<<"\n";
            for (int i=0;i<row;i++)
            {
                for (int j=0;j<col;j++)
                    cout<<num[i][j]<<" ";
                cout<<"\n";
            }
        }
        matrix& operator = (const matrix &tem)
        {
            row=tem.row;
            col=tem.col;
            for (int i=0;i<row;i++)
                for (int j=0;j<col;j++)
                    num[i][j]=tem.num[i][j];
            cout<<"\n成功用重载的=用 "<<&tem<<" 给 "<<this<<" 赋值\n";
            return *this;
        }
        matrix add(const matrix &tem) const
        {
            if (tem.row!=row||tem.col!=col)
            {
                cout<<"\n行数或列数不同,无法相加\n";
                return *this;
            }
            matrix ans(row,col);
            for (int i=0;i<row;i++)
                for (int j=0;j<col;j++)
                    ans.num[i][j]=num[i][j]+tem.num[i][j];
            cout<<"\n"<<this<<" 与 "<<&tem<<" 运算成功,答案暂存ans: "<<&ans<<"\n";
            return ans;
        }
        matrix sub(const matrix &tem) const
        {
            if (tem.row!=row||tem.col!=col)
            {
                cout<<"\n行数或列数不同,无法相减\n";
                return *this;
            }
            matrix ans(row,col);
            for (int i=0;i<row;i++)
                for (int j=0;j<col;j++)
                    ans.num[i][j]=num[i][j]-tem.num[i][j];
            cout<<"\n"<<this<<" 与 "<<&tem<<" 运算成功,答案暂存ans: "<<&ans<<"\n";
            return ans;
        }
};
int main()
{
    {
    matrix a1(5,5),a2(4,5),a3;
    a1.init();
    a2.init();
    a3=a1.add(a2);
    a3.print();
    a3=a1.sub(a2);
    a3.print();
    }
    {
    matrix *a1,*a2,*a3;
    a1=new matrix(5,5);
    a2=new matrix(5,5);
    a3=new matrix;
    a1->init();
    a2->init();
    *a3=a1->add(*a2);
    a3->print();
    *a3=a1->sub(*a2);
    a3->print();
    }
    return 0;
}
/*
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
2 2 2 2 2
2 2 2 2 2
2 2 2 2 2
2 2 2 2 2

1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
2 2 2 2 2
2 2 2 2 2
2 2 2 2 2 
2 2 2 2 2
2 2 2 2 2 
*/