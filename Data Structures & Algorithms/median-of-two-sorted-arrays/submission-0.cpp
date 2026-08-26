class Solution {
   public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        vector<int>& A = nums1;
        vector<int>& B = nums2;
        if (nums1.size() > nums2.size()) {
            swap(A, B);
        }
        int totalSize = A.size() + B.size();
        int leftHalfSize = (totalSize + 1) / 2;
        int l = 0, r = A.size();

        while (l <= r) {
            int cutA = l + (r - l) / 2;
            int cutB = leftHalfSize - cutA;

            int leftA = cutA == 0 ? INT_MIN : A[cutA - 1];
            int rightA = cutA == A.size() ? INT_MAX : A[cutA];
            int leftB = cutB == 0 ? INT_MIN : B[cutB - 1];
            int rightB = cutB == B.size() ? INT_MAX : B[cutB];
            cout << leftA << " " << leftB << " " << rightA << " " << rightB << endl;

            if (leftA <= rightB && leftB <= rightA) {
                if (totalSize % 2) {
                    return max(leftA, leftB);
                }
                return (max(leftA, leftB) + min(rightA, rightB)) / 2.0;
            } else if (leftA > rightB) {
                r = cutA - 1;
            } else {
                l = cutA + 1;
            }
        }

        return 0;
    }
};
