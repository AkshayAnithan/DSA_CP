class Solution {
   public:
    int longestConsecutive(vector<int>& nums) {
        unordered_set<int> s;
        int longest = 0;
        for (auto& n : nums) {
            s.insert(n);
        }

        for (auto& item : s) {
            if (!s.count(item - 1)) {
                int length = 0;
                while (s.count(item + length)) {
                    length++;
                }
                longest = max(length, longest);
            }
        }
        return longest;
    }
};
