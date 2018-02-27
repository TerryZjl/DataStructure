#include<iostream>
#include<vector>
#include<string>
using namsespace std;

void addstr(string s1,string s2, string sum)
{
    int i = s1.size()-1;
    int j = s2.size()-1;
    int c = 0; //进位
    while(i>=0||j>=0)
    {
        int a = i>=0 ? s1[i--] - '0' : 0;
        int b = j>=0 ? s2[j--] - '0': 0;
        int num = a+b+c;
        sum.insert(sum.begin(), sum%10+'0');
        c = num /10;
    }
    if(c!=0)
    {
        sum.insert(sum.begin(),c+'0');
    }
}
int main()
{
    string sum = "";
    string s1 = "1234567";
    string s2 = "45678";
    addstr(s1,s1,sum);
    return 0;    
}