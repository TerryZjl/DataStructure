#include<iostream>
using namespace std;

void Order(TreeNode root,int& MaxNum, int Sum)
{
    if(root == NULL)
    {
        return;        
    }
    if(root->_left == NULL && root->_right == NULL)
    {
        if(Sum > MaxNum)
        {
            MaxNum = Sum;
        }
    }
    if( root->_left != NULL)
        Sum+=root->_left->_data;
    Order(root->_left,MaxNum,Sum);
    if(root->_left != NULL)
        Sum-=root->_left->_data;
    
    if(root->_right != NULL)
        Sum+=root->_right->_data;
    Order(root->_right,MaxNum,Sum);
    if(root->_right != NULL)
        Sum -= root->_right->_data;
}

int maxSumPath(TreeNode root)
{
    if(root == NULL)
    {
        return -1;    
    }
    int MaxNum = 0;     //最大值
    int Sum = root->_data; //和
    Order(root, MaxNum, Sum);   
    return MaxNum;
}
int main()
{
    int ret = MaxSumPath(root);
    return 0;    
}