#include <future>
#include <iostream>
#include <random>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>

#define gettid() syscall(SYS_gettid)

std::pair<size_t, size_t> thread_func(size_t iters) {
    size_t counter = 0;
    // unsigned int tid = gettid();
    // double rand_num = rand_r(&tid);
    // std::random_device rd;
    // std::mt19937 gen(rd());
    std::default_random_engine re(
        std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    for (size_t i = 0; i < iters; ++i) {
        double x = dist(re);
        double y = dist(re);
        if (x * x + y * y <= 1.0) {
            ++counter;
        }
    }
    return std::make_pair(counter, iters);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        perror("needs 3 args\n");
        exit(1);
    }
    size_t nthreads = strtol(argv[1], NULL, 10);
    size_t iters = strtol(argv[2], NULL, 10);
    std::vector<std::future<std::pair<size_t, size_t>>> futures;
    for (size_t i = 0; i < nthreads; ++i) {
        futures.push_back(std::async(std::launch::async, thread_func, iters));
    }
    size_t in = 0;
    size_t out = 0;
    for (auto &x : futures) {
        auto res = x.get();
        in += res.first;
        out += res.second;
    }
    double pi = 4.0 * static_cast<double>(in) / static_cast<double>(out);

    std::cout.precision(5);
    std::cout << std::fixed << pi << std::endl;
    return 0;
}