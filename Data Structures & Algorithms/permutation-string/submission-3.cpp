class Solution {
   public:
    bool checkInclusion(string s1, string s2) {
        if (s1.length() > s2.length()) return false;

        unordered_map<int, int> h1(26);
        unordered_map<int, int> h2(26);
        for (int i = 0; i < s1.length(); i++) {
            h1[s1[i] - 'a']++;
            h2[s2[i] - 'a']++;
        }

        int l = 0;
        for (int r = s1.length(); r < s2.length(); r++) {
            if (h1 == h2) return true;
            h2[s2[l] - 'a']--;
            if (h2[s2[l] - 'a'] == 0) h2.erase(s2[l] - 'a');
            l++;
            h2[s2[r] - 'a']++;
        }
        return h1 == h2;
    }
};
