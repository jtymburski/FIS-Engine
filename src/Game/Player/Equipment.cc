
/******************************************************************************
* Class Name: Equipment
* Date Created: Dec 2 2012
* Inheritance: Item
* Description: Equipment that categorizes under Item in an Inventory unit. Gets
*              called by Person as well for attached Equipment.
* TODO: Finish actions [01-31-13]
* TODO: Bubby attachment algorithm [01-31-13]
* TODO: Bubby signature bonus algorithm [01-31-13]
******************************************************************************/
#include "Game/Player/Equipment.h"

/*=============================================================================
 * CONSTANTS
 *============================================================================*/
const ushort Equipment::kMAX_X = 9;
const ushort Equipment::kMAX_Y = 9;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Equipment class consructor
 *
 * Inputs: none
 */
Equipment::Equipment()
{
  // INITIALIZE SIGNATURE

  for (int i = 0; i < kMAX_X; i++)
      for (int j = 0; j < kMAX_Y; j++)
          bubby_signature[i][j] = NULL;
}

/*
 * Description: Equipment class destructor
 */
Equipment::~Equipment()
{
  for (int i = 0; i < kMAX_X; i++)
    for (int j = 0; j < kMAX_Y; j++)
      if (bubby_signature[i][j] != NULL)
        qDebug() << "WARNING: DELETING EQUIPMENT WITHOUT REMOVING BUBBIES";
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Checks if the bubby will fit into the bubby signature
 *
 * Inputs: Bubby* - pointer to bubby to be checked for attachment capability
 *         int x - the left most coordinate of the proposed location
 *         int y - the top most coordinate of the proposed location
 * Output: bool - boolean if the bubby can be attached
 */
bool Equipment::canAttach(Bubby* new_bubby, ushort x, ushort y)
{
  if (new_bubby->getTier() == 0)
    return false;
  if (signature[x][y] == 'O')
  {
    if (new_bubby->getTier() == 2 && signature[x][y - 1] == 'O')
      return TRUE;
    if (new_bubby->getTier() == 3)
      if (signature[x + 1][y] == 'O' && signature[x + 1][y - 1] == 'O')
        return TRUE;
    return TRUE;
  }
  return false;
}

/*
 * Description: Attempts to attach the bubby into the signature. WARNING:
 *              Only give it the left most and top most coordinate!
 *
 * Inputs: Bubby* - pointer of the bubby to be attached
 *         int x - the left most coordinate
 *         int y - the top most coordinate
 * Output: bool - true if the attachment was successful
 */
bool Equipment::attachBubby(Bubby* new_bubby, ushort x, ushort y)
{
  if (new_bubby->getTier() == 0 || x + 1 >= kMAX_X || y + 1 >= kMAX_Y)
    return false;

  bubby_signature[x][y] = new_bubby;
  signature[x][y] = 'B';

  if (new_bubby->getTier() > 1)
      signature[x][y - 1] = 'C';
  if (new_bubby->getTier() == 3)
  {
      signature[x + 1][y] = 'C';
      signature[x + 1][y - 1] = 'C';
  }
  return TRUE;
}

/*
 * Description: Checks if a Bubby at a given index can be unattached
 *
 * Inputs: ushort x - x coordinate of the Bubby
 *         ushort y - y coordinate of the Bubby
 * Output: bool - TRUE if the Bubby can be unattached
 */
bool Equipment::canUnattach(ushort x, ushort y)
{
  int left_x = getLeftX(x, y);
  int top_y = getTopY(x, y);

  if (left_x == -1 || top_y == -1)
    return false;

  if (signature[left_x][top_y] == 'B')
    return TRUE;
  return false;
}

/*
 * Description: Checks if a Bubby of a given ID can be unattached
 *
 * Inputs: ushort x - x coordinate of the Bubby
 *         ushort y - y coordinate of the Bubby
 * Output: bool - TRUE if the Bubby can be unattached
 */
bool Equipment::canUnattach(uint id)
{
  int left_x = getLeftX(id);
  int top_y =  getTopY(id);

  if (left_x == -1 || top_y == -1)
    return false;

  if (signature[left_x][top_y] == 'B')
    return TRUE;
  return false;
}

/*
 * Description: Unattaches a Bubby at a given (leftmost x, leftmost y) coord
 *
 * Inputs: ushort x - leftmost x coordinate of the Bubby
 *         ushort y - leftmost y coordinate of the Bubby
 * Output: Bubby - the (now) unattached Bubby
 */
Bubby Equipment::unattach(ushort x, ushort y)
{
  int left_x = getLeftX(x, y);
  int top_y  = getTopY(x, y);

  if (bubby_signature[left_x][top_y]->getTier() > 0)
      signature[left_x][top_y] = 'O';
  if (bubby_signature[left_x][top_y]->getTier() > 1)
    signature[left_x][top_y - 1] = 'O';
  if (bubby_signature[left_x][top_y]->getTier() > 2)
  {
    signature[left_x + 1][top_y] = 'O';
    signature[left_x + 1][top_y - 1] = 'O';
  }
  return *bubby_signature[left_x][top_y];
}

/*
 * Description: Returns a vector of all attached Bubbies on the equipment.
 *
 * Inputs: none
 * Output: QVector<Bubby*> - vector of pointers to attached Bubbies
 */
QVector<Bubby*> Equipment::getAttachedBubbies()
{
  QVector<int> bubby_ids;
  QVector<Bubby*> attached_bubbies;


  for (int i = 0; i < kMAX_X; i++)
  {
    for (int j = 0; j < kMAX_Y; j++)
    {
      if (signature[i][j] == 'B')
      {
        if (!bubby_ids.contains(bubby_signature[i][j]->getId()))
        {
          bubby_ids.push_back(bubby_signature[i][j]->getId());
          attached_bubbies.push_back(bubby_signature[i][j]);
        }
      }
    }
  }

  return attached_bubbies;
}

/*
 * Description: Returns a vector of all the unique flavours of Bubbies which are
 *              attached to the equipment.
 *
 * Inputs: none
 * Output: QVector<Bubby*> - vector of pointers to attached Bubbies
 */
QVector<BubbyFlavour*> Equipment::getAttachedFlavours()
{
  QVector<BubbyFlavour*> flavours;
  QVector<QString> flavour_names;

  for (int i = 0; i < kMAX_X; i++)
  {
    for (int j = 0; j < kMAX_Y; j++)
    {
      if (signature[i][j] == 'B')
      {
        if (!flavour_names.contains(bubby_signature[i][j]->getType()->getName()))
        {
          flavours.push_back(bubby_signature[i][j]->getType());
          flavour_names.push_back(bubby_signature[i][j]->getType()->getName());
        }
      }
    }
  }

  return flavours;
}

/*
 * Description: Evaluates a given EquipmentState flag
 *
 * Inputs: EquipmentState flag to be evaluated
 * Output: boolean value of the flag
 */
bool Equipment::getEquipmentFlag(EquipmentState flag)
{
  return (eflag_set.testFlag(flag));
}

/*
 * Description: Calculates and returns the skills the equipmenmt makes available
 *              for used based off an input (the level of the character the
 *              equipment is equipped to
 *
 * Inputs: ushort level - the level of the person the equipment is attached to
 * Output: SkillSet* - pointer to the skill set the equipment provides
 */
SkillSet* Equipment::getSkills(ushort level)
{
  SkillSet* skills;
  QVector<Skill*> temp_skills;
  QVector<ushort> temp_skill_levels;

  QVector<BubbyFlavour*> flavour_list = getAttachedFlavours();

  for (int i = 0; i < flavour_list.size(); i++)
  {
    int tier = getLargestTier(flavour_list.at(i));
    int max_level = BubbyFlavour::getMaxSkillLevel(tier);
    temp_skills = flavour_list[i]->getSkillSet()->getSkills();
    temp_skill_levels = flavour_list[i]->getSkillSet()->getSkillLevels();\

    for (int j = 0; j < temp_skills.size(); i++)
    {
      if (temp_skill_levels.at(j) <= max_level)
        skills->addSkill(temp_skills.at(i));
    }

    temp_skills.clear();
    temp_skill_levels.clear();
  }

  return skills;
}

/*
 * Description: Given a BubbyFlavour, returns the largest tiered Bubby of that
 *              flavour
 *
 * Inputs: QString - the name of the flavour to be checked for
 * Output: int - the largest tier'd Bubby of the given flavour
 */
int Equipment::getLargestTier(BubbyFlavour* flavour)
{
  int largest_tier = 0;

  for (int i = 0; i < kMAX_X; i++)
    for (int j = 0; j < kMAX_Y; j++)
      if (bubby_signature[i][j]->getType()->getName() == flavour->getName())
        if (bubby_signature[i][j]->getTier() > largest_tier)
          largest_tier = bubby_signature[i][j]->getTier();

  return largest_tier;

}

/*
 * Description: Gets the left most X-coordinate on the signature of a Bubby
 *              given the ID of the Bubby (assumes that the ID exists).
 *
 * Inputs: uint - unique ID or x-value of Bubby to be checked
 * Output: int - the leftmost x-coordinate of the Bubby if found, -1 otherwise
 */
int Equipment::getLeftX(int id)
{
  for (int x = 1; x < kMAX_X; x++)
  {
    for (int y = 1; y < kMAX_Y; y++)
    {
      if (bubby_signature[x][y]->getId() == id)
      {
        if (bubby_signature[x - 2][y]->getId() == bubby_signature[x][y]->getId())
          return x - 2;
        if (bubby_signature[x - 1][y]->getId() == bubby_signature[x][y]->getId())
          return x - 1;
        return x;
      }
    }
  }
  return -1;
}

/*
 * Description: Gets the left most X-coordinate on the signature of a Bubby
 *              given .
 *
 * Inputs: uint - unique ID of Bubby to be checked
 *         bool - TRUE if passing an ID rather than an x-coordinate
 * Output: int - the leftmost x-coordinate of the Bubby if found, -1 otherwise
 */
int Equipment::getLeftX(uint x, uint y)
{
  /* If the location to be checked does not have a Bubby, there is no left X */
  if (signature[x][y] != 'B')
    return -1;

  /* 1x1 Bubbies and x == 1 are already the most left X */
  if (bubby_signature[x][y]->getTier() == 1 || x == 1)
    return x;

  /* Tier 2 Bubbies */
  if (bubby_signature[x][y]->getTier() == 2)
  {
    if (bubby_signature[x][y]->getId() == bubby_signature[x - 1][y]->getId())
      return x - 1;
    return x;
  }

  /* Tier 3 Bubbies */
  int left_x = x;
  if (bubby_signature[x][y]->getTier() == 3)
  {
      if (bubby_signature[x][y]->getId() == bubby_signature[x - 1][y]->getId())
        left_x = x - 1;
      if (bubby_signature[x][y]->getId() == bubby_signature[x - 2][y]->getId())
        left_x = x - 2;
  }
  return left_x;
}

/*
 * Description: Gets the top most Y coordinate of a Bubby given an ID that is
 *              presumed to exist
 * Inputs: uint - unique ID of Bubby to be checkede
 * Output: int - the leftmost y-coordinate of the Bubby if found, -1 otherwise
 */
int Equipment::getTopY(int id)
{
  for (int x = 1; x < kMAX_X; x++)
  {
    for (int y = 1; y < kMAX_Y; y++)
    {
      if (bubby_signature[x][y]->getId() == id)
      {
        if (bubby_signature[x - 2][y]->getId() == bubby_signature[x][y]->getId())
          return x - 2;
        if (bubby_signature[x - 1][y]->getId() == bubby_signature[x][y]->getId())
          return x - 1;
        return x;
      }
    }
  }
  return -1;
}

/*
 * Description: Gets the top most Y coordinate of a Bubby given a known location
 *              that the Bubby exists.
 * Inputs: uint - unique ID or y-value of Bubby to be checked
 *         bool - TRUE if passing an ID rather than a y-coordinate
 * Output: int - the leftmost y-coordinate of the Bubby if found, -1 otherwise
 */
int Equipment::getTopY(uint x, uint y)
{
  /* If the location to be checked does not have a Bubby, there is no top Y */
  if (signature[x][y] != 'B')
    return -1;

  /* 1x1 Bubbies and y == 1 are already the most top Y */
  if (bubby_signature[x][y]->getTier() == 1 || y == 1)
    return y;

  /* Tier 2 Bubbies */
  if (bubby_signature[x][y]->getTier() == 2)
  {
    if (bubby_signature[x][y]->getId() == bubby_signature[x][y - 1]->getId())
      return y - 1;
    return y;
  }

  /* Tier 3 Bubbies */
  int top_y = y;
  if (bubby_signature[x][y]->getTier() == 3)
  {
    if (bubby_signature[x][y]->getId() == bubby_signature[x][y - 1]->getId())
      top_y = y - 1;
    if (bubby_signature[x][y]->getId() == bubby_signature[x][y - 2]->getId())
      top_y = y - 2;
  }
  return top_y;
}

/*
 * Description: Given an EquipmentFlags object and a bool, sets the flags
 *              contained in the object to the boolean value.
 *
 * Inputs: EquipmentState flag to be set
 *         set_value - boolean value to set flags to
 * Output: none
 */
void Equipment::setEquipmentFlag(EquipmentState flag, bool set_value)
{
    (set_value) ? (eflag_set |= flag) : (eflag_set ^= flag);
}

/*=============================================================================
 * PUBLIC STATIC FUNCTIONS
 *============================================================================*/

/*
 * Description: Prints out all the info of the equipment by calling the other
 *              print functions.
 *
 * Inputs: none
 * Output: none
 */
void Equipment::printAll()
{
  qDebug() << " --- Equipment --- ";
  printFlags();
  printInfo();
  printSignature();
  qDebug() << " --- / Equipment --- ";
}

/*
 * Description: Prints out the info of the equipment
 *
 * Inputs: none
 * Output: none
 */
void Equipment::printFlags()
{
  qDebug() << "WEAPON: " << getEquipmentFlag(Equipment::WEAPON);
  qDebug() << "METAL: " << getEquipmentFlag(Equipment::METAL);
  qDebug() << "BROKEN: " << getEquipmentFlag(Equipment::BROKEN);
  qDebug() << "HEAD: " << getEquipmentFlag(Equipment::HEAD);
  qDebug() << "LEFT ARM: " << getEquipmentFlag(Equipment::LEFTARM);
  qDebug() << "RIGHT ARM: " << getEquipmentFlag(Equipment::RIGHTARM);
  qDebug() << "TORSO: "<< getEquipmentFlag(Equipment::TORSO);
  qDebug() << "LEGS: " << getEquipmentFlag(Equipment::LEGS);
  qDebug() << "TWOHAND: " << getEquipmentFlag(Equipment::TWOHAND);
  qDebug() << "EQUIPPED: " << getEquipmentFlag(Equipment::EQUIPPED);
}

/*
 * Description: Prints out the info of the equipment
 *
 * Inputs: none
 * Output: none
 */
void Equipment::printInfo()
{
    qDebug() << getName();
}

/*
 * Description: Prints out the info of the equipment
 *
 * Inputs: none
 * Output: none
 */
void Equipment::printSignature()
{
  for (int i = 0; i < kMAX_X; i++)
  {
    for (int j = 0; j < kMAX_Y; j++)
    {
      qDebug() << signature[i][j];
    }
  }
}
