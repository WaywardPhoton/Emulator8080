#include <stdio.h>
#include <stdlib.h>



struct TreeNode {
     int val;
     struct TreeNode *left;
      struct TreeNode *right;
 };


int *enArray = NULL;   
int i = 0; 
int* inorderTraversal(struct TreeNode* root, int* returnSize){

    if( enArray == NULL){
    enArray=(int*)malloc(sizeof(int)*(100));
    }



    if(root != NULL){
        inorderTraversal(root->left,returnSize);
        enArray[i]= root->val;
        printf("%d ", enArray[i]);
        i = i +1;
        inorderTraversal(root->right, returnSize);

    }
    return enArray;

}

int main() {
    struct TreeNode threeTN = {.val = 3, .left = NULL, .right = NULL};
    struct TreeNode twoTN = {.val = 2, .left = &threeTN, .right = NULL};
    struct TreeNode root = {.val = 1, .left = NULL, .right = &twoTN};

    int rs = 3;

    int* res = inorderTraversal(&root, &rs);

    return 0;
}
