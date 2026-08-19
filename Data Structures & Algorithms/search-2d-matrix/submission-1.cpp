class Solution {
   public:
    bool searchMatrix(vector<vector<int>>& matrix, int target) {
        int ROWS = matrix.size();
        int COLS = matrix[0].size();

        int top = 0, bottom = ROWS - 1;
        while (top <= bottom) {
            int row = (top + bottom) / 2;
            if (matrix[row][COLS - 1] < target) {
                top = row + 1;
            } else if (matrix[row][0] > target) {
                bottom = row - 1;
            } else {
                break;
            }
        }

        if (!(top <= bottom)) {
            return false;
        }

        int row = (top + bottom) / 2;
        int l = 0, r = COLS - 1;
        while (l <= r) {
            int m = (l + r) / 2;
            if (target > matrix[row][m]) {
                l = m + 1;
            } else if (target < matrix[row][m]) {
                r = m - 1;
            } else {
                return true;
            }
        }
        return false;
    }
};
