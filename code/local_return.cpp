// BAD
// results in access violation
int* get_value1() {
  // calculate value here
  // …

  int i = 0xdeadbeef; // arrive at some value
  return &i;
} // i gets destroyed when control reaches here

// UGLY
// needs caller to delete i properly; without knowing how it was allocated
// free ≠ delete, delete ≠ delete []
int* get_value2() {
  return new int(i);
}

// GOOD
// caller can acess value and leave it to auto destruct
std::unique_ptr<int> get_value3() {
  return std::make_unique<int>(i);
}

int main() {
  auto p = get_value3();
}
