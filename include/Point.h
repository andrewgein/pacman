#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
#include "Global.hpp"

typedef struct cpoint_t {
  int x;
  int y;
} cpoint_t;

inline bool operator==(const cpoint_t &a, const cpoint_t &b) {
  return (a.x == b.x) && (a.y == b.y);
}

inline bool operator!=(const cpoint_t &a, const cpoint_t &b) {
  return (a.x != b.x) || (a.y != b.y);
}

inline bool operator<(const cpoint_t &a, const cpoint_t &b) {
  return a.x == b.x ? a.y < b.y : a.x < b.x;
}

inline std::ostream &operator<<(std::ostream &stream, const cpoint_t &point) {
  stream << "{ " << point.x << ", " << point.y << " }";
  return stream;
}

inline cpoint_t mkcpoint(const int &x, const int &y) {
  return (cpoint_t){x, y};
}


typedef struct point_t {
  int x;
  int y;
  operator cpoint_t() const {
    cpoint_t cellPoint = {x / BLOCK_SIZE,
                          y / BLOCK_SIZE};
    return cellPoint;
  }
  point_t operator+(const point_t &p) {
    return (point_t){x + p.x, y + p.y};
  }
} point_t;

inline bool operator==(const point_t &a, const point_t &b) {
  return (a.x == b.x) && (a.y == b.y);
}

inline bool operator!=(const point_t &a, const point_t &b) {
  return (a.x != b.x) || (a.y != b.y);
}

inline bool operator<(const point_t &a, const point_t &b) {
  return a.x == b.x ? a.y < b.y : a.x < b.x;
}

inline std::ostream &operator<<(std::ostream &stream, const point_t &point) {
  stream << "{ " << point.x << ", " << point.y << " }";
  return stream;
}

inline point_t mkpoint(const int &x, const int &y) {
  return (point_t){x, y};
}


#endif
