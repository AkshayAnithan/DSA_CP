/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

class Solution {
   public:
    int res;
    int maxPathSum(TreeNode* root) {
        res = root->val;
        dfs(root);
        return res;
    }

    int dfs(TreeNode* root) {
        if (!root) return 0;

        int leftMax = dfs(root->left);
        int rightMax = dfs(root->right);
        leftMax = max(leftMax, 0);
        rightMax = max(rightMax, 0);
        res = max(res, root->val + leftMax + rightMax);
        return root->val + max(leftMax, rightMax);
    }
};
