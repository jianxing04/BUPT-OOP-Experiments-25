#include<bits/stdc++.h>
using namespace std;
class matrix
{
    private:
        int **num;
    public:
        matrix()
        {
			cout<<"\n构造函数调用:"<<this<<"\n";
            num=new int*[4];
            for (int i=0;i<4;i++)
                num[i]=new int[5]{0};
        }
        matrix(const matrix &tem)
        {
            cout<<"\n拷贝构造调用:用 "<<&tem<<" 初始化 "<<this<<"\n";
        }
        ~matrix()
        {
			cout<<"\n析构函数调用:"<<this<<"\n";
            for (int i=0;i<4;i++)
                delete[] num[i];
            delete[] num;
        }
        void init()
        {
            for (int i=0;i<4;i++)
                for (int j=0;j<5;j++)
                    cin>>num[i][j];
        }
        void print()
        {
            cout<<"\n";
            for (int i=0;i<4;i++)
            {
                for (int j=0;j<5;j++)
                    cout<<num[i][j]<<" ";
                cout<<"\n";
            }
        }
        matrix add(const matrix &x) const
        {
            matrix ans;
            for (int i=0;i<4;i++)
                for (int j=0;j<5;j++)
                    ans.num[i][j]=num[i][j]+x.num[i][j];
            return ans;//由于编译器RVO优化，这里不会调用拷贝构造函数
        }
        matrix sub(const matrix &x) const
        {
            matrix ans;
            for (int i=0;i<4;i++)
                for (int j=0;j<5;j++)
                    ans.num[i][j]=num[i][j]-x.num[i][j];
            return ans;//由于编译器RVO优化，这里不会调用拷贝构造函数
        }
        matrix& operator = (const matrix &tem)
        {
            cout<<"\n重载=赋值，用 "<<&tem<<" 给 "<<this<<" 赋值\n";
            if (this!=&tem)
            {
                for (int i=0;i<4;i++)
                    for (int j=0;j<5;j++)
                        num[i][j]=tem.num[i][j];
            }
            return *this;
        }
};
int main()
{
	{
        matrix *a1,*a2,*a3,*a4;
        a1=new matrix;
        a2=new matrix;
        a3=new matrix;
        a4=new matrix;
        a1->init();
        a2->init();
        *a4=*a3=(*a1).add(*a2);
        a3->print();
        a4->print();
        *a4=*a3=(*a1).sub(*a2);
        a3->print();
        a4->print();
        delete a1;
        delete a2;
        delete a3;
        delete a4;
	}
    return 0;
}

/* 
1 2 3 4 5
5 4 3 2 1
1 2 3 4 5
5 4 3 2 1
1 2 3 4 5
5 4 3 2 1
1 2 3 4 5
5 4 3 2 1

1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
1 1 1 1 1
2 2 2 2 2
2 2 2 2 2
2 2 2 2 2
2 2 2 2 2
*/