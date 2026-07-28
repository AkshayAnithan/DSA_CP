public class Solution {
    public bool IsAnagram(string s, string t) {
        char[] s_sort = s.ToCharArray();
        char[] t_sort = t.ToCharArray();
        Array.Sort(s_sort);
        Array.Sort(t_sort);
        return s_sort.SequenceEqual(t_sort);
    }
}
