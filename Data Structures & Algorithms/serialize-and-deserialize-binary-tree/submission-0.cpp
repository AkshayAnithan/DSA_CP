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

class Codec {
   public:
    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        vector<string> res;
        serializeDfs(root, res);
        return join(res, ",");
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        vector<string> vals = split(data, ',');
        int i = 0;
        return deserializeDfs(vals, i);
    }

   private:
    void serializeDfs(TreeNode* node, vector<string>& res) {
        if (!node) {
            res.push_back("N");
            return;
        }
        res.push_back(to_string(node->val));
        serializeDfs(node->left, res);
        serializeDfs(node->right, res);
    }

    TreeNode* deserializeDfs(vector<string>& vals, int& i) {
        if (vals[i] == "N") {
            i++;
            return NULL;
        }
        TreeNode* node = new TreeNode(stoi(vals[i]));
        i++;
        node->left = deserializeDfs(vals, i);
        node->right = deserializeDfs(vals, i);
        return node;
    }

    string join(const vector<string>& v, const string& delim) {
        ostringstream s;
        for (const auto& i : v) {
            if (&i != &v[0]) s << delim;
            s << i;
        }
        return s.str();
    }

    vector<string> split(const string& s, char delim) {
        vector<string> elems;
        stringstream ss(s);
        string item;
        while (getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
};
