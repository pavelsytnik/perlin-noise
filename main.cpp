#include <cmath>
#include <SFML/Graphics.hpp>

sf::Vector2f randomGradient(int ix, int iy)
{
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; 
    unsigned a = ix, b = iy;
    a *= 3284157443;
 
    b ^= a << s | a >> w - s;
    b *= 1911520717;
 
    a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    
    sf::Vector2f v;
    v.x = std::sin(random);
    v.y = std::cos(random);
 
    return v;
}

float dotGridGradient(int ix, int iy, float x, float y)
{
    sf::Vector2f gradient = randomGradient(ix, iy);

    float dx = x - ix;
    float dy = y - iy;

    return dx * gradient.x + dy * gradient.y;
}

float interpolate(float a0, float a1, float w)
{
    return (a1 - a0) * (3.f - w * 2.f) * w * w + a0;
}

float perlin(float x, float y)
{
    int x0 = std::floor(x);
    int y0 = std::floor(y);
    int x1 = std::ceil(x);
    int y1 = std::ceil(y);

    float sx = x - x0;
    float sy = y - y0;

    float n0 = dotGridGradient(x0, y0, x, y);
    float n1 = dotGridGradient(x1, y0, x, y);
    float ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    float ix1 = interpolate(n0, n1, sx);

    float value = interpolate(ix0, ix1, sy);

    return value;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000, 32), "Noise");

    sf::Vector2u winSize = window.getSize();
    sf::Uint8* pixels = new sf::Uint8[winSize.x * winSize.y * 4];

    const int GRID_SIZE = 400;

    for (int x = 0; x < winSize.x; ++x)
    {
        for (int y = 0; y < winSize.y; ++y)
        {
            int index = (y * winSize.x + x) * 4;

            float val = 0;
            float amp = 1;
            float freq = 1;

            for (int i = 0; i < 12; ++i)
            {
                val += perlin(x * freq / GRID_SIZE, y * freq / GRID_SIZE) * amp;

                freq *= 2;
                amp /= 2;
            }
            val *= 1.2f;

            if (val > 1.f) val = 1.f;
            else if (val < -1.f) val = -1.f;

            int color = (int) (((val + 1.f) * .5f) * 255.f);

            pixels[index + 2] = pixels[index + 1] = pixels[index] = color;
            pixels[index + 3] = 255;
        }
    }

    sf::Texture noiseTexture;
    sf::Sprite noiseSprite;

    noiseTexture.create(winSize.x, winSize.y);
    noiseTexture.update(pixels);

    noiseSprite.setTexture(noiseTexture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            }
        }
        window.clear();
        window.draw(noiseSprite);
        window.display();
    }

    return 0;
}