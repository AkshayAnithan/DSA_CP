class Solution {
   public:
    void dfs(int r, int c, vector<vector<char>>& grid, vector<vector<bool>>& visited) {
        int rows = grid.size();
        int cols = grid[0].size();

        if (r < 0 || c < 0 || r >= rows || c >= cols || visited[r][c] || grid[r][c] == '0') {
            return;
        }
        visited[r][c] = true;
        dfs(r + 1, c, grid, visited);
        dfs(r, c + 1, grid, visited);
        dfs(r - 1, c, grid, visited);
        dfs(r, c - 1, grid, visited);
    }

    int numIslands(vector<vector<char>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();
        int islands = 0;

        vector<vector<bool>> visited(rows, vector<bool>(cols, false));

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == '1' && !visited[r][c]) {
                    islands++;
                    dfs(r, c, grid, visited);
                }
            }
        }

        return islands;
    }
};
