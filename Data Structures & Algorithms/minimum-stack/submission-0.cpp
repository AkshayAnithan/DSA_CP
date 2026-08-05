class MinStack {
   public:
    stack<int> s;
    MinStack() {}

    void push(int val) { s.push(val); }

    void pop() { s.pop(); }

    int top() { return s.top(); }

    int getMin() {
        stack<int> temp;
        temp = s;
        int minVal = temp.top();

        while (!temp.empty()) {
            int val = temp.top();
            minVal = min(minVal, val);
            temp.pop();
        }
        return minVal;
    }
};
