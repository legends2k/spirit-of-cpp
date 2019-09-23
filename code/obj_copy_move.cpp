#include <cstdint>
#include <iostream>
#include <algorithm>

struct Image {
  int w = 0, h = 0;
  uint32_t* pixels = nullptr; // rgba

  int size() const { return w * h; }

 Image(int width, int height)
   : w(width), h(height) { std::cout << __LINE__ << '\n'; }
 Image() : Image(0, 0) { std::cout << __LINE__ << '\n'; }
 ~Image() { if (pixels)
               delete [] pixels; }
};

int main() {
  // make an image, allocate pixel data
  Image owner(4, 4);
  owner.pixels = new uint32_t[owner.size()];
  Image clone(owner.w, owner.h);
  clone.pixels = owner.pixels; // BAD! clone.~Image will try delete on dangling ptr
  // deep copy - byte by byte copy: each image gets its own copy of pixels
  clone.pixels = new uint32_t[owner.size()];
  std::copy_n(owner.pixels, owner.size(), clone.pixels);
  // move: no allocation, no deep copy
  Image thief;
  thief.w = owner.w; thief.h = owner.h;
  std::swap(thief.pixels, owner.pixels);
  owner.w = owner.h = 0;                  // be a responsible thief ;)
}
