#include "FontCache.h"

namespace fontcache {

namespace 
{
  sf::Font g_Font;
}

bool init()
{
  return g_Font.loadFromFile(RESOURCE_PATH + std::string("fonts/verdana.ttf"));
}

sf::Font& font()
{
  return g_Font;
}

}