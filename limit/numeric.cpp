#include <iostream>
#include <limits>

int main()
{
  std::cout << std::boolalpha;
  std::cout << "Mininum value for int: " << std::numeric_limits<int>::min() << std::endl;
}
