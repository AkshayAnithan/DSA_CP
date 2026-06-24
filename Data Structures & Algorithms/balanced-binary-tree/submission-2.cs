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
    public bool IsBalanced(TreeNode root) {
        bool result = true;
        DFS(root, ref result);
        return result;
    }

    public int DFS(TreeNode root, ref bool result) {
        if (root == null)
            return 0;

        int left = DFS(root.left, ref result);
        int right = DFS(root.right, ref result);
        Console.WriteLine($"{left} {right}");
        bool isNodeBalanced = Math.Abs(left - right) > 1 ? false : true;
        result = isNodeBalanced & result;
        Console.WriteLine(result);
        return 1 + Math.Max(left, right);
    }
}
