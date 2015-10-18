
#include "U8Gettext.h"
#include <Arduino.h>

#define ENSURE(condition) do { if (!(condition)) {Serial.println("ENSURE failed at: "#condition); while(1); } } while(0)
#define SIZE_OF_ARRAY(array) (sizeof((array))/sizeof((array)[0]))
#define PTR_OFFSET_BYTES(ptr, nBytes) ((void*)(((char *)(ptr)) + (nBytes)))
#define PTR_OFFSET_BETWEEN(ptrBegin, ptrEnd) ((char*)(ptrEnd) - (char*)(ptrBegin))

static const U8GettextLanguage *
sLanguageInstance = NULL;

static size_t sLanguagesLength = 0;
static const U8GettextLanguage *sLanguages = NULL;

void _U8GettextInitialize(const U8GettextLanguage * languages, const size_t languageCount, const char *language)
{
  sLanguages = languages;
  sLanguagesLength = languageCount;
  U8GettextSetLanguage(language);
}

const char *U8GettextSetLanguage(const char *language)
{
  uint16_t i = 0;
  const char * oldLanguage = "";
  const U8GettextLanguage * languageInstance = NULL;

  if (sLanguageInstance) {
    oldLanguage = sLanguageInstance->language;
  }

  if (!sLanguages) {
    // If we have not initialized the gettext ...
    return oldLanguage;
  }

  languageInstance = sLanguages;
  for(i = 0; i < sLanguagesLength; ++i, ++languageInstance) {
    // Set the language instance by specific language
    if(0 == strcmp(language, languageInstance->language)) {
      sLanguageInstance = languageInstance;
      break;
    }
  }

  return oldLanguage;
}

const char *U8GettextGetLanguage()
{
  if (sLanguageInstance) {
    return sLanguageInstance->language;
  }

  // If you have not set any langauge, it return empty string. So that
  // it won't cause program crash if any other function invoke this
  // function without set language.
  return "";
}

const char *U8Gettext(const char *str)
{
  uint16_t i = 0;
  const U8GettextTranslation *translation = NULL;

  if (!sLanguageInstance) {
    return str;
  }

  translation = sLanguageInstance->translations;
  for(i = 0; i < *sLanguageInstance->translationCount; ++i, ++translation) {
    // Slowest way to find translation
    if(0 == strcmp(str, translation->msgId)) {
      return translation->msgStr;
    }
  }

  // Can't find any translations!
  return str;
}
