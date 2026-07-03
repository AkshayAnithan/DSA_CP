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
    bool isValidBST(TreeNode* root) { return dfs(root, LONG_MIN, LONG_MAX); }

    bool dfs(TreeNode* node, long left, long right) {
        if (!node) return true;
        if (!(left < node->val && node->val < right)) return false;
        return dfs(node->left, left, node->val) && dfs(node->right, node->val, right);
    }
};
