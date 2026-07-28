class Solution {
   public:
    bool isAnagram(string s, string t) {
        unordered_map<char, int> hs1, hs2;
        for (auto c : s) {
            hs1[c]++;
        }
        for (auto c : t) {
            hs2[c]++;
        }
        return hs1 == hs2;
    }
};
