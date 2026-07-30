class Solution {
   public:
    bool isPalindrome(string s) {
        vector<char> arr;
        for (auto& c : s) {
            if (isalnum(c)) {
                arr.push_back(tolower(c));
                cout << c;
            }
        }
        int n = arr.size();
        for (int i = 0; i < n / 2; i++) {
            if (arr[i] != arr[n - 1 - i]) {
                return false;
            }
        }
        return true;
    }
};
