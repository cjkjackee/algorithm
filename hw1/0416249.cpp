#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h>
using namespace std;

// variable
vector<long> young_tableau;
int col, row;

// function
void insert (int);
int extract_min ();
void show();

int main ()
{
    fstream fin;
    char c;
    int T;
    int func;
    vector<int> input;

    fin.open("input.txt",fstream::in);

    fin >> T;
    for(int i=0;i<T;++i)
    {
        input.clear();
        young_tableau.clear();
        young_tableau.push_back(LONG_MAX);
        row = 0;
        col = 0;
        int tmp;

        // get method
        fin >> func;
        switch(func)
        {
            case 1:
            {

                // get input
                fin.get(c);
                if (c<'0' || c>'9')
                {
                    while(fin.get(c))
                        if (c==10)
                            break;
                }

                while((fin.get(c)))
                {
                    if (c==10)
                        break;

                    if (c>='0' && c<='9')
                    {
                        fin.putback(c);
                        fin >> tmp;
                        input.push_back(tmp);
                    }
                }

                // get table
                while(1) 
                {
                    fin.get(c);
                    if (c==10 || c==13)
                        break;
                    fin.putback(c);
                    ++row;
                    
                    while((fin.get(c)))
                    {
                        if (c==10)
                            break;

                        if (c=='x')
                        {
                            young_tableau.push_back(LONG_MAX);
                            if (row==1)
                                ++col;
                        }
                        else if (c>='0' && c<='9')
                        {
                            fin.putback(c);
                            fin >> tmp;
                            young_tableau.push_back(tmp);
                            if (row==1)
                                ++col;
                        }
                    }
                }
                
                // output
                cout << "Insert";
                vector<int>::iterator it;
                for (it=input.begin();it!=input.end();++it)
                {
                    cout << " " << *it;
                    insert(*it);
                }
                cout << endl;
                show();
                break;
            }
            case 2:
            {
                // get table
                fin.get(c);
                if (c<'0' || c>'9')
                {
                    while(fin.get(c))
                        if (c==10)
                            break;
                }

                while(1) 
                {
                    fin.get(c);
                    if (c==10 || c==13)
                        break;
                    fin.putback(c);
                    ++row;
                    
                    while((fin.get(c)))
                    {
                        if (c==10)
                            break;

                        if (c=='x')
                        {
                            young_tableau.push_back(LONG_MAX);
                            if (row==1)
                                ++col;
                        }
                        else if (c>='0' && c<='9')
                        {
                            fin.putback(c);
                            fin >> tmp;
                            young_tableau.push_back(tmp);
                            if (row==1)
                                ++col;
                        }
                    }
                }

                // output
                cout << "Extract-min " << extract_min() << endl;
                show();
                break;
            }
            default: break;
        }
    }

    return 0;
}

int max_pos (int n1, int n2)
{
    if(young_tableau[n1] >= young_tableau[n2])
        return n1;
    return n2;
}

int min_pos (int n1, int n2)
{
    if (young_tableau[n1] <= young_tableau[n2])
        return n1;
    return n2;
}

void insert (int no)
{
    int max;
    int now, target;
    int now_row, now_col;
    int left, up;

    for (int i=1;i<=(col*row);++i)
    {
        if (young_tableau[i]==LONG_MAX)
        {
            now = i;
            young_tableau[now] = no;
            break;
        }
    }

    if (now == 1)
        return;

    while(1)
    {
        now_row = now / col;
        now_col = now % col;
        up = ((now_row-1)*col) + now_col;
        left = (now_row*col) + now_col-1;

        if (now_row == 0)
            target = max_pos(left,now);
        else if (now_col == 1)
            target = max_pos(up,now);
        else 
        {
            max = max_pos(left,up);
            target = max_pos(max, now);
        }

        if (target == now)
            break;

        int tmp = young_tableau[now];
        young_tableau[now] = young_tableau[target];
        young_tableau[target] = tmp;

        now = target;
    }

    return;
}

int extract_min ()
{
    int no;
    int now, target;
    int now_row, now_col;
    int down, right;

    now = 1;
    no = young_tableau[1];

    while(1)
    {
        now_row = now / col;
        now_col = now % col;

        down = ((now_row+1)*col) + now_col;
        right = (now_row*col) + now_col+1;

        if (now_row == row)
            target = right;
        else if (now_col == 0)
            target = down;
        else
            target = min_pos(right,down);

        if (young_tableau[target]==LONG_MAX || now == (row*col))
        {
            young_tableau[now] = LONG_MAX;
            break;
        }

        young_tableau[now] = young_tableau[target];

        now = target;
    }
   
    return no;
}

void show ()
{
    int i = 0;
    for(int x=1;x<=(row*col);++x)
    {
        if (young_tableau[x]==LONG_MAX)
            cout << "x ";
        else 
            cout << young_tableau[x] << " ";
        
        if (++i == col)
        {
            cout << endl;
            i = 0;
        }
    }
    cout << endl;
    return ;
}