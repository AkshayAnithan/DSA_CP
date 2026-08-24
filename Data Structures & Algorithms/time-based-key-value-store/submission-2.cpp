class TimeMap {
   public:
    unordered_map<string, vector<pair<int, string>>> hashmap;
    TimeMap() {}

    void set(string key, string value, int timestamp) {
        hashmap[key].push_back({timestamp, value});
    }

    string get(string key, int timestamp) {
        const vector<pair<int, string>>& search = hashmap[key];
        int l = 0, r = search.size() - 1;
        int idx = -1;
        while (l <= r) {
            int m = l + (r - l) / 2;
            int currentTimestamp = search[m].first;

            if (currentTimestamp <= timestamp) {
                idx = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        return idx == -1 ? "" : search[idx].second;
    }
};
