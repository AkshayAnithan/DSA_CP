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
    public int res;
    public int MaxPathSum(TreeNode root) {
        res = root.val;
        dfs(root);
        return res;
    }

    public int dfs(TreeNode root) {
        if (root == null)
            return 0;
        int leftMax = dfs(root.left);
        int rightMax = dfs(root.right);
        leftMax = Math.Max(leftMax, 0);
        rightMax = Math.Max(rightMax, 0);
        res = Math.Max(res, root.val + leftMax + rightMax);
        return root.val + Math.Max(leftMax, rightMax);
    }
}
