class Solution {
   public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        unordered_map<string, vector<string>> hs;
        for (auto s : strs) {
            string t = s;
            sort(t.begin(), t.end());
            hs[t].push_back(s);
        }
        vector<vector<string>> result;

        for (auto ele : hs) {
            result.push_back(ele.second);
        }
        return result;
    }
};
