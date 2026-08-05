class MinStack {
   public:
    stack<int> s;
    stack<int> minStack;
    MinStack() {}

    void push(int val) {
        s.push(val);
        int mini = min(val, minStack.empty() ? val : minStack.top());
        minStack.push(mini);
    }

    void pop() {
        s.pop();
        minStack.pop();
    }

    int top() { return s.top(); }

    int getMin() { return minStack.top(); }
};
