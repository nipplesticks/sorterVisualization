#ifndef _VALUE_H_
#define _VALUE_H_

#include <SFML/Graphics.hpp>
#include "../Utility/Timer.h"

class Value
{
public:
  static double SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION;
  static double TOTAL_SLEEP_DURATION_FOR_ALL_VALUES;

  Value();
  Value(int idx, int val);
  Value(const Value& other);
  ~Value() {};

  void SetIdx(int idx) { m_val.x = idx;  }
  void SetVal(int val) { m_val.y = val;  }
  int  GetIdx() const  { return m_val.x; }
  int  GetVal() const  { return m_val.y; }

  void SetColor(const sf::Color& col) { m_col = col; }

  bool operator==(const Value& other);
  bool operator!=(const Value& other);
  Value& operator=(const Value& other);
  bool operator<(const Value& other);
  bool operator>(const Value& other);
  bool operator<=(const Value& other);
  bool operator>=(const Value& other);

  void Draw(sf::RenderWindow* wnd);

private:
    double _sleep(double duration_ms);

private:
  sf::Vector2i m_val;
  sf::Color m_col;
};

#endif