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

#include "Game/Player/AttributeSet.h"
#include "Game/Player/SkillSet.h"
#include "Game/Frame.h"

class Flavour
{
public:
  Flavour();

  Flavour(const std::string &flavour_name, const AttributeSet &min_stats,
  	      const double &min_mass, const SkillSet* skills = nullptr);

  ~Flavour();

private:

  const AttributeSet &base_stats;

  const double base_mass;

  std::string description;

  std::vector<std::string> flavour_list;

  const std::string name;

  SkillSet* skill_list;

  std::vector<Frame*> thumbnails;

  /* ------------ Constants --------------- */
  static const size_t             kMAX_DESC_LENGTH; /* Maximum desc size */
  static const std::vector<float> kTIER_MODIFIER;   /* Stat modifiers */
  static const std::vector<int>   kTIER_LEVELS;     /* Levels at tier up */
  static const std::vector<float> kTIER_MASSES;     /* Mass modifiers */

/*=============================================================================
 * PRIVATE FUNCTIONS
 *============================================================================*/
private:

  static bool addFlavour(const std::string &new_flavour);

/*=============================================================================
 * PUBLIC FUNCTIONS
 *============================================================================*/
public:

  const AttributeSet& getBaseStats();

  std::string getDescription();

  double getMass(const uint &tier);

  const std::string& getName();

  std::deque<SkillSetElement> getSkills(const uint &at_level);

  SkillSet* getSkillSet();

  /* Obtains the stats at a given tier */
  AttributeSet getStats(const uint &tier);

  Frame* getThumb(const uint &index);

  bool setDescription(const std::string& new_description);

  bool setSkills(const SkillSet* new_skills);

  bool setThumbs(const std::vector<Frame*> &thumbnails);

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

  static std::vector<std::string> getFlavours();

};

#endif //FLAVOUR_H
