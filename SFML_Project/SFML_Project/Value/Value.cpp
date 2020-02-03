#include "Value.h"

double Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION = 0.0;
double Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES = 0.0;

Value::Value()
{
    m_val = sf::Vector2i(0, 0);
    m_col = sf::Color::White;
}

Value::Value(int idx, int val)
{
    m_val = sf::Vector2i(idx, val);
}

Value::Value(const Value& other)
{
    m_val = other.m_val;
    m_col = other.m_col;
}

bool Value::operator==(const Value& other)
{
    return m_val.y == other.m_val.y;
}

bool Value::operator!=(const Value& other)
{
    return m_val.y != other.m_val.y;
}

Value& Value::operator=(const Value& other)
{
    if (this != &other)
    {
        m_val.y = other.m_val.y;
        m_col = other.m_col;
    }

    Value::TOTAL_SLEEP_DURATION_FOR_ALL_VALUES += 
        _sleep(Value::SLEEP_DURATION_IN_MS_FOR_EACH_ASSIGNMENT_OPERATION);

    return *this;
}

bool Value::operator<(const Value& other)
{
    return m_val.y < other.m_val.y;
}

bool Value::operator>(const Value& other)
{
    return m_val.y > other.m_val.y;
}

bool Value::operator<=(const Value& other)
{
    return m_val.y <= other.m_val.y;
}

bool Value::operator>=(const Value& other)
{
    return m_val.y >= other.m_val.y;
}

void Value::Draw(sf::RenderWindow* wnd)
{
    sf::Vector2u winSize = wnd->getSize();
    sf::Vertex ver[2];
    ver[0].position.x = (float)m_val.x;
    ver[0].position.y = (float)winSize.y;
    ver[0].color      = m_col;

    ver[1].position.x = (float)m_val.x;
    ver[1].position.y = (float)winSize.y - (float)m_val.y;
    ver[1].color      = m_col;

    wnd->draw(ver, 2, sf::Lines);
}

double Value::_sleep(double duration_ms)
{
    double time = 0.0;
    Timer t;
    t.Start();

    while (time < duration_ms)
        time += t.Stop(Timer::MILLISECONDS);

    return time;
}
