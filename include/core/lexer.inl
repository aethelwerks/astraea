#pragma once

inline bool
is_character_skipable(char ch)
{
    if (ch == ' ' ||  // Space
        ch == 9 ||    // Horizontal Tab
        ch == 10 ||   // New Line
        ch == 11 ||   // Vertical Tab
        ch == 13      // Carriage Return
    ) {
        return true;
    }

    return false;
}

inline bool
is_character_number(char ch)
{
    if (ch >= '0' && ch <= '9') {
        return true;
    }

    return false;
}

inline bool
is_character_octal(char ch)
{
    if (ch >= '0' && ch <= '7') {
        return true;
    }

    return false;
}

inline bool
is_character_hex(char ch)
{
    if ((ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F') ||
        is_character_number(ch)) {
        return true;
    }

    return false;
}

inline bool
is_character_reserved(char ch)
{
    if (ch == ' ' || ch == '.' || ch == ',' || ch == ':' ||
        ch == ';' || ch == '-' || ch == '+' || ch == '/' ||
        ch == '*' || ch == '{' || ch == '}' || ch == '(' ||
        ch == ')' || ch == '[' || ch == ']' || ch == '<' ||
        ch == '>' || ch == '^' || ch == '&' || ch == '!' ||
        ch == '=' || ch == '"' || ch == '#' ||
        ch == 39  // [ ' ] APOSTROPHE
    ) {
        return true;
    }

    return false;
}

inline bool
is_character_valid_identifier(char ch)
{
    if (is_character_reserved(ch) || is_character_skipable(ch) || ch == '\0') {
        return false;
    }

    return true;
}
