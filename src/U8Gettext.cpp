
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

static char * sBuffer = NULL;
static int16_t sBufferSize = 0; // The last character must be '\0'

/**
 * @brief flashStrcmp
 *
 * Compare two string in flash
 *
 * @param s1
 * @param s2
 */
static int flashStrcmp(const U8GFChar * s1, const U8GFChar * s2)
{
  if(s1 == s2)
  {
    return 0;
  }

  for(; pgm_read_byte_far(s1) == pgm_read_byte_far(s2);
      s1 = (const U8GFChar *)((char *)s1 + 1),
      s2 = (const U8GFChar *)((char *)s2 + 1)) {
      if('\0' == pgm_read_byte_far(s1)) {
          return 0;
      }
  }

  return (((unsigned char)pgm_read_byte_far(s1) <
           (unsigned char)pgm_read_byte_far(s2)) ? -1 : +1);
}

void _u8gettextInitialize(
    const U8GettextContext * context,
    const char *language,
    int16_t internalBufferSize)
{
  sContext = context;
  if (internalBufferSize > 0) {
    sBuffer = (char * )malloc(internalBufferSize);
    sBufferSize = internalBufferSize;
  }
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

const U8GFChar *u8gettext(const U8GFChar *str)
{
  uint16_t i = 0;
  U8GettextTranslation translation;

  if (!sLanguageInstance) {
    return str;
  }

  for(i = 0; i < *sLanguageInstance->translationCount; ++i) {
    memcpy_P(&translation,
             sLanguageInstance->translations + i,
             sizeof(U8GettextTranslation));

    // Slowest way to find translation
    if(0 == flashStrcmp(str, translation.msgId)) {
      return translation.msgStr;
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

static char utf8ToU8GlibFontEncodingF(const U8GFChar *str)
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


const char *u8gettextUN(const char *str)
{
  char * position = NULL;

  sBuffer[0] = '\0';

  if (!sContext) {
    return sBuffer;
  }

  position = sBuffer;

  while(*str) {
    *position = utf8ToU8GlibFontEncoding(str);
    str = utf8FindNextChar(str);
    ++ position;
    if(PTR_OFFSET_BETWEEN(sBuffer, position) >= sBufferSize) {
      break;
    }
  }

  *position = '\0';

  return sBuffer;
}

const char *u8gettextUNF(const U8GFChar *str)
{
  char * position = NULL;

  sBuffer[0] = '\0';

  if (!sContext) {
    return sBuffer;
  }

  position = sBuffer;

  while(pgm_read_byte_far(str)) {
    *position = utf8ToU8GlibFontEncodingF(str);
    str = utf8FindNextChar(str);
    ++ position;
    if(PTR_OFFSET_BETWEEN(sBuffer, position) >= sBufferSize) {
      break;
    }
  }

  *position = '\0';

  return sBuffer;
}
