/*
 * parse.h: A plugin for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef _PARSE_H
#define _PARSE_H

#include <vdr/tools.h>
#include <vdr/epg.h>
#include <libxml/parser.h>
#include <time.h>

#include "maps.h"

class cXMLTVEvent
{
private:
    char *title;
    char *shorttext;
    char *description;
    char *country;
    char *system;
    char *review;
    char *rating;
    char *origtitle;
    char *director;
    int year;
    time_t starttime;
    int duration;
    time_t vps;
    tEventID eventid;
    cStringList actors;
    cStringList others;
#if VDRVERSNUM >= 10711
    uchar parentalRating;
    uchar contents[MaxEventContents];
#endif
public:
    cXMLTVEvent();
    ~cXMLTVEvent();
    bool Add2Description(const char *Name, const char *Value);
    bool Add2Description(const char *Name, int Value);
    bool Add2Description(const char *Value);
    void Clear();
    void SetTitle(const char *Title);
    void SetOrigTitle(const char *OrigTitle);
    void SetShortText(const char *ShortText);
    void SetDescription(const char *Description);
    void SetDirector(const char *Director);
    void AddActor(const char *Actor, const char *ActorRole=NULL);
    void AddOther(const char *OtherType, const char *Other);
    void SetCountry(const char *Country);
    void SetReview(const char *Review);
    void SetRating(const char *System, const char *Rating);
    void SetYear(int Year)
    {
        year=Year;
    }
    void SetStartTime(time_t StartTime)
    {
        starttime=StartTime;
    }
    void SetDuration(int Duration)
    {
        duration=Duration;
    }
    void SetVps(time_t Vps)
    {
        vps=Vps;
    }
    void SetEventID(tEventID EventID)
    {
        eventid=EventID;
    }
    time_t Vps(void) const
    {
        return vps;
    }
    int Duration()
    {
        return duration;
    }
    time_t StartTime()
    {
        return starttime;
    }
    const char *Title(void) const
    {
        return title;
    }
    const char *ShortText(void) const
    {
        return shorttext;
    }
    const char *Description(void) const
    {
        return description;
    }
    const char *Country(void) const
    {
        return country;
    }
    int Year()
    {
        return year;
    }
    const char *Review(void) const
    {
        return review;
    }
    const char *Rating(void) const
    {
        return rating;
    }
    const char *RatingSystem(void) const
    {
        return system;
    }
    const char *OrigTitle(void) const
    {
        return origtitle;
    }
    tEventID EventID(void) const
    {
        return eventid;
    }
    cStringList *Actors(void)
    {
        return &actors;
    }
    const char *Director(void) const
    {
        return director;
    }
    cStringList *Others(void)
    {
        return &others;
    }
};

class cParse
{
    struct split
    {
        char *pointers[256];
        int count;
    };

    enum
    {
        PARSE_NOERROR=0,
        PARSE_NOSCHEDULE=1,
        PARSE_NOCHANNEL=2,
        PARSE_NOSCHEDULES=3,
        PARSE_XMLTVERR=4,
        PARSE_NOMAPPING=5,
        PARSE_NOCHANNELID=6,
	PARSE_EMPTYSCHEDULE=7
    };

private:
    char *name;
    cEPGMappings *maps;
    cTEXTMappings *texts;
    cXMLTVEvent xevent;
    cCharSetConv *conv;
    char *RemoveNonASCII(const char *src);
    struct split split(char *in, char delim);
    u_long DoSum(u_long sum, const char *buf, int nBytes);
    cEvent *SearchEvent(cSchedule* schedule, cXMLTVEvent *xevent);
    time_t ConvertXMLTVTime2UnixTime(char *xmltvtime);
    bool FetchEvent(xmlNodePtr node);
    cEPGMapping *EPGMapping(const char *ChannelName);
    cTEXTMapping *TEXTMapping(const char *Name);
    bool PutEvent(cSchedule* schedule,cEvent *event,cXMLTVEvent *xevent, cEPGMapping *map);
public:
    cParse(const char *Name, cEPGMappings *Maps, cTEXTMappings *Texts);
    ~cParse();
    bool Process(char *buffer, int bufsize);
    static void InitLibXML();
    static void CleanupLibXML();
};

#endif
