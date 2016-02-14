#include "Game/Lay.h"

const float Lay::kABS_MAX_VELOCITY_X{700};
const float Lay::kABS_MAX_VELOCITY_Y{300};

Lay::Lay()
    : animation_time{0},
      config{nullptr},
      path{""},
      renderer{nullptr},
      lay_type{LayType::NONE}
{
}

Lay::Lay(std::string path, uint32_t animation_time, Floatinate velocity,
         LayType lay_type, SDL_Renderer* renderer, Options* config)
    : Lay()
{
  this->path = path;
  this->animation_time = animation_time;
  this->lay_type = lay_type;
  this->velocity = velocity;

  setConfig(config);
  setRenderer(renderer);
  setVelocity(velocity);

  if(this->lay_type != LayType::NONE)
  {
    auto success = createTiledLays();

#ifdef UDEBUG
    if(!success)
    {
      std::cout << "[ERROR] Creating tile overlays with path: " << path
                << std::endl;
    }
#endif
  }
}

Lay::~Lay()
{
  for(auto& lay_tile : lay_tiles)
  {
    if(lay_tile && lay_tile->lay_sprite)
    {
      delete lay_tile->lay_sprite;
      lay_tile->lay_sprite = nullptr;
    }

    delete lay_tile;
  }
}

void Lay::setConfig(Options* config)
{
  this->config = config;
}

void Lay::setRenderer(SDL_Renderer* renderer)
{
  this->renderer = renderer;
}

void Lay::setVelocity(Floatinate new_velocity)
{
  velocity.x = Helpers::setInRange(new_velocity.x, -kABS_MAX_VELOCITY_X,
                                   kABS_MAX_VELOCITY_X);
  velocity.y = Helpers::setInRange(new_velocity.y, -kABS_MAX_VELOCITY_Y,
                                   kABS_MAX_VELOCITY_Y);
}

bool Lay::render()
{
  bool success{true};

  for(auto lay_tile : lay_tiles)
  {
    if(lay_tile && lay_tile->lay_sprite && renderer && config)
    {
      success &= lay_tile->lay_sprite->render(
          renderer, lay_tile->location.x, lay_tile->location.y,
          config->getScreenWidth(), config->getScreenHeight());
    }
  }

  return success;
}

void Lay::update(int32_t cycle_time)
{
  assert(config);

  error.x += cycle_time * velocity.x;
  error.y += cycle_time * velocity.y;

  auto dist_x = std::floor(error.x);
  auto dist_y = std::floor(error.y);

  error.x -= dist_x;
  error.y -= dist_y;

  // std::cout << "Error: " << error.x << ", " << error.y << std::endl;
  // std::cout << "Distance: " << dist_x << ", " << dist_y << std::endl;

  for(auto& lay_tile : lay_tiles)
  {
    if(lay_tile)
    {
      if(velocity.x > 0)
      {
        if((lay_tile->location.x + dist_x) > config->getScreenWidth())
          lay_tile->location.x -= config->getScreenWidth() * 2;

        lay_tile->location.x += dist_x;
      }

      if(velocity.y > 0)
      {
        if((lay_tile->location.y + dist_y) > (config->getScreenHeight()))
          lay_tile->location.y -= config->getScreenHeight() * 2;

        lay_tile->location.y += dist_y;
      }

      if(velocity.x < 0)
      {
        if((lay_tile->location.x + dist_x) < -config->getScreenWidth())
          lay_tile->location.x += config->getScreenWidth() * 2;

        lay_tile->location.x += dist_x;
      }

      if(velocity.y < 0)
      {
        if((lay_tile->location.y + dist_y) < -config->getScreenHeight())
          lay_tile->location.y += config->getScreenHeight() * 2;

        lay_tile->location.y += dist_y;
      }

      if(lay_tile->lay_sprite)
        lay_tile->lay_sprite->update(animation_time);
    }
  }
}

bool Lay::createTiledLays()
{
  bool success{true};

  success &= createLay(LayIndex::CENTRE);

  if(velocity.x < 0)
    success &= createLay(LayIndex::EAST);
  if(velocity.x > 0)
    success &= createLay(LayIndex::WEST);
  if(velocity.y < 0)
    success &= createLay(LayIndex::SOUTH);
  if(velocity.y > 0)
    success &= createLay(LayIndex::NORTH);

  if(velocity.x > 0 && velocity.y > 0)
    success &= createLay(LayIndex::NORTH_WEST);
  else if(velocity.x > 0 && velocity.y < 0)
    success &= createLay(LayIndex::SOUTH_WEST);
  else if(velocity.x < 0 && velocity.y > 0)
    success &= createLay(LayIndex::NORTH_EAST);
  else if(velocity.x < 0 && velocity.y < 0)
    success &= createLay(LayIndex::SOUTH_EAST);

  return success;
}

bool Lay::createLay(LayIndex lay_index)
{
  if(renderer && config && path != "")
  {
    auto new_tile = new LayTile();

    auto split = Helpers::split(path, '|');
    auto num_frames = 1;

    if(split.size() == 3)
    {
      num_frames = std::stoi(split.at(1));
      new_tile->lay_sprite =
          new Sprite(split.at(0), num_frames, split.at(2), renderer);
    }
    else if(split.size() == 1)
    {
      new_tile->lay_sprite = new Sprite(path, renderer);
    }

    if((split.size() == 3 || split.size() == 1) && new_tile->lay_sprite)
    {
      new_tile->lay_sprite->setAnimationTime(animation_time);
      new_tile->lay_sprite->setNonUnique(true, num_frames);
      new_tile->lay_sprite->createTexture(renderer);

      auto x = config->getScreenWidth();
      auto y = config->getScreenHeight();

      if(lay_index == LayIndex::NORTH_WEST)
        new_tile->location = {-x, -y};
      else if(lay_index == LayIndex::NORTH)
        new_tile->location = {0, -y};
      else if(lay_index == LayIndex::NORTH_EAST)
        new_tile->location = {x, -y};
      else if(lay_index == LayIndex::WEST)
        new_tile->location = {-x, 0};
      else if(lay_index == LayIndex::CENTRE)
        new_tile->location = {0, 0};
      else if(lay_index == LayIndex::EAST)
        new_tile->location = {x, 0};
      else if(lay_index == LayIndex::SOUTH_WEST)
        new_tile->location = {-x, y};
      else if(lay_index == LayIndex::SOUTH)
        new_tile->location = {0, y};
      else if(lay_index == LayIndex::SOUTH_EAST)
        new_tile->location = {x, y};

      lay_tiles.push_back(new_tile);

      return true;
    }
  }

  return false;
}
