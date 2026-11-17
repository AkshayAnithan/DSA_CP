class Solution {
   public:
    bool checkInclusion(string s1, string s2) {
        if (s1.length() > s2.length()) return false;

        sort(s1.begin(), s1.end());
        unordered_map<char, int> h1;
        int n = s1.length();
        int m = s2.length();
        for (auto c : s1) {
            h1[c]++;
        }
        int i = 0, j = i + s1.length() - 1;
        while (j < m) {
            string substr = s2.substr(i, j - i + 1);
            sort(substr.begin(), substr.end());
            auto h2 = unordered_map<char, int>();
            for (auto c : substr) {
                h2[c]++;
            }
            bool check = h1 == h2;
            cout << "i: " << i << " j: " << j << " check: " << check << endl;
            for (auto hs : h1) {
                cout << hs.first << ":" << hs.second << ",";
            }
            cout << endl;
            for (auto hs : h2) {
                cout << hs.first << ":" << hs.second << ",";
            }
            cout << endl;

            if (check) return true;
            i++;
            j++;
            h2.clear();
        }
        return false;
    }
};
