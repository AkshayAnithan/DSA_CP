/**
 * Definition for a binary tree node.
 * public class TreeNode {
 *     public int val;
 *     public TreeNode left;
 *     public TreeNode right;
 *     public TreeNode(int val=0, TreeNode left=null, TreeNode right=null) {
 *         this.val = val;
 *         this.left = left;
 *         this.right = right;
 *     }
 * }
 */

public class Solution {
    public bool IsValidBST(TreeNode root) {
        return dfs(root, long.MinValue, long.MaxValue);
    }

    public bool dfs(TreeNode node, long left, long right) {
        if (node == null)
            return true;
        if (!(left < node.val && node.val < right))
            return false;
        return dfs(node.left, left, node.val) && dfs(node.right, node.val, right);
    }
}
