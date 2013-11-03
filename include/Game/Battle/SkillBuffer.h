/*******************************************************************************
* Class Name: Skill Buffer [Declaration]
* Date Created: October 28th, 2013
* Inheritance: None
* Description: Skill Buffer is a helper class for Battle which contains the stack
*              of skills with users and targets.
*******************************************************************************/
#ifndef SKILLBUFFER_H
#define SKILLBUFFER_H

#include "Game/Player/Person.h"

class SkillBuffer
{
public:
  /* Creates a SkillBuffer object */
  SkillBuffer();

  /* Annihilates a SkillBuffer object */
  ~SkillBuffer();

private:
  /* Structure containing the data describing one element in the skill buffer */
  struct SkillUseAction
  {
    Person* user;
    Skill* skill_used;
    QVector<Person*> targets;
    bool valid_skill_use;
  };

  /* The current index of the SkillUse buffer */
  int curr_index;
  
  /* QList describing the elements of the SkillBuffer */
  QList<SkillUseAction> skill_buffer;

  /* ------------ Constants --------------- */
  static const ushort kMAXIMUM_ELEMENTS;
  static const ushort kMAXIMUM_TARGETS;
  static const ushort kSTARTING_ELEMENT;
  
/*==============================================================================
 * PRIVATE FUNCTIONS
 *=============================================================================*/
private:
  /* Asserts that a given SkillUseAction is a valid skill use */
  bool checkValid(SkillUseAction skill_use_action);
  
  /* Clears the vector of SkillUseActions -- Does not delete skill/person data */
  void clearAll();
  
  /* Returns the entire structure of a SkillBuffer element */
  SkillUseAction getIndex(int index);

/*==============================================================================
 * PUBLIC FUNCTIONS
 *=============================================================================*/
public:
  /* Attempts to add a SkillUseAction to the Skill Buffer */
  bool addSkillUse(Person* user, Skill* skill_used, QVector<Person*> targets);
 
 /* Calculates and clears the invalid SkillUseActions from the SkillBuffer */
 void clearInvalid();
 
 /* Evaluates whether the next SkillUseAction is valid */
 bool isNextValid();
 
 /* Methods for printing out basic information describing Skill Buffer */
 void printAll();
 void printInfo();
 bool printElement(int index);
 
 /* Removes a SkillBuffer element by a given user of the item */
 bool removeSkillUse(Person* user);
 
 /* Removes a SkillBuffer element by a given index */
 bool removeSkillUse(int index);
 
 /* Returns a pointer to the current user on the ItemBuffer */
 Person* getUser();
 
 /* Returns a pointer to the current skill on the SkillBuffer */
 Skill* getSkill();
 
 /* Returns the vector of targets of the current SkillBuffer index */
 QVector<Person*> getTargets();
 
 /* Assigns the curr_index to the next element if one exists */
 bool setNextIndex();
};

#endif // SKILLBUFFER_H
