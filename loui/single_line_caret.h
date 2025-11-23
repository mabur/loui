#pragma once

#include <stddef.h>

#include "string.h"

typedef struct SingleLineCaret {
    int column;
} SingleLineCaret;

// Move SingleLineCaret:
SingleLineCaret moveSingleLineCaretColumn(SingleLineCaret caret, StringRange text, int column);
SingleLineCaret moveSingleLineCaretLeft(SingleLineCaret caret, StringRange text);
SingleLineCaret moveSingleLineCaretRight(SingleLineCaret caret, StringRange text);
SingleLineCaret moveSingleLineCaretHome(SingleLineCaret caret, StringRange text);
SingleLineCaret moveSingleLineCaretEnd(SingleLineCaret caret, StringRange text);

// Update text for SingleLineCaret:
SingleLineCaret insertCharactersSingleLineCaret(SingleLineCaret caret, StringBuilder text, StringRange clipboard);
SingleLineCaret insertCharacterSingleLineCaret(SingleLineCaret caret, StringBuilder text, char c);
SingleLineCaret deleteCharacterAfterSingleLineCaret(SingleLineCaret caret, StringRange text);
SingleLineCaret deleteCharacterBeforeSingleLineCaret(SingleLineCaret caret, StringRange text);
SingleLineCaret deleteSelectedCharacters(SingleLineCaret caret, SingleLineCaret selection_anchor, StringRange text);

SingleLineCaret minSingleLineCaret(SingleLineCaret a, SingleLineCaret b);
SingleLineCaret maxSingleLineCaret(SingleLineCaret a, SingleLineCaret b);
