public class Solution {
    public bool IsAnagram(string s, string t) {
        if (s.Length != t.Length)
            return false;
        Dictionary<char, int> hs1 = new Dictionary<char, int>();
        Dictionary<char, int> hs2 = new Dictionary<char, int>();
        for (int i = 0; i < s.Length; i++) {
            hs1[s[i]] = hs1.GetValueOrDefault(s[i], 0) + 1;
            hs2[t[i]] = hs2.GetValueOrDefault(t[i], 0) + 1;
        }
        return hs1.Count == hs2.Count && !hs1.Except(hs2).Any();
    }
}
