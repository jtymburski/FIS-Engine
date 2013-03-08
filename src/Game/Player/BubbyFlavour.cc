/*******************************************************************************
* Class Name: BubbyFlavour
* Date Created: March 6th, 2013
* Inheritance: None
* Description: Describes the flavour of a Bubby. Every Bubby of certain flavours
*              will have the same SkillSet and AttributeSet as others.
*******************************************************************************/

#include "Game/Player/BubbyFlavour.h"

BubbyFlavour::BubbyFlavour(QString name, AttributeSet stats, SkillSet *skills)
{
  setName(name);
  setAttr(stats);
  setSkillSet(skills);

  for (int i = 0; i < kTIER_CAP; i++)
      sprites.push_back(NULL);
}

BubbyFlavour::~BubbyFlavour()
{
  delete skill_list;
  skill_list = NULL;
  for (int i = 0; i < sprites.size(); i++)
  {
    delete sprites.at(i);
    sprites[i] = NULL;
  }
}

AttributeSet* BubbyFlavour::getAttr()
{
  return &stats;
}

QString BubbyFlavour::getName()
{
  return name;
}

QString BubbyFlavour::getDescription()
{
  return description;
}

SkillSet* BubbyFlavour::getSkillSet()
{
  return skill_list;
}

QVector<Sprite*> BubbyFlavour::getSprites()
{
  return sprites;
}

void BubbyFlavour::setAttr(AttributeSet new_stats)
{
  stats = new_stats;
}

void BubbyFlavour::setName(QString new_name)
{
  name = new_name;
}

void BubbyFlavour::setDescription(QString new_description)
{
  description = new_description;
}

void BubbyFlavour::setSkillSet(SkillSet* new_skill_list)
{
  skill_list = new_skill_list;
}

void BubbyFlavour::setSprites(QVector<Sprite*> new_sprites)
{
  sprites = new_sprites;
}
