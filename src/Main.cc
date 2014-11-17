
/*******************************************************************************
 * Class Name: N/A - int main implementation
 * Date Created: November 17, 2013
 * Inheritance: none
 * Description: The main call implementation. Essentially just sets up
 *              necessary subsystems and starts up the application.
 ******************************************************************************/
#include "Application.h"
#include "Helpers.h"
#include "Game/Map/TileSprite.h"

#include <unistd.h>

bool initSDL()
{
  bool success = true;
  
  /* Initialize SDL */
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
  {
    std::cerr << "[ERROR] SDL could not initialize. SDL error: "
              << SDL_GetError() << std::endl;
    success = false;
  }
  
  /* Initialize SDL Image library */
  if(!IMG_Init(IMG_INIT_PNG))
  {
    std::cerr << "[ERROR] SDL_Image could not initialize. SDL_image error: "
              << IMG_GetError() << std::endl;
    success = false;
  }
    
  /* Initialize SDL TTF library */
  if(!TTF_WasInit() && TTF_Init() == -1)
  {
    std::cerr << "[ERROR] SDL_ttf could not initialize. SDL_ttf error: "
              << TTF_GetError() << std::endl;
    success = false;
  }

  /* Init SDL mixer, and open the audio with the chosen settings */
  int buffers = 1024; /* Size of audio buffers in memory */
  int channels = 2;   /* 1 channel = mono, 2 = stereo */
  int rate = 22050;   /* Frequency of Audio Playback */
  if(!Mix_Init(MIX_INIT_OGG) || 
     Mix_OpenAudio(rate, AUDIO_S16SYS, channels, buffers) != 0)
  {
    std::cerr << "[WARNING] Unable to initialize audio: " 
              << Mix_GetError() << std::endl;
  }

  return success;
}

int main(int argc, char** argv)
{
  (void)argc;
  (void)argv;

  // TESTING: TODO - Remove
  //while(1) {
  std::cout << "--THING TEST--" << std::endl;
  Tile* t = new Tile();
  t->setStatus(Tile::ACTIVE);
  MapThing* thing = new MapThing();
  std::cout << "PTR: " << thing << std::endl;
  std::cout << "Is Set: " << t->isThingsSet() << " " 
                          << t->isThingSet(2) << " "
                          << t->isThingSet(3) << std::endl;
  std::cout << "Status: " << t->setThing(thing, 2) << std::endl;
  std::cout << "Is Set: " << t->isThingsSet() << " " 
                          << t->isThingSet(2) << " "
                          << t->isThingSet(3) << std::endl;
  std::cout << "PTR: " << t->getThing(2) << std::endl;
  std::vector<MapThing*> thing_set = t->getThings();
  for(uint16_t i = 0; i < thing_set.size(); i++)
    std::cout << "PTR-SET: " << thing_set[i] << std::endl;
  thing_set.clear();

  MapItem* r_item = NULL;
  MapPerson* r_person = NULL;
  MapThing* r_thing = NULL;
  std::cout << "R Status: " << t->getRenderThings(2, r_item, r_person, r_thing)
            << std::endl;
  std::cout << "RENDER: " << r_thing << " " << r_person << std::endl;

  std::cout << "U Status: " << t->unsetThing(1) << std::endl;
  t->unsetThings();
  std::cout << "PTR: " << t->getThing(2) << std::endl;

  std::cout << "PTR: " << thing << std::endl;
  std::cout << "Is Set: " << t->isThingsSet() << " " 
                          << t->isThingSet(0) << " "
                          << t->isThingSet(3) << std::endl;
  std::cout << "Status: " << t->setThing(thing, 0) << std::endl;
  std::cout << "Is Set: " << t->isThingsSet() << " " 
                          << t->isThingSet(0) << " "
                          << t->isThingSet(3) << std::endl;
  std::cout << "PTR: " << t->getThing(0) << std::endl;
  thing_set = t->getThings();
  for(uint16_t i = 0; i < thing_set.size(); i++)
    std::cout << "PTR-SET: " << thing_set[i] << std::endl;
  thing_set.clear();

  delete t;
  delete thing;
  std::cout << "--END THING TEST--" << std::endl;

  std::cout << "--PERSON TEST--" << std::endl;
  Tile* t2 = new Tile();
  t2->setStatus(Tile::ACTIVE);
  MapPerson* person = new MapPerson();
  std::cout << "PTR: " << person << std::endl;
  std::cout << "Is Set: " << t2->isPersonsSet() << " " 
                          << t2->isPersonSet(9) << " "
                          << t2->isPersonSet(3) << std::endl;
  std::cout << "Status: " << t2->setPerson(person, 9, false) << std::endl;
  std::cout << "Is Set: " << t2->isPersonsSet() << " " 
                          << t2->isPersonSet(9) << " "
                          << t2->isPersonSet(3) << std::endl;
  std::cout << "PTR: " << t2->getPerson(9) << std::endl;
  std::vector<MapPerson*> person_set = t2->getPersons();
  for(uint16_t i = 0; i < person_set.size(); i++)
    std::cout << "PTR-SET: " << person_set[i] << std::endl;
  person_set.clear();

  r_item = NULL;
  r_person = NULL;
  r_thing = NULL;
  std::cout << "R Status: " << t2->getRenderThings(2, r_item, r_person, r_thing)
            << std::endl;
  std::cout << "RENDER: " << r_thing << " " << r_person << std::endl;

  std::cout << "U Status: " << t2->unsetPerson(1, true) << std::endl;
  t2->unsetPersons();
  std::cout << "PTR: " << t2->getPerson(2) << std::endl;

  std::cout << "PTR: " << person << std::endl;
  std::cout << "Is Set: " << t2->isPersonsSet() << " " 
                          << t2->isPersonSet(2) << " "
                          << t2->isPersonSet(3) << std::endl;
  std::cout << "Status: " << t2->setPerson(person, 0, false) << std::endl;
  std::cout << "Is Set: " << t2->isPersonsSet() << " " 
                          << t2->isPersonSet(0) << " "
                          << t2->isPersonSet(3) << std::endl;
  std::cout << "PTR: " << t2->getPerson(0) << std::endl;
  person_set = t2->getPersons();
  for(uint16_t i = 0; i < person_set.size(); i++)
    std::cout << "PTR-SET: " << person_set[i] << std::endl;
  person_set.clear();

  delete t2;
  delete person;
  std::cout << "--END PERSON TEST--" << std::endl;

  std::cout << "--ITEM TEST--" << std::endl;
  Tile* t3 = new Tile();
  t3->setStatus(Tile::ACTIVE);
  MapItem* item = new MapItem();
  MapItem* item3 = new MapItem();
  std::cout << "PTR: " << item << std::endl;
  std::cout << "Is Set: " << t3->isItemsSet() << std::endl; 
  std::cout << "Status: " << t3->addItem(item) << std::endl;
  std::cout << "Is Set: " << t3->isItemsSet() << std::endl;
  std::cout << "Status: " << t3->addItem(item3) << std::endl;
  std::cout << "Check: " << t3->getItemCount() << std::endl;
  std::vector<MapItem*> item_set = t3->getItems();
  for(uint16_t i = 0; i < item_set.size(); i++)
    std::cout << "PTR-SET: " << item_set[i] << std::endl;
  item_set.clear();

  r_item = NULL;
  r_person = NULL;
  r_thing = NULL;
  std::cout << "R Status: " << t3->getRenderThings(0, r_item, r_person, r_thing)
            << std::endl;
  std::cout << "RENDER: " << r_thing << " " << r_person << " " 
                          << r_item << std::endl;

  MapItem* item2 = new MapItem();
  std::cout << "U Status: " << t3->unsetItem(item3) << std::endl;
  //t3->unsetItems();
  std::cout << "Check: " << t3->getItemCount() << std::endl;
  item_set = t3->getItems();
  for(uint16_t i = 0; i < item_set.size(); i++)
    std::cout << "PTR-SET: " << item_set[i] << std::endl;
  item_set.clear();
  
  std::cout << "PTR: " << item << std::endl;
  std::cout << "Is Set: " << t3->isItemsSet() << std::endl; 
  std::cout << "Status: " << t3->addItem(item) << std::endl;
  std::cout << "Is Set: " << t3->isItemsSet() << std::endl;
  std::cout << "Check: " << t3->getItemCount() << std::endl;
  t3->unsetItems();
  item_set = t3->getItems();
  for(uint16_t i = 0; i < item_set.size(); i++)
    std::cout << "PTR-SET: " << item_set[i] << std::endl;
  item_set.clear();

  delete t3;
  delete item;
  delete item2;
  delete item3;
  std::cout << "--END ITEM TEST--" << std::endl;
  //}

  /* Get the base directory to the executable, which will be the location of
   * all applicable resources */
  char* directory = SDL_GetBasePath();
  std::string dir_string(directory);
  SDL_free(directory);
  
  /* Initialize SDL libraries */
  bool success = initSDL();
  
  if(success)
  {
    /* Create the application and start the run loop */
    Application* game_app = new Application(dir_string);
    if(game_app->initialize())
      game_app->run();
  
    /* Clean up the application, after the run loop is finished */
    game_app->uninitialize();
    delete game_app;
    game_app = NULL;

  }
  
  /* Clean up SDL libraries */
  Mix_CloseAudio();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  
	return 0;
}
