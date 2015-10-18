
#include "U8Gettext.h"
#include <Arduino.h>

#define ENSURE(condition) do { if (!(condition)) {Serial.println("ENSURE failed at: "#condition); while(1); } } while(0)
#define SIZE_OF_ARRAY(array) (sizeof((array))/sizeof((array)[0]))
#define PTR_OFFSET_BYTES(ptr, nBytes) ((void*)(((char *)(ptr)) + (nBytes)))
#define PTR_OFFSET_BETWEEN(ptrBegin, ptrEnd) ((char*)(ptrEnd) - (char*)(ptrBegin))

extern const size_t gU8GettextLanguagesLength;
extern const U8GettextLanguage gU8GettextLanguages[];

static const char *
sU8GettextLanguage = "en_US";

static const U8GettextLanguage *
sLanguageInstance = NULL;

const char *U8GettextSetLanguage(const char *language)
{
  const char * oldLanguage = sU8GettextLanguage;

  sU8GettextLanguage = language;
  return oldLanguage;
}

const char *U8GettextGetLanguage()
{
  return sU8GettextLanguage;
}

const char *U8Gettext(const char *str)
{
  // TODO Implementation
  if (!sLanguageInstance) {
    return str;
  }
}
