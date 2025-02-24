#pragma once

#include <memory>
#include <stack>
#include <optional>

namespace stack_ns {

namespace detail {
template <typename T, typename Less>
struct Node {
    template <class U>
    requires std::is_convertible_v<U, T>
    Node(U&& value) : value(std::forward<U>(value)) {}

    template <class... U>
    requires std::is_constructible_v<T, U...>
    Node(U&&... value) : value(std::forward<U>(value)...) {}

    void set_min(const Node* prev) {
        if (prev == nullptr || Less{}(value, *prev->min)) {
            min = &value;
        } else {
            min = prev->min;
        }
    }

    const T& get_min() const {
        return *min;
    }

    T value;
private:
    const T* min = nullptr;
};

// specialization for trivially copyable types
//   store minimun value explicitly for small simple types
template <typename T, typename Less>
requires std::is_trivially_copyable_v<T> && std::is_trivially_destructible_v<T> && std::copy_constructible<T> && (sizeof(T) <= sizeof(T*))
struct Node<T, Less> {
    template <class U>
    requires std::is_convertible_v<U, T>
    Node(U&& value) : value(std::forward<U>(value)) {}

    template <class... U>
    requires std::is_constructible_v<T, U...>
    Node(U&&... value) : value(std::forward<U>(value)...) {}

    void set_min(const Node* prev) {
        if (prev == nullptr || Less{}(value, prev->min)) {
            min = value;
        } else {
            min = prev->min;
        }
    }

    const T& get_min() const {
        return min;
    }

    T value;
private:
    T min;
};

static_assert(sizeof(Node<char, std::less<char>>) == sizeof(char) * 2);

} // namespace detail


template <typename T, typename Less = std::less<T>, typename Container = std::stack<T>>
class StackWithMin {
private:
    std::stack<detail::Node<T, Less>> stack;
public:
    using node_t = detail::Node<T, Less>*;
    StackWithMin() = default;
    ~StackWithMin() = default;

    template <class U>
    requires std::is_convertible_v<U, T>
    void push(U&& value) {  
        node_t prev_node = stack.empty() ? nullptr : &stack.top();
        // Prefer move semantics if possible
        if constexpr (std::is_nothrow_move_constructible_v<T> && !std::is_copy_constructible_v<T>) {
            stack.push(std::move(value));
        } 
        // Prefer copy semantics for exception safety
        if constexpr (!(std::is_nothrow_move_constructible_v<T> && !std::is_copy_constructible_v<T>)) {
            stack.push(value);
        }
        stack.top().set_min(prev_node);
    }

    template <class... U>
    requires std::is_constructible_v<T, U...>
    void emplace(U&&... value) {
        node_t prev_node = stack.empty() ? nullptr : &stack.top();
        stack.emplace(std::forward<U>(value)...);
        stack.top().set_min(prev_node);
    }

    void pop() {
        if (empty()) {
            throw std::runtime_error("empty stack");
        }
        stack.pop();
    }

    enum class pop_result {
        success,
        empty_stack,
    };

    pop_result try_pop() noexcept {
        if (empty()) {
            return pop_result::empty_stack;
        }
        stack.pop();
        return pop_result::success;
    }

    const T& top() const {
        if (empty()) {
            throw std::runtime_error("empty stack");
        }
        return stack.top().value;
    }

    std::optional<std::reference_wrapper<const T>> try_top() const noexcept {
        if (empty()) {
            return std::nullopt;
        }
        return stack.top().value;
    }

    const T& get_min() const {
        if (empty()) {
            throw std::runtime_error("empty stack");
        }
        return stack.top().get_min();
    }

    std::optional<std::reference_wrapper<const T>> try_get_min() const noexcept {
        if (empty()) {
            return std::nullopt;
        }
        return stack.top().get_min();
    }

    bool empty() const {
        return stack.empty();
    }

    auto size() const {
        return stack.size();
    }
};

}  // namespace stack_ns
