#include<iostream>
#include<vector>
#include<bitset>
#include<unordered_map>
#include<queue>
using namespace std;
const int N=10;
int n,num_of_F,num_of_cin,num_of_tran[N][N],cnt;
bitset<N> tran[N][N],tran2[N][N],closure[N];
bool is_F[N];
unordered_map<string,int> mp;
queue<string> que;
void DFS(int k)
{
    if (closure[k][k])//状态k的闭包已经求好了
        return;
    closure[k][k]=1;
    for (int i=0;i<n;i++)
    {
        if (i==k||!closure[k][i])
            continue;
        DFS(i);
        closure[k]|=closure[i];//k的儿子能去的地方k也能去
    }
}
int main()
{
    cout<<"请输入状态数:\n";
    cin>>n;
    cout<<"初始状态默认为0,请输入结束状态个数:\n";
    cin>>num_of_F;
    cout<<"输入这"<<num_of_F<<"个结束状态:\n";
    for (int i=1,x;i<=num_of_F;i++)
    {
        cin>>x;
        is_F[x]=1;//记录状态x是结束状态
    }
    cout<<"输入字母表T的大小(含空串,非空默认为从a开始记)\n";
    cin>>num_of_cin;
    for (int i=0;i<n;i++)//输入转移函数
    {
        cout<<"下面请给出状态"<<i<<"的转移函数:\n\n";
        for (int j=0;j<num_of_cin;j++)
        {
            cout<<"对于状态"<<i<<",在输入";
            if (j==0)//j==0表示空串的转移函数
                cout<<"空串";
            else
                cout<<char('a'+j-1);
            cout<<"时能直接转移到的与他相邻的状态的个数:\n";
            cin>>num_of_tran[i][j];//状态i在输入j时能转移去的状态数
            if (num_of_tran[i][j]>0)
                cout<<"请输入这"<<num_of_tran[i][j]<<"个状态:\n";
            for (int k=1,x;k<=num_of_tran[i][j];k++)
            {
                cin>>x;
                if (j==0)
                    closure[i][x]=1;//i在输入空串时可以转移到x
                else
                    tran[i][j][x]=1;//状态i在输入j时可以转移到x
            }
        }
    }
    for (int i=0;i<n;i++)
        DFS(i);
    for (int i=1;i<n;i++)
        if (closure[0][i]&&is_F[i])//状态0的闭包包含终止状态
        {
            is_F[0]=1;//则对应的无空串的NFA中状态0也是终止状态
            break;
        }//带空串的NFA转向无空串的NFA,先确定终止状态
    for (int i=0;i<n;i++)//枚举每个状态
        for (int j=1;j<num_of_cin;j++)//枚举每个输入
            for (int u=0;u<n;u++)//寻找它的闭包
            {
                if (!closure[i][u])
                    continue;//状态i在输入空串无法到达状态u，即u不在i的闭包
                for (int v=0;v<n;v++)
                {
                    if (!tran[u][j][v])
                        continue;//状态u在输入字母j无法转移到状态v
                    for (int r=0;r<n;r++)
                    {
                        if (!closure[v][r])
                            continue;//状态v在输入空串无法到达状态r，即r不在v的闭包
                        tran2[i][j][r]=1;//i--(空串)-->u--(j)-->v--(空串)-->r
                    }
                }
            }
    cout<<"\n对应的DFA如下,01串从左往右依次表示状态0,1,2...该位为0表示对应的状态不在此集合中,为1表示存在\n";
    cout<<"\t";
    for (int i=0;i<num_of_cin-1;i++)
        cout<<char(i+'a')<<"\t";
    string str(n,'0');
    str[0]='1';
    que.push(str);
    mp.insert(make_pair(str,cnt++));//NFA到DFA的转换法:先从状态0开始
    while(!que.empty())
    {
        string stt=que.front();//构造该集合的转移方程
        que.pop();
        cout<<"\n"<<stt<<"\t";
        for (int i=1;i<num_of_cin;i++)//对于输入i
        {
            string tem(n,'0');
            for (int j=0;j<n;j++)
            {
                if (stt[j]=='0')
                    continue;//该状态集合不含状态j
                for (int k=0;k<n;k++)
                    if (tran2[j][i][k])
                        tem[k]='1';//j能在输入i时转移到k
            }
            cout<<tem<<"\t";
            if (tem!=string(n,'0')&&mp.find(tem)==mp.end())//这个集合不为空且没出现过
            {
                que.push(tem);
                mp.insert(make_pair(tem,cnt++));
            }
        }
    }
    cout<<"\n其中终止状态为:";
    for (int i=0;i<n;i++)
        if (is_F[i])
            cout<<i<<" ";
    return 0;
}

//3 1 2 4 1 1 1 0 0 0 1 2 0 1 1 0 0 0 0 1 2