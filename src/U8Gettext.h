#ifndef __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856
#define __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856

#include <stddef.h>
#include <inttypes.h>

struct U8GettextCharMapping
{
  uint32_t utf32Char;
  uint16_t u8gChar;
};

struct U8GettextTranslation
{
  const char * msgId;
  const char * msgStr;
};

struct U8GettextLanguage
{
  const char * language;
  const U8GettextTranslation * translations;
  const size_t * translationCount;
};

extern const size_t __gU8GettextLanguagesLength;
extern const U8GettextLanguage __gU8GettextLanguages[];

void _U8GettextInitialize(const U8GettextLanguage * languages, const size_t languageCount, const char *language);
#define U8GettextInitialize(language) _U8GettextInitialize(__gU8GettextLanguages, __gU8GettextLanguagesLength, (language))

/**
 * Set current U8Gettext language
 *
 * @param language You must pass a constant string or dynamic allocated
 * string throughout the whole U8Gettext life!
 *
 * @return Old language string
 */
const char *U8GettextSetLanguage(const char *language);

/**
 * Get current language
 *
 * @return Return current language or return empty string if you have
 * not set language before.
 */
const char *U8GettextGetLanguage();
const char *U8Gettext(const char *str);

#endif // __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856

