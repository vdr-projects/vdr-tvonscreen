/*
 * magazine.h
 *
 * See the README file for copyright information and how to reach the author.
 *
 * $Id: magazine.h,v 1.4 2004/03/05 12:16:26 schmitzj Exp $
 *
 */

#ifndef MAGAZINE_H
#define MAGAZINE_H

#include <vdr/plugin.h>
#include <time.h>
#include "anyfont.h"

void mzlog(int level, const char *fmt, ...);

class magazine : public cOsdObject
{
	cPlugin *parent;
	cOsdBase *osd;

	class cMenuEvent *me;
	class cMenuEditTimer *met;
	
	anyFont *f1,*f2,*f3,*f4;

	tWindowHandle timeline;
	tWindowHandle names;
	tWindowHandle sched1,sched2,sched3;
	tWindowHandle control;

	cMutexLock _mutexLock;
	const cSchedule** schedArray;
	int schedArrayNum;
	int currentFirst;
	time_t currentFirstTime;

	int evnum;
	class cEventInfo **ev1;
	class cEventInfo **ev2;
	class cEventInfo **ev3;
	int *fullHours;

	enum modes {SHOW,EDIT};
	enum modes curmode;

	unsigned short EDIT_curEvent;
	int EDIT_curChannel;
	int EDIT_curEVI;
	
	const class cEventInfo *getNext(const cSchedule *s,const cEventInfo *e);
	const class cEventInfo *getPrev(const cSchedule *s,const cEventInfo *e);
	cEventInfo **ev4ch(int);
	void searchcEvt();

//	void printHead(const cSchedule *s,tWindowHandle sched);
	void printHead(const cSchedule *s,int p);
	void showKeys(void);
	void showTimeline(void);
	void showHeads(void);
	void showScheds(void);
	void showSched(const cSchedule *s,cEventInfo **ev,tWindowHandle sched);
	void calcSched(const cSchedule *s,cEventInfo **ev);
	void calcScheds(void);

public:
	magazine(class cPlugin *);
	virtual ~magazine();
	virtual void Show(void);
	virtual eOSState ProcessKey(eKeys Key);
};

class cMenuEvent : public cOsdMenu {
private:
  const class cEventInfo *eventInfo;
public:
  cMenuEvent(const class cEventInfo *EventInfo, bool CanSwitch = false);
  cMenuEvent(bool Now);
  virtual eOSState ProcessKey(eKeys Key);
};
class cMenuEditTimer : public cOsdMenu {
private:
  cTimer *timer;
  cTimer data;
  int channel;
  bool addIfConfirmed;
  class cMenuEditDateItem *firstday;
  void SetFirstDayItem(void);
public:
  cMenuEditTimer(cTimer *Timer, bool New = false);
  virtual ~cMenuEditTimer();
  virtual eOSState ProcessKey(eKeys Key);
  };

#endif
