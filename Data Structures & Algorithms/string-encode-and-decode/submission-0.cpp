class Solution {
   public:
    string encode(vector<string>& strs) {
        string encode;
        for (const auto& str : strs) {
            encode.append(to_string(str.size()));
            encode.push_back('#');
            encode.append(str);
        }
        return encode;
    }

    vector<string> decode(string s) {
        vector<string> decode;
        int i = 0;
        while (i < s.size()) {
            int j = i;
            while (s[j] != '#') j++;
            int length = stoi(s.substr(i, j - i));
            i = j + 1;
            j = i + length;
            decode.push_back(s.substr(i, length));
            i = j;
        }
        return decode;
    }
};
