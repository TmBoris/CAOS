#include <vector>
#include <thread>

template <typename F, typename C, typename... Args>
void async_launch(F &&f, C &&c, Args &&... args) {
    std::thread(
        [](auto &&func, auto &&cb, auto &&... args) {
            cb(func(std::forward<decltype(args)>(args)...));
        },
        std::forward<F>(f), std::forward<C>(c), std::forward<Args>(args)...).detach();
}

template <class Input, class Output, class Function, class Callback>
void async_vector(const std::vector<Input> &ins, const Function &f,
                  Callback &&c) {
    std::vector<Output> ans;
    for (const auto& x : ins) {
        ans.push_back(f(x));
    }
    c(ans);
}