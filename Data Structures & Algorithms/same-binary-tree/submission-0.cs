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
    public bool IsSameTree(TreeNode p, TreeNode q) {
        return check(p, q);
    }

    public bool check(TreeNode a, TreeNode b) {
        if (a == null && b == null)
            return true;

        if (a != null && b != null && a.val == b.val) {
            bool left = check(a.left, b.left);
            bool right = check(a.right, b.right);
            return left && right;
        }
        return false;
    }
}
