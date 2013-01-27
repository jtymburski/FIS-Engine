/******************************************************************************
* Class Name: Action
* Date Created: Sunday, October 28th, 2012
* Inheritance: 
* Description: Display information on the screen about battle events
*
* TODO: CONSTRUCTORS TO BE FINISHED
*
* Notes: - Possibility to target person with most skills
*     - num_targets could be that value or less based on the number of 
*     legal targets or user option
*
* Parse layout: IF{[Element Type], [Greater/Lesser Qualifier], [Value]} : 
* [Upper/Lower or GIVE/TAKE] , [Skill Element 1] , [Skill Element 2] ,
* [Ignore Attack], [Ignore Atk Elm] . [Ignore Atk Elm 2] . [Etc.] , 
* [Ignore Def Elm 1] . [Ignore Def Elm 2] . [Etc.] , [Base Damage Value] , 
* [Variance] , [Chance]
*
* while (line available)
* { 
* string 1: string 2
* if (switch) boolean valid
* effect
* }
*
******************************************************************************/
#ifndef ACTION_H
#define ACTION_H

#include <QtGui/QWidget>

/*LOWER,RAISE,INFLICT,CURE,IGNORE,CHANCE,<,<=,>,>=,==,!=,
  THERMAL,THERMAL_AGGRESSION,THERMAL_FORTITUDE,
  POLAR,POLAR_AGGRESSION,POLAR_FORTITUDE,
  PRIMAL,PRIMAL_AGGRESSION,PRIMAL_FORTITUDE,
  CHARGED,CHARGED_AGGRESSION,CHARGED_FORTITUDE,
  CYBERNETIC,CYBERNETIC_AGGRESSION,CYBERNETIC_FORTITUDE,
  NIHIL,NIHIL_AGGRESSION,NIHIL_FORTITUDE,
  VITALITY,QUANTUM_DRIVE,AGILITY,LIMBERTUDE,UNBEARABILITY */
enum ActionType {SELF, SINGLE, SINGLEIGNOREUSER, PARTY,
         PARTYIGNOREUSER, EVERYONE, EVERYONEIGNOREUSER};

class Action : public QWidget 
{
public:
    /* Action constructor object */
  	Action(QWidget *pointer = 0);

    /* Annihilates an action object */
  	~Action();

private:
  /* The action's type (defined by enumerator) */
  ActionType type; 

  /* The health shift this action will cause (damage or healing) */
  int vitality_shift; 

  /* The action's use description */
  QString description,brief_description; 

  /* The actions name */
  QString name; //The actions name

  /* The sequence that defines an action */
  QVector<QString> sequence;

  /* The QD payment htat must be made to use this action */
  unsigned int cost; 

  /* The number of targets this acftion can target */
  unsigned int num_targets;

  /* Flag to determine if action is beneficial to you or party member */
  bool FRIENDLY;

  /* The language used to create actions, more can be added */
  QVector<QString> language;

public:
  /* Evaluates the FRIENDLY flag */
  bool isFriendly();

  /* Attempts to perform the action as defined by sequence, return false if it is invalid */
  bool use(); 

  /* Returns the brief description */
  QString getBriefDescription(); 

  /* Returns the category (for menu grouping) */
  QString getCategory(); 

  /* Returns the actions cost */
  unsigned int getCost(); 

  /* Returns the description */
  QString getDescription();

  /* Returns the action type */
  ActionType getMode(); 

  /* Returns the name */
  QString getName();

  /* Returns the number of targets this will require */
  unsigned int getNumTargets(); 

  /* Sets the action's cost */
  void setCost(unsigned int);

  /* Sets the action's mode */
  void setMode(ActionType a);

  /* Sets the action name */
  void setName(QString); 

  /* Sets the number of targets */
  void setNumTargets(unsigned int i); 
};

#endif // ACTION_H
