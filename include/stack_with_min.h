#pragma once

#include <memory>
#include <stack>

namespace stack_ns {

template <typename T>
class StackWithMin {
private:
    struct Node {
        template <class U>
        requires std::is_convertible_v<U, T>
        Node(U&& value) : value(std::forward<U>(value)), min(nullptr) {}

        template <class... U>
        requires std::is_constructible_v<T, U...>
        Node(U&&... value) : value(std::forward<U>(value)...), min(nullptr) {}

        T value;
        const T* min = nullptr;
    };
    std::stack<Node> stack;
public:
    StackWithMin() = default;
    ~StackWithMin() = default;

    template <class U>
    requires std::is_convertible_v<U, T>
    void push(U&& value) {  
        auto* last_top = empty() ? nullptr : stack.top().min;
        stack.push(std::forward<U>(value));
        if (last_top == nullptr) {
            stack.top().min = &stack.top().value;
        } else {
            stack.top().min = (stack.top().value < *last_top) ? &stack.top().value : last_top;
        }
    }

    template <class... U>
    requires std::is_constructible_v<T, U...>
    void emplace(U&&... value) {
        auto* last_top = empty() ? nullptr : stack.top().min;
        stack.emplace(std::forward<U>(value)...);
        if (last_top == nullptr) {
            stack.top().min = &stack.top().value;
        } else {
            stack.top().min = (stack.top().value < *last_top) ? &stack.top().value : last_top;
        }
    }


    void pop() {
        if (empty()) {
            throw std::runtime_error("empty stack");
        }
        stack.pop();
    }

    const T& top() const {
        if (empty()) {
            throw std::runtime_error("empty stack");
        }
        return stack.top().value;
    }

    const T& get_min() const {
        if (empty()) {
            throw std::runtime_error("empty stack");
        }
        return *stack.top().min;
    }

    bool empty() const {
        return stack.empty();
    }

    auto size() const {
        return stack.size();
    }
};

}  // namespace stack_ns
