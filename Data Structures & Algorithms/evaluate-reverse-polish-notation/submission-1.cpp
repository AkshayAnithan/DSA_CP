class Solution {
   public:
    int evalRPN(vector<string>& tokens) {
        stack<int> s;

        for (string& c : tokens) {
            if (c == "+" || c == "-" || c == "*" || c == "/") {
                int op1 = s.top();
                s.pop();
                int op2 = s.top();
                s.pop();
                int val = calc(op1, op2, c[0]);
                s.push(val);
            } else {
                s.push(stoi(c));
            }
        }
        return s.top();
    }

   private:
    int calc(int op1, int op2, char op) {
        switch (op) {
            case '+':
                return op2 + op1;
            case '-':
                return op2 - op1;
            case '*':
                return op2 * op1;
            case '/':
                return op2 / op1;
            default:
                return 0;
        }
    }
};
