#include "Map.hpp"

Map::Map(SDL_Renderer *renderer) : _blinkTimer(BLINK_TICKS) {
  if (renderer != NULL) {
    _mapTexture.loadImage(SPRITE_SHEET_FILE, renderer, MAP_CLIP_X_POSITION,
                          MAP_CLIP_Y_POSITION, MAP_CLIP_WIDTH, MAP_CLIP_HEIGHT);
    _mapTexture.setOffset(MAP_OFFSET_X, MAP_OFFSET_Y);
    _mapTexture.setCoordinates(0, 0);

    _smallFoodTexture.loadImage(SPRITE_SHEET_FILE, renderer, 1128, 138, 5, 5);
    _smallFoodTexture.setOffset(MAP_OFFSET_X, MAP_OFFSET_Y);
    _bigFoodTexture.loadImage(SPRITE_SHEET_FILE, renderer, 1128, 143, 20, 20);
    _bigFoodTexture.setOffset(MAP_OFFSET_X, MAP_OFFSET_Y);
  }

  mapSchematic = (char **)calloc(BOARD_HEIGHT, sizeof(char *));
  if (mapSchematic == NULL) {
    std::cout << "calloc error" << std::endl;
    exit(1);
  }
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    mapSchematic[i] = (char *)calloc(BOARD_WIDTH, sizeof(char *));
    if (mapSchematic[i] == NULL) {
      std::cout << "calloc error" << std::endl;
      exit(1);
    }
    strcpy(mapSchematic[i], startMapSchematic[i]);
  }
  _renderer = renderer;
  _dotsLeft = START_DOTS_COUNT;
}

bool Map::wallCollision(const point_t &center, const short &collisionFlags) const {

  if ((collisionFlags & CanUseTeleport) == 0) {
    return isInTeleport(center);
  }
  short xMax = ceil((center.x + BLOCK_SIZE / 2.0f - 1) / BLOCK_SIZE) - 1;
  short xMin = floor((center.x - BLOCK_SIZE / 2.0f) / BLOCK_SIZE);
  short yMax = ceil((center.y + BLOCK_SIZE / 2.0f - 1) / BLOCK_SIZE) - 1;
  short yMin = floor((center.y - BLOCK_SIZE / 2.0f) / BLOCK_SIZE);
  bool result = false;
  if ((collisionFlags & CanUseDoor) != 0) {
    mapSchematic[DOOR_POSITION_L.y][DOOR_POSITION_L.x] = ' ';
    mapSchematic[DOOR_POSITION_R.y][DOOR_POSITION_R.x] = ' ';
  }
  if (mapSchematic[yMin][xMin] == '#' || mapSchematic[yMin][xMax] == '#' ||
      mapSchematic[yMax][xMin] == '#' || mapSchematic[yMax][xMax] == '#') {
    result = true;
  }
  if ((collisionFlags & CanUseDoor) != 0) {
    mapSchematic[DOOR_POSITION_L.y][DOOR_POSITION_L.x] = '#';
    mapSchematic[DOOR_POSITION_R.y][DOOR_POSITION_R.x] = '#';
  }
  return result;
}

bool Map::wallCollision(const cpoint_t &center, const short &collisionFlags) const {
  if ((collisionFlags & CanUseTeleport) == 0) {
    return isInTeleport((point_t){center.x * BLOCK_SIZE, center.y * BLOCK_SIZE});
  }
  if ((collisionFlags & CanUseDoor) &&
      (center == DOOR_POSITION_L || center == DOOR_POSITION_R)) {
    return false;
  }
  return mapSchematic[center.y][center.x] == '#';
}

short Map::foodCollision(const point_t &center) {
  cpoint_t cellPosition = (cpoint_t)center;
  char foodType = mapSchematic[cellPosition.y][cellPosition.x];
  mapSchematic[cellPosition.y][cellPosition.x] = ' ';
  switch (foodType) {
  case '.':
    _dotsLeft -= 1;
    return 10;
  case '0':
    _dotsLeft -= 1;
    return 50;
  default:
    return 0;
  }
}

void Map::drawFood() const {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (mapSchematic[i][j] == '.') {
        _smallFoodTexture.draw(j * BLOCK_SIZE + BLOCK_SIZE / 3 + 2,
                               i * BLOCK_SIZE + BLOCK_SIZE / 3 + 2);
      } else if (mapSchematic[i][j] == '0') {
        _bigFoodTexture.draw(j * BLOCK_SIZE + (BLOCK_SIZE - 20) / 2,
                             i * BLOCK_SIZE + (BLOCK_SIZE - 20) / 2);
      }
    }
  }
}

void Map::draw() const {
  if (_isBlinking) {
    // TODO avoid this
    if (const_cast<Map *>(this)->_blinkTimer.isTriggered()) {
      const_cast<Map *>(this)->_isVisible = !_isVisible;
      const_cast<Map *>(this)->_blinkTimer.start();
    }
    if (_isVisible) {
      _mapTexture.draw();
    }
  } else {
    _mapTexture.draw();
    drawFood();
  }
}

void Map::startBlinking() {
  _blinkTimer.start();
  _isBlinking = true;
}

void Map::stopBlinking() {
  _blinkTimer.deactivate();
  _isBlinking = false;
}

typedef struct {
  cpoint_t position;
  int priority;
} queue_element_t;

class queue_cmp {
public:
  bool operator()(const queue_element_t &left, const queue_element_t &right) {
    return left.priority > right.priority;
  }
};

struct map_cmp {
  bool operator()(const cpoint_t &left, const cpoint_t &right) const {
    return left.x == right.x ? left.y < right.y : left.x < right.x;
  }
};

#ifndef NDEBUG

std::ostream &operator<<(std::ostream &stream, queue_element_t element) {
  stream << "{" << element.position.x << ", " << element.position.y << "}";
  return stream;
}

void printPriorities(std::map<cpoint_t, int, map_cmp> &costSoFar) {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    for (int j = 0; j < BOARD_WIDTH; j++) {
      if (costSoFar.find((cpoint_t){j, i}) != costSoFar.end()) {
        std::cout << costSoFar[(cpoint_t){j, i}];
      } else {
        std::cout << startMapSchematic[i][j];
      }
    }
    std::cout << "\n";
  }
}

#endif

std::vector<cpoint_t> Map::buildPathWithDirection(const Direction &direction,
                                                  const cpoint_t &start,
                                                  const cpoint_t &end) const {

  assert((end.y >= 0) && (end.y < BOARD_HEIGHT));
  if (end.x < 0)
    return buildPathWithDirection(direction, start, mkcpoint(BOARD_WIDTH - 1, end.y));
  if (end.x > (BOARD_WIDTH - 1))
    return buildPathWithDirection(direction, start, mkcpoint(0, end.y));

  mapSchematic[DOOR_POSITION_L.y][DOOR_POSITION_L.x] = ' ';
  mapSchematic[DOOR_POSITION_R.y][DOOR_POSITION_R.x] = ' ';
  std::priority_queue<queue_element_t, std::vector<queue_element_t>, queue_cmp> frontier;
  frontier.push((queue_element_t){start, 0});
  assert(mapSchematic[start.y][start.x] != '#');
  assert(mapSchematic[end.y][end.x] != '#');
  cpoint_t back = start;
  switch (direction) {
  case Right:
    back.x--;
    break;
  case Left:
    back.x++;
    break;
  case Up:
    back.y++;
    break;
  case Down:
    back.y--;
    break;
  default:
    break;
  }
  std::map<cpoint_t, cpoint_t, map_cmp> cameFrom;
  std::map<cpoint_t, int, map_cmp> costSoFar;
  costSoFar[start] = 0;
  while (!frontier.empty()) {
    cpoint_t current = frontier.top().position;
    frontier.pop();
    if (current == end) {
      break;
    }
    for (int i = 0; i < 4; i++) {
      cpoint_t next = current;
      switch ((Direction)(i)) {
      case Right:
        if ((next.x < (BOARD_WIDTH - 1)) && (mapSchematic[next.y][next.x + 1] != '#'))
          next.x += 1;
        break;
      case Left:
        if ((next.x > 0) && (mapSchematic[next.y][next.x - 1] != '#'))
          next.x -= 1;
        break;
      case Up:
        if ((next.y > 0) && (mapSchematic[next.y - 1][next.x] != '#'))
          next.y -= 1;
        break;
      case Down:
        if ((next.y < (BOARD_HEIGHT - 1)) && (mapSchematic[next.y + 1][next.x] != '#'))
          next.y += 1;
        break;
      default:
        break;
      }
      if (current == next) {
        continue;
      }
      if (current == start && next == back && direction != None) {
        continue;
      }
      int newCost = costSoFar[current] + 1;
      if ((costSoFar.find(next) == costSoFar.end()) || (newCost < costSoFar[next])) {
        costSoFar[next] = newCost;
        int priority = newCost + abs(next.x - end.x) + abs(next.y - end.y);
        frontier.push((queue_element_t){next, priority});
        cameFrom[next] = current;
      }
    }
  }
  mapSchematic[DOOR_POSITION_L.y][DOOR_POSITION_L.x] = '#';
  mapSchematic[DOOR_POSITION_R.y][DOOR_POSITION_R.x] = '#';
  std::vector<cpoint_t> path;
  for (cpoint_t i = end; i != start; i = cameFrom[i]) {
    path.push_back(i);
  }
  path.push_back(start);
  std::reverse(path.begin(), path.end());
  return path;
}

std::vector<cpoint_t> Map::buildPath(const cpoint_t &start, const cpoint_t &end) const {
  return buildPathWithDirection(None, start, end);
}

const point_t Map::getTileCenter(const point_t &tile) {
  point_t tileCenter = {static_cast<int>(tile.x * BLOCK_SIZE + BLOCK_SIZE / 2.0f),
                        static_cast<int>(tile.y * BLOCK_SIZE + BLOCK_SIZE / 2.0f)};
  return tileCenter;
}

bool Map::isInTileCenter(const point_t &real) const {
  return (((real.x % BLOCK_SIZE) == static_cast<float>(BLOCK_SIZE) / 2) &&
          ((real.y % BLOCK_SIZE) == static_cast<float>(BLOCK_SIZE) / 2));
}

bool Map::isOnTileEdge(const point_t &real) const {
  return (((real.x % BLOCK_SIZE) == 0) &&
          ((real.y % BLOCK_SIZE) == static_cast<float>(BLOCK_SIZE) / 2));
}

bool Map::isExactLocation(const point_t &real, const cpoint_t &cell) const {
  return (real.x == cell.x * BLOCK_SIZE + BLOCK_SIZE / 2) &&
         (real.y == cell.y * BLOCK_SIZE + BLOCK_SIZE / 2);
}

bool Map::isInTeleport(const point_t &center) const {
  return (center.y == 14.5 * BLOCK_SIZE) &&
         ((center.x < BLOCK_SIZE * 5.5) || center.x > (BOARD_WIDTH - 5.5f) * BLOCK_SIZE);
}

bool Map::isColliding(const point_t &a, const point_t &b) {
  return ((abs(a.x - b.x) < BLOCK_SIZE) && (abs(a.y - b.y) < BLOCK_SIZE));
  // return toCellCoordinates(a) == toCellCoordinates(b);
}

void Map::restart() {
  for (int i = 0; i < BOARD_HEIGHT; i++) {
    strcpy(mapSchematic[i], startMapSchematic[i]);
  }
  _dotsLeft = START_DOTS_COUNT;
}

bool Map::areAnyDotsLeft() const { return _dotsLeft > 0; }
