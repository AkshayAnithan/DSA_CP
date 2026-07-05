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
    int kthSmallest(TreeNode* root, int k) {
        vector<int> a;
        inorder(root, a);
        sort(a.begin(), a.end());
        return a[k - 1];
    }

    void inorder(TreeNode* node, vector<int>& a) {
        if (!node) return;
        inorder(node->left, a);
        a.push_back(node->val);
        inorder(node->right, a);
    }
};
