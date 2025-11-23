#include "single_line_caret.h"

#include "carma.h"

SingleLineCaret moveSingleLineCaretColumn(SingleLineCaret caret, StringRange text, int column) {
    auto count = (int)text.count;
    if (column < 0) {
        column = 0;
    }
    if (column > count) {
        column = count;
    }
    caret.column = column;
    return caret;
}

SingleLineCaret moveSingleLineCaretLeft(SingleLineCaret caret, StringRange text) {
    return moveSingleLineCaretColumn(caret, text, caret.column - 1);
}

SingleLineCaret moveSingleLineCaretRight(SingleLineCaret caret, StringRange text) {
    return moveSingleLineCaretColumn(caret, text, caret.column + 1);
}

SingleLineCaret moveSingleLineCaretHome(SingleLineCaret caret, StringRange text) {
    return moveSingleLineCaretColumn(caret, text, 0);
}

SingleLineCaret moveSingleLineCaretEnd(SingleLineCaret caret, StringRange text) {
    return moveSingleLineCaretColumn(caret, text, text.count);
}

SingleLineCaret insertCharactersSingleLineCaret(SingleLineCaret caret, StringBuilder text, StringRange clipboard) {
    auto len = text.count;
    if ((size_t)caret.column > len) { // TODO: what is this condition for?
        return caret;
    }
    if (len + clipboard.count >= text.capacity) {
        return caret;
    }
    INSERT_RANGE(text, (size_t)caret.column, clipboard);
    text.data[text.count] = '\0';
    auto text_range = (StringRange){text.data, text.count};
    for (size_t i = 0; i < clipboard.count; ++i) {
        caret = moveSingleLineCaretRight(caret, text_range);
    }
    return caret;
}

SingleLineCaret insertCharacterSingleLineCaret(SingleLineCaret caret, StringBuilder text, char c) {
    auto len = text.count;
    if ((size_t)caret.column > len) { // TODO: what is this condition for?
        return caret;
    }
    if (len + 1 >= text.capacity) {
        return caret;
    }
    INSERT_INDEX(text, (size_t)caret.column, c);
    text.data[text.count] = '\0';
    return moveSingleLineCaretRight(caret, (StringRange){text.data, text.count});
}

SingleLineCaret deleteCharacterAfterSingleLineCaret(SingleLineCaret caret, StringRange text) {
    auto index = (size_t)caret.column;
    if (index < text.count) {
        ERASE_INDEX_ORDERED(text, index);
        text.data[text.count] = '\0';
    }
    return caret;
}

SingleLineCaret deleteCharacterBeforeSingleLineCaret(SingleLineCaret caret, StringRange text) {
    if (caret.column < 1) {
        return caret;
    }
    auto index = (size_t)caret.column - 1;
    ERASE_INDEX_ORDERED(text, index);
    text.data[text.count] = '\0';
    return moveSingleLineCaretLeft(caret, text);
}

SingleLineCaret deleteSelectedCharacters(SingleLineCaret caret, SingleLineCaret selection_anchor, StringRange text) {
    auto min = minSingleLineCaret(caret, selection_anchor);
    auto max = maxSingleLineCaret(caret, selection_anchor);
    auto selection_count = (size_t)(max.column - min.column);
    auto index = (size_t)min.column;
    ERASE_MANY_ORDERED(text, index, selection_count);
    text.data[text.count] = '\0';
    return min;
}

SingleLineCaret minSingleLineCaret(SingleLineCaret a, SingleLineCaret b) {
    return a.column < b.column ? a : b;
}

SingleLineCaret maxSingleLineCaret(SingleLineCaret a, SingleLineCaret b) {
    return a.column > b.column ? a : b;
}
