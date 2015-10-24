
#include "U8Gettext.h"
#include <Arduino.h>
#include <Utf8.h>

#define ENSURE(condition) do { if (!(condition)) {Serial.println("ENSURE failed at: "#condition); while(1); } } while(0)
#define SIZE_OF_ARRAY(array) (sizeof((array))/sizeof((array)[0]))
#define PTR_OFFSET_BYTES(ptr, nBytes) ((void*)(((char *)(ptr)) + (nBytes)))
#define PTR_OFFSET_BETWEEN(ptrBegin, ptrEnd) ((char*)(ptrEnd) - (char*)(ptrBegin))

static const U8GettextLanguage *
sLanguageInstance = NULL;

static const U8GettextContext *
sContext = NULL;

void _u8gettextInitialize(
    const U8GettextContext * context,
    const char *language)
{
  sContext = context;
  u8gettextSetLanguage(language);
}

const char *u8gettextSetLanguage(const char *language)
{
  uint16_t i = 0;
  const char * oldLanguage = "";
  const U8GettextLanguage * languageInstance = NULL;

  if (sLanguageInstance) {
    oldLanguage = sLanguageInstance->language;
  }

  if (!sContext) {
    // If we have not initialized the gettext ...
    return oldLanguage;
  }

  languageInstance = sContext->languages;
  // If we can't find any language matched user provided language,
  // we will reset the langugae instance to NULL.
  sLanguageInstance = NULL;
  for(i = 0; i < *sContext->languageCount; ++i, ++languageInstance) {
    // Set the language instance by specific language
    if(0 == strcmp(language, languageInstance->language)) {
      sLanguageInstance = languageInstance;
      break;
    }
  }

  return oldLanguage;
}

const char *u8gettextGetLanguage()
{
  if (sLanguageInstance) {
    return sLanguageInstance->language;
  }

  // If you have not set any langauge, it return empty string. So that
  // it won't cause program crash if any other function invoke this
  // function without set language.
  return "";
}

const char *u8gettext(const char *str)
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

static char utf8ToU8GlibFontEncoding(const char *str)
{
  size_t i;
  uint32_t utf32Char;
  U8GettextCharMapping charMapping;

  utf32Char = utf8ToUtf32(str);

  // Slowest way to translate string to U8Glib font encoding
  for(i = 0; i < *sContext->charMappingCount; ++ i) {
    memcpy_P(&charMapping, sContext->charMappings + i, sizeof(charMapping));
    if (charMapping.utf32Char == utf32Char) {
      return charMapping.u8gChar;
    }
  }

  return '\0';
}

const char *u8gettextU(const char *str)
{
  static char sBuffer[1024] = {0, };
  const char * translatedStr = NULL;
  char * position = NULL;
  const U8GettextTranslation *translation = NULL;

  sBuffer[0] = '\0';

  if (!sContext) {
    return sBuffer;
  }

  translatedStr = u8gettext(str);
  position = sBuffer;

  while(*str) {
    *position = utf8ToU8GlibFontEncoding(str);
    str = utf8FindNextChar(str);
    ++ position;
    if(PTR_OFFSET_BETWEEN(sBuffer, position) >= sizeof(sBuffer)) {
      break;
    }
  }

  *position = '\0';

  return sBuffer;
}
