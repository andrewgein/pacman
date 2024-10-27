#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../GameData.hpp"
#include <doctest.h>

TEST_CASE("testing ability to add point") {
  GhostData data;
  CHECK(data.path.size() == 0);
  data.path.push_back((cpoint_t){0, 0});
  CHECK(data.path.size() == 1);
}
