class Solution {
   public:
    bool isValidSudoku(vector<vector<char>>& board) {
        unordered_map<int, unordered_set<char>> rows, cols;
        map<pair<int, int>, unordered_set<char>> squares;

        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                char val = board[r][c];
                if (val == '.') continue;
                if (rows[r].count(val) || cols[c].count(val) ||
                    squares[{r / 3, c / 3}].count(val)) {
                    return false;
                }
                rows[r].insert(val);
                cols[c].insert(val);
                squares[{r / 3, c / 3}].insert(val);
            }
        }
        return true;
    }
};
