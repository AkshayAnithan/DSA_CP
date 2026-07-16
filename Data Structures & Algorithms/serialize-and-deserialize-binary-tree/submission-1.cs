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

public class Codec {
    // Encodes a tree to a single string.
    public string Serialize(TreeNode root) {
        List<string> res = new List<string>();
        serializeDfs(root, res);
        return String.Join(",", res);
    }

    private void serializeDfs(TreeNode node, List<string> res) {
        if (node == null) {
            res.Add("N");
            return;
        }
        res.Add(node.val.ToString());
        serializeDfs(node.left, res);
        serializeDfs(node.right, res);
    }

    // Decodes your encoded data to tree.
    public TreeNode Deserialize(string data) {
        string[] vals = data.Split(',');
        int i = 0;
        return deserializeDfs(vals, ref i);
    }

    TreeNode deserializeDfs(string[] vals, ref int i) {
        if (vals[i] == "N") {
            i++;
            return null;
        }
        TreeNode node = new TreeNode(Int32.Parse(vals[i]));
        i++;
        node.left = deserializeDfs(vals, ref i);
        node.right = deserializeDfs(vals, ref i);
        return node;
    }
}
