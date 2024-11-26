#include "GhostController.hpp"

GhostController::GhostController(GhostData &data, const Map &map,
                                 const PlayerController &player)
    : _map(map), _data(data), _player(player), _updateTimer(GHOST_UPDATE_TICKS) {
  _collisionFlags = CanUseTeleport;
  _isAlongEdgeOfTile = false;
  _updateTimer.start();
  _data.isVisible = true;
  phase_data phases[7] = {
      {GhostData::Scatter, Timer(116)},   {GhostData::Chase, Timer(33000)},
      {GhostData::Scatter, Timer(11600)}, {GhostData::Chase, Timer(33000)},
      {GhostData::Scatter, Timer(8300)},  {GhostData::Chase, Timer(33000)},
      {GhostData::Scatter, Timer(8300)},
  };
  _phaseQueue.assign(std::begin(phases), std::end(phases));
}

GhostController::~GhostController() {}

void GhostController::setState(GhostData::State state) {
  // If state was Scatter or Chase ghost reverses direction
  if ((state != GhostData::Dead) &&
      ((_data.state == GhostData::Scatter) || (_data.state == GhostData::Chase))) {
    clearPath();
    reverseDirection();
  }
  switch (state) {
  case GhostData::Dead:
    _collisionFlags = CanUseDoor + CanUseTeleport;
    _data.state = state;
    goHome();
    break;
  case GhostData::Scatter:
  case GhostData::Chase:
    _collisionFlags = CanUseTeleport;
    assert(!_map.wallCollision(_data.position, _collisionFlags));
    if (state == GhostData::Chase) {
      adjustPath(_player);
    } else {
      updateTargetCorner();
    }
    break;
  case GhostData::Frightened:
    if (_data.state == state) {
      return;
    }
    if (!_phaseQueue.empty() && _phaseQueue.front().timer.isActive()) {
#ifndef NDEBUG
      std::cout << "[log] pause top state timer\n";
#endif
      _phaseQueue.front().timer.pause();
    }
    if (_data.state == GhostData::ComingOut) {
      _data.state = GhostData::FrightenedAndComingOut;
      return;
    } else if (_data.state == GhostData::Waiting) {
      _data.state = GhostData::FrightenedAndWaiting;
      return;
    }
    randomizePath();
    break;
  case GhostData::ComingOut:
  case GhostData::FrightenedAndComingOut:
    _collisionFlags = CanUseDoor + CanUseTeleport;
    goToExit();
    break;
  case GhostData::Waiting:
  case GhostData::FrightenedAndWaiting:
    break;
  }
  _data.state = state;
}

void GhostController::continueState() {
  if (_data.state == GhostData::FrightenedAndComingOut) {
    _data.state = GhostData::ComingOut;
    return;
  }
  if (_data.state == GhostData::FrightenedAndWaiting) {
    _data.state = GhostData::Waiting;
    return;
  }
  if (!_phaseQueue.empty()) {
#ifndef NDEBUG
    std::cout << "[log] unpause top state timer\n";
#endif
    _phaseQueue.front().timer.start();
    setState(_phaseQueue.front().state);
  } else {
    setState(GhostData::Chase);
  }
}

GhostData::State GhostController::getState() { return _data.state; }

void GhostController::setDirection(Direction direction) { _data.direction = direction; }

Direction GhostController::getDirection() const { return _data.direction; }

point_t GhostController::getPosition() const { return _data.position; }

point_t GhostController::getNextPosition() const {
  return getNextPosition(_data.direction);
}

point_t GhostController::getNextPosition(Direction direction) const {
  point_t nextPosition = _data.position;
  assert(direction != None);
  switch (direction) {
  case Up:
    nextPosition.y -= 2;
    break;
  case Down:
    nextPosition.y += 2;
    break;
  case Left:
    nextPosition.x -= 2;
    break;
  case Right:
    nextPosition.x += 2;
    break;
  default:
    break;
  }
  return nextPosition;
}

void GhostController::update() {
  if (!_updateTimer.isTriggered()) {
    return;
  }
  _updateTimer.start();
  if ((!_phaseQueue.empty()) &&
      ((_data.state == GhostData::Scatter) || (_data.state == GhostData::Chase))) {
    assert(_data.state == _phaseQueue.front().state);
    if (!_phaseQueue.front().timer.isActive())
      _phaseQueue.front().timer.start();

    if (_phaseQueue.front().timer.isTriggered()) {
#ifndef NDEBUG
      std::cout << "[log] update state\n";
#endif
      _phaseQueue.pop_front();
      if (!_phaseQueue.empty()) {
        _phaseQueue.front().timer.start();
        setState(_phaseQueue.front().state);
      } else {
        setState(GhostData::Chase);
      }
    }
  }
  if (((cpoint_t)_data.position).y == MAP_TELEPORT_Y) {
    if (_data.position.x + BLOCK_SIZE / 2 <= 0) {
      _data.position.x += MAP_WIDTH + BLOCK_SIZE / 2;
    } else if (_data.position.x >= MAP_WIDTH + BLOCK_SIZE / 2) {
      _data.position.x -= (MAP_WIDTH + BLOCK_SIZE / 2);
    }
  }

  // TODO build a path directly in the teleport, instead of waiting to exit it
  if (_map.isInTeleport(_data.position)) {
    move();
    return;
  }

  switch (_data.state) {
  case GhostData::Dead:
    for (int i = 0; i < 4; i++) {
      if (isOnPorch()) {
        _isAlongEdgeOfTile = true;
        if (_data.direction == Right) {
          onAfterMove();
        }
      }
      updateDirection();
      move();
    }
    if (getTargetPoint() == NULL) {
      setState(GhostData::ComingOut);
    }
    return;
  case GhostData::Scatter:
    if (getTargetPoint() == NULL)
      updateTargetCorner();
    updateDirection();
    break;
  case GhostData::Frightened:
    if ((getTargetPoint() == NULL) && (_map.isInTileCenter(_data.position)))
      randomizePath();
    updateDirection();
    break;
  case GhostData::Chase:
    if (_map.isInTileCenter(_data.position))
      adjustPath(_player);
    updateDirection();
    break;
  case GhostData::FrightenedAndComingOut:
  case GhostData::ComingOut:
    if (getTargetPoint() == NULL) {
      if (_data.state == GhostData::FrightenedAndComingOut) {
        setState(GhostData::Frightened);
      } else {
        continueState();
      }
      updateDirection();
      _isAlongEdgeOfTile = false;
      return;
    }
    updateDirection();
    break;
  case GhostData::FrightenedAndWaiting:
  case GhostData::Waiting:
    assert(getReleaseTimer() != NULL);
    if (!getReleaseTimer()->isActive()) {
      getReleaseTimer()->start();
    }
    if (getReleaseTimer()->isTriggered() && _data.position.y == getHomePosition().y) {
      if (_data.state == GhostData::FrightenedAndWaiting) {
        setState(GhostData::FrightenedAndComingOut);
      } else {
        setState(GhostData::ComingOut);
      }
      return;
    }
    if (getTargetPoint() == NULL) {
      reverseDirection();
      std::vector<cpoint_t> path;
      point_t endPoint;
      if (_data.position.y == getHomePosition().y - BLOCK_SIZE) {
        endPoint = mkpoint(_data.position.x, getHomePosition().y + BLOCK_SIZE);
      } else if (_data.position.y == getHomePosition().y + BLOCK_SIZE) {
        endPoint = mkpoint(_data.position.x, getHomePosition().y - BLOCK_SIZE);
      }
      buildPathTo(endPoint);
    }
    updateDirection();
    break;
  }
  move();
}

void GhostController::updateDirection() {
  if (getTargetPoint() == NULL || !canChangeDirection()) {
    return;
  }
  assert(abs(((cpoint_t)_data.position).y - getTargetPoint()->y) <= 1);
  assert(abs(((cpoint_t)_data.position).x - getTargetPoint()->x) <= 1);
  Direction nextDirection = None;
  cpoint_t cellCoordinates = (cpoint_t)_data.position;
  if (getTargetPoint()->x != cellCoordinates.x) {
    if ((getTargetPoint()->x < cellCoordinates.x) &&
        (!_map.wallCollision(getNextPosition(Left), _collisionFlags)))
      nextDirection = Left;
    else if (!_map.wallCollision(getNextPosition(Right), _collisionFlags))
      nextDirection = Right;
  } else if (getTargetPoint()->y != cellCoordinates.y) {
    if ((getTargetPoint()->y > cellCoordinates.y) &&
        (!_map.wallCollision(getNextPosition(Down), _collisionFlags)))
      nextDirection = Down;
    else if (!_map.wallCollision(getNextPosition(Up), _collisionFlags))
      nextDirection = Up;
  }
  if (nextDirection != None) {
    assert(!_map.wallCollision(getNextPosition(nextDirection), _collisionFlags));
    setDirection(nextDirection);
  }
}

void GhostController::move() {
  if (_data.direction != None) {
    point_t nextPosition = getNextPosition();
    if (!_map.wallCollision(nextPosition, _collisionFlags)) {
      _data.position = getNextPosition();
    }
  }
  onAfterMove();
}

// TODO rename this
void GhostController::onAfterMove() {
  if (getTargetPoint() == NULL) {
    return;
  }
  if (_isAlongEdgeOfTile && _map.isOnTileEdge(_data.position) &&
      (_data.position == *getTargetPoint())) {
    updateTargetPoint();
  }
  if (_map.isInTileCenter(_data.position) &&
      _map.isExactLocation(_data.position, *getTargetPoint())) {
    updateTargetPoint();
  }
}

void GhostController::reverseDirection() {
  switch (_data.direction) {
  case Left:
    _data.direction = Right;
#ifndef NDEBUG
    std::cout << "[log] left to right\n";
#endif
    break;
  case Right:
    _data.direction = Left;
#ifndef NDEBUG
    std::cout << "[log] right to left\n";
#endif
    break;
  case Up:
    _data.direction = Down;
#ifndef NDEBUG
    std::cout << "[log] down to up\n";
#endif
    break;
  case Down:
    _data.direction = Up;
#ifndef NDEBUG
    std::cout << "[log] up to down\n";
#endif
    break;
  default:
    break;
  }
}

void GhostController::randomizePath() {
  std::array<bool, 4> availableMoves;
  const cpoint_t position = _data.position;
  availableMoves[Right] =
      !_map.wallCollision(mkcpoint(position.x + 1, position.y), _collisionFlags);
  availableMoves[Left] =
      !_map.wallCollision(mkcpoint(position.x - 1, position.y), _collisionFlags);
  availableMoves[Up] =
      !_map.wallCollision(mkcpoint(position.x, position.y - 1), _collisionFlags);
  availableMoves[Down] =
      !_map.wallCollision(mkcpoint(position.x, position.y + 1), _collisionFlags);
  if (_data.direction == Right)
    availableMoves[Left] = false;
  if (_data.direction == Left)
    availableMoves[Right] = false;
  if (_data.direction == Up)
    availableMoves[Down] = false;
  if (_data.direction == Down)
    availableMoves[Up] = false;

  Direction nextDirection;

  // In case the state is changed to Frightened and ghost forced to
  // reverse direction while currently in an unclear position
  int countOfAvailableMoves =
      std::accumulate(availableMoves.begin(), availableMoves.end(), 0);
  if (countOfAvailableMoves == 0) {
    _data.path.push_back(_data.position);
    return;
  }

  if (countOfAvailableMoves > 1) {
    while (availableMoves[nextDirection = static_cast<Direction>(std::rand() % 4)] ==
           false)
      ;
  } else if (countOfAvailableMoves == 1) {
    nextDirection = static_cast<Direction>(
        std::distance(availableMoves.begin(),
                      std::find(availableMoves.begin(), availableMoves.end(), true)));
  }
  cpoint_t nextPoint = (cpoint_t)_data.position;
  switch (nextDirection) {
  case Right:
    nextPoint.x += 1;
    break;
  case Left:
    nextPoint.x -= 1;
    break;
  case Down:
    nextPoint.y += 1;
    break;
  case Up:
    nextPoint.y -= 1;
    break;
  default:
    break;
  }
  buildPathTo(nextPoint);
}

void GhostController::goToExit() {
  std::vector<cpoint_t> path1 = _map.buildPath(_data.position, HOME_CENTER_POSITION);
  path1.pop_back();
  std::vector<cpoint_t> path2 = _map.buildPath(HOME_CENTER_POSITION, EXIT_POSITION);
  path1.insert(path1.end(), path2.begin(), path2.end());

  setPath(path1);
}

bool GhostController::isOnPorch() {
  return (_data.position == PORCH_POSITION_L || _data.position == PORCH_POSITION_R);
}

void GhostController::setPath(const std::vector<cpoint_t> &path) {
  _data.path.assign(path.begin(), path.end());
  optimizePath();
}

void GhostController::adjustPath(const PlayerController &playerController) {
  if (rand() % 4) {
    cpoint_t startPoint =
        getTargetPoint() == NULL ? (cpoint_t)_data.position : *getTargetPoint();
    std::vector<cpoint_t> path = _map.buildPathWithDirection(
        _data.direction, startPoint, playerController.getPosition());
    setPath(path);
  } else {
    GhostController::randomizePath();
  }
}

void GhostController::clearPath() { _data.path.clear(); }

cpoint_t *GhostController::getTargetPoint() const {
  if (_data.path.empty()) {
    return NULL;
  }
  return &_data.path.front();
}

void GhostController::updateTargetPoint() {
  if (!_data.path.empty()) {
    _data.path.pop_front();
  }
}

void GhostController::setVisibility(bool isVisible) { _data.isVisible = isVisible; }

bool GhostController::isVisible() { return _data.isVisible; }

bool GhostController::isFrightened() {
  return (_data.state == GhostData::Frightened ||
          _data.state == GhostData::FrightenedAndWaiting ||
          _data.state == GhostData::FrightenedAndComingOut);
}

void GhostController::goHome() {
  cpoint_t startPoint = _data.position;
  if (_map.isInTeleport(_data.position)) {
    if (_data.position.x < BOARD_WIDTH / 2) {
      startPoint = mkpoint(MAP_TELEPORT_X2, MAP_TELEPORT_Y);
    } else {
      startPoint = mkpoint(MAP_TELEPORT_X1, MAP_TELEPORT_Y);
    }
  }
  std::vector<cpoint_t> path1 = _map.buildPath(startPoint, EXIT_POSITION);
  path1.pop_back();
  std::vector<cpoint_t> path2 = _map.buildPath(EXIT_POSITION, HOME_CENTER_POSITION);
  path2.pop_back();
  std::vector<cpoint_t> path3 = _map.buildPath(HOME_CENTER_POSITION, getHomePosition());
  path1.insert(path1.end(), path2.begin(), path2.end());
  path1.insert(path1.end(), path3.begin(), path3.end());
  setPath(path1);
}

bool GhostController::canChangeDirection() {
  if (_isAlongEdgeOfTile) {
    bool result = _map.isInTileCenter(_data.position + (point_t){BLOCK_SIZE / 2, 0});
    return result;
  } else {
    cpoint_t current = _data.position;
    cpoint_t target = *getTargetPoint();
    switch (_data.direction) {
    case Up:
      if (current.y < target.y) {
        return true;
      }
      break;
    case Down:
      if (current.y > target.y) {
        return true;
      }
      break;
    case Left:
      if (current.x < target.x) {
        return true;
      }
      break;
    case Right:
      if (current.x > target.x) {
        return true;
      }
      break;
    default:
      break;
    }
  }
  return _map.isInTileCenter(_data.position);
}

void GhostController::buildPathTo(cpoint_t end) {
  std::vector<cpoint_t> path =
      _map.buildPathWithDirection(_data.direction, _data.position, end);
  setPath(path);
}

void GhostController::optimizePath() {
  if (_data.path.size() <= 1) {
    return;
  }
  if (_data.position == _data.path.front()) {
    if (_isAlongEdgeOfTile && _map.isOnTileEdge(_data.position)) {
      _data.path.pop_front();
    } else if (!_isAlongEdgeOfTile && _map.isInTileCenter(_data.position)) {
      _data.path.pop_front();
    }
  }
  /*cpoint_t current = _data.position;
  cpoint_t target = *getTargetPoint();
  switch (_data.direction) {
  case Up:
    if (current.y < target.y) {
      updateTargetPoint();
    }
    break;
  case Down:
    if (current.y > target.y) {
      updateTargetPoint();
    }
    break;
  case Left:
    if (current.x < target.x) {
      updateTargetPoint();
    }
    break;
  case Right:
    if (current.x > target.x) {
      updateTargetPoint();
    }
    break;
  default:
    break;
  }*/
}
