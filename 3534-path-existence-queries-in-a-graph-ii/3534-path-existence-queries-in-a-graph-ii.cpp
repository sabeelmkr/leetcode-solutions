class Solution {
public:
    vector<int> pathExistenceQueries(
        int n,
        vector<int>& nums,
        int maxDiff,
        vector<vector<int>>& queries
    ) {
        // {value, original index}
        vector<pair<int, int>> a(n);

        for (int i = 0; i < n; i++) {
            a[i] = {nums[i], i};
        }

        sort(a.begin(), a.end());

        // Position of each original node in sorted order
        vector<int> pos(n);

        for (int i = 0; i < n; i++) {
            pos[a[i].second] = i;
        }

        // far[i] = farthest sorted position reachable
        // from i in exactly one edge
        vector<int> far(n);

        int r = 0;

        for (int i = 0; i < n; i++) {
            if (r < i)
                r = i;

            while (r + 1 < n &&
                   (long long)a[r + 1].first - a[i].first <= maxDiff) {
                r++;
            }

            far[i] = r;
        }

        // Binary lifting table
        int LOG = 1;
        while ((1 << LOG) <= n)
            LOG++;

        vector<vector<int>> jump(LOG, vector<int>(n));

        for (int i = 0; i < n; i++) {
            jump[0][i] = far[i];
        }

        for (int k = 1; k < LOG; k++) {
            for (int i = 0; i < n; i++) {
                jump[k][i] =
                    jump[k - 1][jump[k - 1][i]];
            }
        }

        vector<int> answer;

        for (auto& q : queries) {
            int left = pos[q[0]];
            int right = pos[q[1]];

            if (left > right)
                swap(left, right);

            if (left == right) {
                answer.push_back(0);
                continue;
            }

            int current = left;
            int steps = 0;

            // Take the largest jumps that still stay
            // before the destination
            for (int k = LOG - 1; k >= 0; k--) {
                if (jump[k][current] < right &&
                    jump[k][current] != current) {

                    current = jump[k][current];
                    steps += (1 << k);
                }
            }

            // One final jump to destination
            if (far[current] >= right) {
                answer.push_back(steps + 1);
            } else {
                answer.push_back(-1);
            }
        }

        return answer;
    }
};