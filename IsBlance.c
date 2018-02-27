int InOrder(TreeNode root)
{
    if(root == NULL )
        return 0;
    if(root->_left == NULL && root->_right == NULL)
        return 1;
    
    int retleft = InOrder(root->_left);
    if(retleft == -1)
        return -1;
    int retright = InOrder(root->_right);
    if(retright == -1)
        return -1;
    
    int num = retleft - retright;
    if(num < -1 ||  num > 1)
    {
        return -1;
    }
    return retleft > right ? retleft+1 : retright+1;
}