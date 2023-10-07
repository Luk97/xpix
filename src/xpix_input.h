#ifndef XPIX_INPUT_H_
#define XPIX_INPUT_H_

typedef enum {

    // special
    XPIX_KEY_NULL        = 0,       // used for no key
    XPIX_KEY_ESCAPE      = 27,      // Escape
    XPIX_KEY_SPACE       = 32,      // Spacebar

    // numbers
    XPIX_KEY_0           = 48,      // 0
    XPIX_KEY_1           = 49,      // 1
    XPIX_KEY_2           = 50,      // 2
    XPIX_KEY_3           = 51,      // 3
    XPIX_KEY_4           = 52,      // 4
    XPIX_KEY_5           = 53,      // 5
    XPIX_KEY_6           = 54,      // 6
    XPIX_KEY_7           = 55,      // 7
    XPIX_KEY_8           = 56,      // 8
    XPIX_KEY_9           = 57,      // 9

    // letters
    XPIX_KEY_A           = 65,      // A | a
    XPIX_KEY_B           = 66,      // B | b
    XPIX_KEY_C           = 67,      // C | c
    XPIX_KEY_D           = 68,      // D | d
    XPIX_KEY_E           = 69,      // E | e
    XPIX_KEY_F           = 70,      // F | f
    XPIX_KEY_G           = 71,      // G | g
    XPIX_KEY_H           = 72,      // H | h
    XPIX_KEY_I           = 73,      // I | i
    XPIX_KEY_J           = 74,      // J | j
    XPIX_KEY_K           = 75,      // K | k
    XPIX_KEY_L           = 76,      // L | l
    XPIX_KEY_M           = 77,      // M | m
    XPIX_KEY_N           = 78,      // N | n
    XPIX_KEY_O           = 79,      // O | o
    XPIX_KEY_P           = 80,      // P | p
    XPIX_KEY_Q           = 81,      // Q | q
    XPIX_KEY_R           = 82,      // R | r
    XPIX_KEY_S           = 83,      // S | s
    XPIX_KEY_T           = 84,      // T | t
    XPIX_KEY_U           = 85,      // U | u
    XPIX_KEY_V           = 86,      // V | v
    XPIX_KEY_W           = 87,      // W | w
    XPIX_KEY_X           = 88,      // X | x
    XPIX_KEY_Y           = 89,      // Y | y
    XPIX_KEY_Z           = 90,      // Z | z

} XPix_Key;

typedef enum {
    XPIX_MOUSE_NULL       = 0,       // used for no mouse button
    XPIX_MOUSE_LEFT       = 1,       // left mouse button
    XPIX_MOUSE_RIGHT      = 2,       // right mouse button
} XPix_MouseButton;

#endif // XPIX_INPUT_H_