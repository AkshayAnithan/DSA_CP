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
    public int DFS(TreeNode root, ref int result) {
        if (root == null)
            return 0;
        int left = DFS(root.left, ref result);
        int right = DFS(root.right, ref result);
        result = Math.Max(result, left + right);
        Console.WriteLine($"result: {result} left: {left} right: {right}");
        return 1 + Math.Max(left, right);
    }

    public int DiameterOfBinaryTree(TreeNode root){
        int res = 0;
        DFS(root, ref res);
        return res;
    }
}
