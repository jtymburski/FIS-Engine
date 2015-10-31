/*******************************************************************************
 * Class Name: TestBattle
 * Date Created: July 31, 2015
 * Inheritance: none
 * Description: This class is the testing interface for battle scenarios.
 ******************************************************************************/
#ifndef TESTBATTLE_H
#define TESTBATTLE_H

#include <SDL2/SDL.h>
#include <chrono>
#include <memory>

#include "Game/Battle/BattleDisplayData.h"
#include "Game/Battle/Battle.h"
#include "Game/Player/Player.h"
#include "Options.h"

class TestBattle
{
public:
  /* Constructor function */
  TestBattle(Options* running_config = nullptr);

  /* Destructor function */
  ~TestBattle();

  /* Enumerator: Test battle selection items */
  enum MenuItems
  {
    AC = 0, /* Arcadius */
    AA = 1, /* Aurora Agent */
    AAx2 = 2, /* Aurora Agent x 2 */
    AAx5 = 3, /* Aurora Agent x 5 */
    AAnAH = 4, /* Aurora Agent and Heavy */
    AH = 5, /* Aurora Heavy */
    AEnAD = 6, /* Aurora Engineer and Drone */
    AEnADx4 = 7, /* Aurora Engineer and Drone x 4 */
    AAnAHnADnAE = 8, /* Aurora Agent, Engg, Heavy, and Drone */
    REnRG = 9, /* Reverdile and Reverdling */
    REnRGx2 = 10, /* Reverdile and Reverdling x 2 */
    REnRGx4 = 11, /* Reverdile and Reverdling x 4 */
    RG = 12, /* Reverdling */
    RGx2 = 13, /* Reverdling x 2 */
    RGx5 = 14
  }; /* Reverdling x 5 */

  /* Enumerator: Test mode for user input */
  enum TestMode
  {
    SCENARIO = 0,
    FRIEND_LVL = 1,
    FOE_LVL = 2,
    TEST_BATTLE = 3,
    NONE = 4
  };

  /* Enumerator: Person selection mode */
  enum TestPerson
  {
    ARCADIUS,
    AURORAAGENT,
    AURORAHEAVY,
    AURORAENGG,
    AURORADRONE,
    REVERDILE,
    REVERDLING,
    PLAYER
  };

private:
  /* Action sets */
  std::vector<Action*> act_alt; /* Alter */
  std::vector<Action*> act_asn; /* Assign */
  std::vector<Action*> act_dmg; /* Damage */
  std::vector<Action*> act_inf; /* Inflict */
  std::vector<Action*> act_rlv; /* Relieve */

  /* The computed base path for resources in the application */
  std::string base_path;

  /* Display Data Pointer */
  BattleDisplayData* display_data;

  /* A current battle pointer */
  Battle* battle_logic;
  bool battle_start;

  /* Classes */
  Category* class_arcadius1;
  Category* class_aurora_agent;
  Category* class_aurora_heavy;
  Category* class_aurora_drone;
  Category* class_aurora_engg;
  Category* class_player;
  Category* class_reverdile;
  Category* class_reverdling;

  Sprite* sprite_paralysis;
  Sprite* sprite_confusion;
  Sprite* sprite_buff;
  Sprite* sprite_poison;
  Sprite* sprite_hibernation;

  Sprite* plep_sullen_sting;
  Sprite* plep_befuddling_sting;
  Sprite* plep_chlorophoria;
  Sprite* plep_canopy;
  Sprite* plep_numbing_sting;
  Sprite* plep_toxic_sting;
  Sprite* plep_updraft;
  Sprite* plep_light_push;
  Sprite* plep_light_shot;
  Sprite* plep_prismatic_shot;
  Sprite* plep_rail_shot;
  Sprite* plep_shatter_shot;
  Sprite* plep_static_shot;
  Sprite* plep_locked_shot;
  Sprite* plep_strike;
  Sprite* plep_paw_strike;
  Sprite* plep_maul;
  Sprite* plep_multi_strike;
  Sprite* plep_ensnare;
  Sprite* plep_enrich;
  Sprite* plep_upgrade;

  Sprite* sprite_defend_start;
  Sprite* sprite_defend_break;
  Sprite* sprite_defend_persist;

  /* First run trigger */
  bool first_run;

  /* Font for rendering */
  TTF_Font* font_normal;

  /* The configuration for the display of the game */
  Options* game_config;

  /* Inventories for the dudes */
  Inventory* inventory_allies;
  Inventory* inventory_foes;

  /* Levels */
  uint8_t lvl_foe;
  uint8_t lvl_friend;

  /* Menu items */
  uint8_t menu_index;
  std::vector<Text*> menu_items;
  std::vector<Text*> menu_items_sel;

  /* The mode of the test battle interface */
  TestMode mode;

  /* Parties */
  Party* party_foes;
  Party* party_friends;

  /* Races */
  Category* race_arcadius;
  Category* race_robot;
  Category* race_bear;
  Category* race_human;
  Category* race_treefolk;

  /* List of all skills and skillsets - just for management */
  std::vector<Skill*> skills;
  std::vector<SkillSet*> skillsets;

  /* Attribute Sets */
  AttributeSet stats_bear_min;
  AttributeSet stats_bear_max;

  AttributeSet stats_arcadius_race_min;
  AttributeSet stats_arcadius_race_max;

  AttributeSet stats_arcadius_cat_min;
  AttributeSet stats_arcadius_cat_max;

  AttributeSet stats_aurora_agent_min;
  AttributeSet stats_aurora_agent_max;

  AttributeSet stats_aurora_heavy_min;
  AttributeSet stats_aurora_heavy_max;

  AttributeSet stats_human_min;
  AttributeSet stats_human_max;

  AttributeSet stats_aurora_eng_min;
  AttributeSet stats_aurora_eng_max;

  AttributeSet stats_aurora_drone_min;
  AttributeSet stats_aurora_drone_max;

  AttributeSet stats_robot_min;
  AttributeSet stats_robot_max;

  AttributeSet stats_treefolk_min;
  AttributeSet stats_treefolk_max;

  AttributeSet stats_reverdling_min;
  AttributeSet stats_reverdling_max;

  AttributeSet stats_reverdile_min;
  AttributeSet stats_reverdile_max;

  AttributeSet stats_player_min;
  AttributeSet stats_player_max;

  /* ------------ Constants --------------- */
  const static uint8_t kLVL_MAX; /* Max level for person */
  const static std::string kMENU_ITEMS[]; /* The stored menu items */
  const static std::string kMENU_FOE; /* Foe preface text */
  const static std::string kMENU_FRIEND; /* Friend preface text */
  const static uint8_t kNUM_MENU_ITEMS; /* Number of menu items in screen */

  /*=============================================================================
   * PRIVATE FUNCTIONS
   *============================================================================*/
private:
  /* Builds the ailment frames */
  void buildBattleDisplay(SDL_Renderer* renderer);

  /* Main create call - first construction */
  void create();

  /* Create sub calls */
  void createActions();
  void createClasses();
  void createInventories();
  void createMenu();
  Person* createPerson(int id, TestPerson type, SDL_Renderer* renderer,
                       uint8_t level = 1, bool include_ai = true);
  void createRaces();
  void createSkills();
  void createSkillSets();

  /* Delete sub calls */
  void deleteActions();
  void deleteClasses();
  void deleteInventories();
  void deleteMenu();
  void deleteRaces();
  void deleteSkills();
  void deleteSkillSets();

  /* Main destruction call - ending */
  void destroy();

  /* Battle destruction */
  void destroyBattle();

  /* Get calls */
  Skill* getSkill(int id);
  SkillSet* getSkillSet(int id);

  /* Start a battle with the selected parameters */
  void initBattle(SDL_Renderer* renderer);

  /*============================================================================
   * PUBLIC FUNCTIONS
   *===========================================================================*/
public:
  /* The key up and down events to be handled by the class */
  bool keyDownEvent(SDL_KeyboardEvent event);
  void keyUpEvent(SDL_KeyboardEvent event);

  /* Renders the title screen */
  bool render(SDL_Renderer* renderer);

  /* Set the running configuration, from the options class */
  bool setConfiguration(Options* running_config);

  /* Updates the game state */
  bool update(int cycle_time);
};

#endif // TESTBATTLE_H
