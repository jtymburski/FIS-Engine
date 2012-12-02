/******************************************************************************
* Class Name: SectorAction
* Date Created: Oct 28 2012
* Inheritance: QWidget
* Description: The SectorAction class
******************************************************************************/
#ifndef SECTORACTION_H
#define SECTORACTION_H

/* The types of actions available(Will be added as needed) */
enum SectorActionType{CUTSCENE,DIALOG};

class SectorAction : public QWidget
{
public:
  /* Constructor function */
  SectorAction(QWidget *parent = 0);

  /* Destructor function */
  ~SectorAction();

private:
  /* Maximum number of times this action can be triggered */
  int max_triggers;

  /* Number of times this has been triggered */
  int triggers;

  /* The type of action for this sector */
  SectorActionType type;

  /* Flag for if this is an enter trigger */
  bool ENTER;

  /* Flag for if this is a leave trigger */
  bool LEAVE;

public:
  /* True if the action can be performed */
  bool isValid();

  /* Performs the sector action */
  void perform();
};

#endif // SECTORACTION_H
