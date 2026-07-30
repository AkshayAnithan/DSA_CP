class Solution {
   public:
    bool isPalindrome(string s) {
        string res = "";
        for (auto& c : s) {
            if (isalnum(c)) res += tolower(c);
        }
        return res == string(res.rbegin(), res.rend());
    }
};
