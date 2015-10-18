#ifndef __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856
#define __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856

#include <stddef.h>
#include <inttypes.h>
#include <U8glib.h>

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

struct U8GettextContext
{
  const U8GettextLanguage *languages;
  const size_t *languageCount;
  const u8g_fntpgm_uint8_t *font;
  const size_t *fontEncodingCount;
  const U8GettextCharMapping *charMappings;
  const size_t *charMappingCount;
};

extern const U8GettextContext __gU8GettextContext;

void _U8GettextInitialize(
    const U8GettextContext * context,
    const char *language);
#define U8GettextInitialize(language) _U8GettextInitialize(\
    &__gU8GettextContext, \
    (language))


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

/**
 * Get translated message only for U8Glib display method !
 *
 * @param str
 * @return Return string is a temporary pointer with about 1KB buffer!
 * @note Don't use the return pointer cross two U8GettextU()!
 */
const char *U8GettextU(const char *str);

#endif // __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856

