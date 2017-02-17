#ifndef __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856
#define __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856

#include <stddef.h>
#include <inttypes.h>
#include <U8glib.h>

#define U8GETTEXT_MAKE_VERSION(major, minor, micro) \
  ((((major) & 0xFF) << 24) | (((minor) & 0xFF) << 16) | ((micro) & 0xFFFF))

#define U8GETTEXT_VERSION U8GETTEXT_MAKE_VERSION(0, 0, 1)

typedef class __FlashStringHelper U8GFChar;

struct U8GettextCharMapping
{
  uint32_t utf32Char;
  uint16_t u8gChar;
};

struct U8GettextTranslation
{
  const U8GFChar * msgId;
  const U8GFChar * msgStr;
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
  /* This data be stored in Flash, so do not use normal way access it ! */
  const U8GettextCharMapping *charMappings;
  const size_t *charMappingCount;
};

extern const U8GettextContext __gU8GettextContext;

void _u8gettextInitialize(
    const U8GettextContext * context,
    const char *language,
    int16_t internalBufferSize=256);
#define u8gettextInitialize(language, internalBufferSize) _u8gettextInitialize(\
    &__gU8GettextContext, \
    (language), \
    (internalBufferSize))


/**
 * Set current U8Gettext language
 *
 * @param language You must pass a constant string or dynamic allocated
 * string throughout the whole U8Gettext life!
 *
 * @return Old language string
 */
const char *u8gettextSetLanguage(const char *language);

/**
 * Get current language
 *
 * @return Return current language or return empty string if you have
 * not set language before.
 */
const char *u8gettextGetLanguage();
const U8GFChar *u8gettext(const U8GFChar *str);

/**
 * Convert message to encodings that U8Glib display method needs. It
 * won't translate the input str before convert.
 *
 * @param str
 * @return Return string is a temporary pointer with about 1KB buffer!
 * @note Don't use the return pointer cross two U8GettextU*()!
 */
const char *u8gettextUN(const U8GFChar *str);

#endif // __U8GETTEXT_H_INCLUDED_4AD09038_006D_A018_36AB_D116C344B856

