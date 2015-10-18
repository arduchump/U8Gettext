
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
  uint16_t i = 0;
  const U8GettextTranslation *translation = NULL;

  if (!sLanguageInstance) {
    return str;
  }

  translation = sLanguageInstance->translations;
  for(i = 0; i < sLanguageInstance->translationCount; ++i, ++translation) {
    // Slowest way to find translation
    if(0 == strcmp(str, translation->msgId)) {
      return translation->msgStr;
    }
  }

  // Can't find any translations!
  return str;
}
