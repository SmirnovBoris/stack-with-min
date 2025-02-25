#include <gtest/gtest.h>
#include "stack_with_min.h"  // Include your stack header file

class NonCopyable {
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
public:
    NonCopyable() = delete;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator=(NonCopyable&&) = default;
    NonCopyable(int value) : value{value} {}
    operator int() const { return value; }
private:
    int value;
};

template <typename T>
class StackTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }

    stack_ns::StackWithMin<T> stack; 
};


using MyTypes = ::testing::Types<char, int, NonCopyable>;
TYPED_TEST_SUITE(StackTest, MyTypes);

// Test for pushing elements onto the stack
TYPED_TEST(StackTest, CreateStack) {
    return;
}

TYPED_TEST(StackTest, PushStack) {
    TestFixture::stack.push(1);
    TestFixture::stack.push(2);
    TestFixture::stack.push(3);
}

TYPED_TEST(StackTest, Top) {
    try {
        TestFixture::stack.top();
    } catch (const std::runtime_error &err) {
        EXPECT_STREQ(err.what(), "empty stack");
    }
    TestFixture::stack.push(1);
    EXPECT_EQ(TestFixture::stack.top(), 1);
    TestFixture::stack.push(2);
    EXPECT_EQ(TestFixture::stack.top(), 2);
    TestFixture::stack.push(3);
    EXPECT_EQ(TestFixture::stack.top(), 3);
}

TYPED_TEST(StackTest, Size) {
    EXPECT_EQ(TestFixture::stack.size(), 0);
    TestFixture::stack.push(1);
    EXPECT_EQ(TestFixture::stack.size(), 1);
    TestFixture::stack.push(2);
    EXPECT_EQ(TestFixture::stack.size(), 2);
    TestFixture::stack.push(3);
    EXPECT_EQ(TestFixture::stack.size(), 3);
}

TYPED_TEST(StackTest, Pop) {
    try {
        TestFixture::stack.pop();
    } catch (const std::runtime_error &err) {
        EXPECT_STREQ(err.what(), "empty stack");
    }
    TestFixture::stack.push(1);
    TestFixture::stack.push(2);
    TestFixture::stack.push(3);
    TestFixture::stack.pop();
    EXPECT_EQ(TestFixture::stack.size(), 2);
    EXPECT_EQ(TestFixture::stack.top(), 2);
    TestFixture::stack.pop();
    EXPECT_EQ(TestFixture::stack.size(), 1);
    EXPECT_EQ(TestFixture::stack.top(), 1);
    TestFixture::stack.pop();
    EXPECT_EQ(TestFixture::stack.size(), 0);
    try {
        TestFixture::stack.pop();
    } catch (const std::runtime_error &err) {
        EXPECT_STREQ(err.what(), "empty stack");
    }
}

TYPED_TEST(StackTest, GetMin) {
    try {
        TestFixture::stack.get_min();
    } catch (const std::runtime_error &err) {
        EXPECT_STREQ(err.what(), "empty stack");
    }
    TestFixture::stack.push(3);
    EXPECT_EQ(TestFixture::stack.get_min(), 3);
    TestFixture::stack.push(2);
    EXPECT_EQ(TestFixture::stack.get_min(), 2);
    TestFixture::stack.push(4);  
    EXPECT_EQ(TestFixture::stack.get_min(), 2);
    TestFixture::stack.push(1);
    EXPECT_EQ(TestFixture::stack.get_min(), 1);
    TestFixture::stack.pop();
    EXPECT_EQ(TestFixture::stack.get_min(), 2);
}

TYPED_TEST(StackTest, NoexceptTest) {
    TestFixture::stack.push(1);
    TestFixture::stack.push(2);
    TestFixture::stack.push(3);
    EXPECT_EQ(TestFixture::stack.try_pop(), stack_ns::StackWithMin<TypeParam>::pop_result::success);
    EXPECT_EQ(TestFixture::stack.try_pop(), stack_ns::StackWithMin<TypeParam>::pop_result::success);
    EXPECT_EQ(TestFixture::stack.try_pop(), stack_ns::StackWithMin<TypeParam>::pop_result::success);
    EXPECT_EQ(TestFixture::stack.try_pop(), stack_ns::StackWithMin<TypeParam>::pop_result::empty_stack);
    EXPECT_EQ(TestFixture::stack.try_top().has_value(), false);
    EXPECT_EQ(TestFixture::stack.try_get_min().has_value(), false);
}



class WithExplicitConstructor {
public:
    explicit WithExplicitConstructor(int first_value, int second_value) : value{first_value + second_value} {}
    operator int() const { return value; }
    WithExplicitConstructor& operator=(const WithExplicitConstructor&) = delete;
    WithExplicitConstructor(const WithExplicitConstructor&) = delete;
    WithExplicitConstructor(WithExplicitConstructor&&) = delete;
    WithExplicitConstructor& operator=(WithExplicitConstructor&&) = delete;
private:
    int value;
};

class ExplicitTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }

    stack_ns::StackWithMin<WithExplicitConstructor> stack; 
};

TEST_F(ExplicitTest, Emplace) {
    stack.emplace(1, 0);
    stack.emplace(2, 0);
    stack.emplace(3, 1);
    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.top(), 4);
    stack.pop();
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 2);
    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 1);
    stack.pop();
    EXPECT_EQ(stack.size(), 0);
    try {
        stack.pop();
    } catch (const std::runtime_error &err) {
        EXPECT_STREQ(err.what(), "empty stack");
    }
}

class ThrowableMove {
public:
    ThrowableMove(int value) : value{value} {}
    ThrowableMove(ThrowableMove&& other) : value{other.value} {
        if (value == -3) {
            throw std::runtime_error("-3");
        }
    }
    ThrowableMove(const ThrowableMove&) = default;
    int value;
    bool operator<(const ThrowableMove& other) const {
        return value < other.value;
    }
};

class ThrowableMoveTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }

    stack_ns::StackWithMin<ThrowableMove> stack; 
};

TEST_F(ThrowableMoveTest, test) {
    stack.push(1);
    stack.push(2);
    stack.push(3);
    try {
        stack.push(-3);
    } catch (const std::runtime_error &err) {
        EXPECT_STREQ(err.what(), "-3");
    }
}

class TestOnVector : public ::testing::Test {
protected:
    void SetUp() override {
        // Code here will be called immediately after the constructor (right before each test).
    }

    void TearDown() override {
        // Code here will be called immediately after each test (right before the destructor).
    }

    template <typename T>
    class StackOnVector {
    public:
        void push(T value) {
            stack.push_back(value);
        }

        void pop() {
            stack.pop_back();
        }

        const T& top() const {
            return stack.back();
        }

        T& top() {
            return stack.back();
        }

        bool empty() const {
            return stack.empty();
        }

        size_t size() const {
            return stack.size();
        }

        std::vector<T> stack;
    };

    stack_ns::StackWithMin<int, std::less<int>, StackOnVector> stack; 
};

TEST_F(TestOnVector, test) {
    stack.push(1);
    stack.push(2);
    stack.push(3);
    EXPECT_EQ(stack.size(), 3);
    EXPECT_EQ(stack.top(), 3);
    stack.pop();
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 2);
    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 1);
    stack.pop();
    EXPECT_EQ(stack.size(), 0);
    EXPECT_EQ(stack.empty(), true);
}