#include <SFML/Graphics.hpp>

class Triangle2D : public sf::Shape 
{
public:
    Triangle2D (sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
    {
        vertices[0] = p1;
        vertices[1] = p2;
        vertices[2] = p3;
        update();
    }

    virtual std::size_t getPointCount() const
    {
        return 3;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const
    {
        return vertices[index];
    }
private:
    sf::Vector2f vertices[3];

} ;