#include <thread>

template <typename F, typename C, typename... Args>
void async_launch(F &&f, C &&c, Args &&... args) {
    std::thread(
        [](auto &&func, auto &&cb, auto &&... args) {
            cb(func(std::forward<decltype(args)>(args)...));
        },
        std::forward<F>(f), std::forward<C>(c), std::forward<Args>(args)...).detach();
}