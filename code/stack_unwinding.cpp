// http://coliru.stacked-crooked.com/a/7a2ece5e08c85380

#include <iostream>
#include <memory>
#include <stdexcept>

struct Base {
  Base(float f): id(f) {
    std::cout << __func__ << ": " << id << '\n';
  }

  ~Base() {
    std::cout << __func__ << ": " << id << '\n';
  }

  float id = 0.0;
};

void h() {
  throw std::invalid_argument{"Oops!"};
}

void g() {
  Base local{2.1};
  std::unique_ptr<Base> smart = std::make_unique<Base>(100);

  Base *raw = new Base{-1.0f};  // Manual memory handling by directly calling
  h();                          // new and delete, instead of being smart.
  delete raw;   // This would never get called if h() throws, memory leak!!
                // However local and smart would get deleted properly.
}

void f() {
  Base local{1.1};
  g();
}

int main()
{
  Base local1{0.1};
  try {
    Base local{0.2};
    f();
  }
  catch(...) {
    std::cout << "Caught!\n";
  }
}
