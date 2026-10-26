class Solution {
   public:
    int characterReplacement(string s, int k) {
        int left = 0, maxLength = 0;
        vector<int> frequency(26, 0);
        int result = 0, windowSize = 0;

        for (int right = 0; right < s.length(); right++) {
            frequency[s[right] - 'A']++;
            maxLength = max(maxLength, frequency[s[right] - 'A']);
            windowSize = right - left + 1;
            while (windowSize - maxLength > k) {
                frequency[s[left] - 'A']--;
                left++;
                windowSize = right - left + 1;
            }
            result = max(result, windowSize);
        }

        return result;
    }
};
