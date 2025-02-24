#include <gtest/gtest.h>

#include <optional>

#include "stack_with_min.h"  // Include your stack header file


enum class simple_stack_ret {
    SUCCESS,
    EMPTY_STACK,
};

class simple_stack_with_min {
public:
    simple_stack_with_min() = default;
    void push(int value) {
        stack.push_back(value);
    }
    simple_stack_ret pop() {
        if (stack.empty()) {
            return simple_stack_ret::EMPTY_STACK;
        }
        stack.pop_back();
        return simple_stack_ret::SUCCESS;
    }
    std::optional<int> top() const {
        if (stack.empty()) {
            std::nullopt;
        }
        return stack.back();
    }
    std::optional<int> get_min() const {
        if (stack.empty()) {
            return std::nullopt;
        }
        int min = stack.front();
        for (auto&& value : stack) {
            min = std::min(min, value);
        }
        return min;
    }
    bool empty() const {
        return stack.empty();
    }
    int size() const {
        return stack.size();
    }
private:
    std::vector<int> stack;

};

// Test fixture for Stack
class CompareStackTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }

    stack_ns::StackWithMin<int> stack;  
    simple_stack_with_min simple_stack;

    void push(int value) {
        stack.push(value);
        simple_stack.push(value);
    }

    void pop() {
        try {
            stack.pop();
            EXPECT_EQ(simple_stack.pop(), simple_stack_ret::SUCCESS);
        } catch (const std::runtime_error &err) {
            EXPECT_EQ(simple_stack.pop(), simple_stack_ret::EMPTY_STACK);
        }
    }

    void top() {
        try {
            auto stack_top = stack.top();
            EXPECT_EQ(stack_top, simple_stack.top().value());
        } catch (const std::runtime_error &err) {
            EXPECT_EQ(simple_stack.top(), std::nullopt);
        }
    }

    void get_min() {
        try {
            auto stack_min = stack.get_min();
            EXPECT_EQ(stack_min, simple_stack.get_min().value());
        } catch (const std::runtime_error &err) {
            EXPECT_EQ(simple_stack.get_min(), std::nullopt);
        }
    }
};

// Test for pushing elements onto the stack
TEST_F(CompareStackTest, CreateStack) {
    push(1);
    top();
    push(2);
    top();
    get_min();
}
