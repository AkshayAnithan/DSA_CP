class Solution {
   public:
    int lengthOfLongestSubstring(string str) {
        set<char> s;
        int maxLength = 0;
        int left = 0, n = str.size();

        for (int right = 0; right < n; right++) {
            while (s.contains(str[right]) && left <= right) {
                s.erase(str[left]);
                left++;
            }
            s.insert(str[right]);
            maxLength = max(maxLength, right - left + 1);
        }
        return maxLength;
    }
};
