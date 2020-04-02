#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <string>
#include <mutex>

std::mutex m;
struct X
{
  void foo(int i, const std::string &str)
  {
    std::lock_guard<std::mutex> lk(m);
    std::cout << str << ' ' << i << '\n';
  }
  void bar(const std::string &str)
  {
    std::lock_guard<std::mutex> lk(m);
    std::cout << str << '\n';
  }
  int operator()(int i)
  {
    std::lock_guard<std::mutex> lk(m);
    std::cout << i << '\n';
    return i + 10;
  }
};

template <typename RandomIt>
int parallel_sum(RandomIt beg, RandomIt end)
{
  auto len = end - beg;
  if (len < 1000)
    return std::accumulate(beg, end, 0);

  RandomIt mid = beg + len / 2;
  auto handle = std::async(std::launch::async, parallel_sum<RandomIt>, mid, end);

  int sum = parallel_sum(beg, mid);
  return sum + handle.get();
}

int main()
{
  std::vector<int> v(10000, 1);
  std::cout << "The sum is " << parallel_sum(v.begin(), v.end()) << '\n';

  X x;
  auto a1 = std::async(&X::foo, &x, 42, "Hello");
  auto a2 = std::async(std::launch::deferred, &X::bar, &x, "world!");
  auto a3 = std::async(std::launch::async, X(), 43);
  a2.wait();
  return 0;
}
