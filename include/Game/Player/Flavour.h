/*******************************************************************************
* Class Name: Flavour [Declaration]
* Date Created: November 24th, 2013
* Inheritance: None
* Description: Bubbies of certain "flavours" (BubbyFlavours) have the same
*              AttributeSet bonus values as well as SkillSet unlocks as
*              other Bubbies of the same flavour.
*
* Notes
* -----
*
* [1]: 
*
* TODO
* ----
*
*******************************************************************************/
#ifndef FLAVOUR_H
#define FLAVOUR_H

#include <vector> 

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"
#include "Frame.h"
#include "StringDB.h"

class Flavour
{
public:
  /* Constructs a default Flavour object */
  Flavour();

  /* Constructs a normal Flavour object given a name and stats */
  Flavour(const int &game_id, const std::string &flavour_name, 
          const AttributeSet &min_stats, const double &min_mass, 
          const uint32_t &min_value, SkillSet* skills = nullptr);

  /* Annihilates a Flavour object */
  ~Flavour();

private:
  /* Ref to the base attribute set (const -> unchanging) */
  AttributeSet base_stats;

  /* The base mass of the Bubby Flavour (mass at Tier 0) */
  const double base_mass;

  /* Base value of the flavour */
  const uint32_t base_value;

  /* Description of the flavour */
  std::string description;

  /* The corresponding Game ID (non-unique) value for a Bubby of this flv */
  const int game_id;

  /* Standing list of all flavours in the game */
  static std::vector<Flavour*> flavour_list;

  /* The name of the Bubby Flavour */
  const std::string name;

  /* Pointer ot the list of skills the flavour can give to person */
  SkillSet* skill_list;

  /* Vector of frames (thumbnails) of images of each tier of the flavour */
  std::vector<Frame*> thumbnails;

  /* ------------ Constants --------------- */
  static const std::vector<float>    kTIER_MODIFIERS;   /* Stat modifiers */
  static const std::vector<uint32_t> kTIER_LEVELS;      /* Levels at tier up */
  static const std::vector<float>    kTIER_MASSES;      /* Mass modifiers */
  static const std::vector<float>    kTIER_VALUES;      /* Value modifiers */
  static const uint32_t              kMAX_LVL;          /* Max lvl */
  static const int                   kUNSET_ID;         /* Def. ID value */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:
  /* Adds a flavour to the list of flavours */
  static bool addFlavour(Flavour* new_flavour);

  /* Evaluates whether a given flavour exists */
  static bool isFlavour(Flavour* flavour_check);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns the base stats of the Flavour */
  AttributeSet getBaseStats();

  /* Returns the description of the Flavour */
  const std::string& getDescription();

  /* Returns the Game ID of the Flavour */
  const int& getGameID();

  /* Returns the current mass of the Flavour at a given tier */
  double getMass(const uint32_t &tier = 0);

  /* Returns the string name of the Flavour */
  const std::string& getName();

  /* Returns a deque of SkillSetElements the flavour unlocks at a given level */
  std::deque<SkillSetElement> getSkills(const uint32_t &at_level);

  /* Returns a pointer to the skill set for the Flavour */
  SkillSet* getSkillSet();

  /* Obtains the stats at a given tier */
  AttributeSet getStats(const uint32_t &tier = 0);
  
  /* Returns the thumbnail of the Flavour at a given tier */
  Frame* getThumb(const uint32_t &index);

  /* Returns the value of a Flavour at a given tier */
  uint32_t getValue(const uint32_t &tier);

  /* Assigns a new description to the flavour */
  bool setDescription(const std::string& new_description);

  /* Assigns a new set of skills to the Flavour */
  bool setSkills(SkillSet* new_skills);

  /* Assigns a new vector of thumbnails to the flavour */
  bool setThumbs(const std::vector<Frame*> &thumbnails);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/
public:
  /* Returns a vector of strings of the flavour names */
  static std::vector<Flavour*> getFlavours();

  /* Returns the minimum level at a given tier */
  static uint32_t getLevels(const uint32_t &tier = 0);

  /* Returns the maximum level of a Bubby */
  static uint32_t getMaxLevel();

    /* Returns the # of tiers of a Bubby */
  static uint32_t getTiers();

};

#endif //FLAVOUR_H
