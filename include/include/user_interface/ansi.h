#pragma once

/**
 * Portable header containing some basic ANSI Escape sequences for applying
 * colors and other graphic effects to text on terminal
*/
namespace ANSI {

    /**
     * ANSI Escape sequences for foreground colors
    */
    namespace Foreground {
        inline constexpr auto BLACK = "\x1b[30m";
        inline constexpr auto RED = "\x1b[31m";
        inline constexpr auto GREEN = "\x1b[32m";
        inline constexpr auto YELLOW = "\x1b[33m";
        inline constexpr auto BLUE = "\x1b[34m";
        inline constexpr auto MAGENTA = "\x1b[35m";
        inline constexpr auto CYAN = "\x1b[36m";
        inline constexpr auto WHITE = "\x1b[37m";
        inline constexpr auto RESET = "\x1b[39m";

        /**
         * Brighter variants of colors.
         * May not work on some terminals
        */
        namespace Bright {
            inline constexpr auto BLACK = "\x1b[90m";
            inline constexpr auto RED = "\x1b[91m";
            inline constexpr auto GREEN = "\x1b[92m";
            inline constexpr auto YELLOW = "\x1b[93m";
            inline constexpr auto BLUE = "\x1b[94m";
            inline constexpr auto MAGENTA = "\x1b[95m";
            inline constexpr auto CYAN = "\x1b[96m";
            inline constexpr auto WHITE = "\x1b[97m";
        }
    }

    /**
     * ANSI Escape sequences for background colors
    */
    namespace Background {
        inline constexpr auto BLACK = "\x1b[40m";
        inline constexpr auto RED = "\x1b[41m";
        inline constexpr auto GREEN = "\x1b[42m";
        inline constexpr auto YELLOW = "\x1b[43m";
        inline constexpr auto BLUE = "\x1b[44m";
        inline constexpr auto MAGENTA = "\x1b[45m";
        inline constexpr auto CYAN = "\x1b[46m";
        inline constexpr auto WHITE = "\x1b[47m";
        inline constexpr auto RESET = "\x1b[49m";

        /**
         * Brighter variants of colors.
         * May not work on some terminals
        */
        namespace Bright {
            inline constexpr auto BLACK = "\x1b[100m";
            inline constexpr auto RED = "\x1b[101m";
            inline constexpr auto GREEN = "\x1b[102m";
            inline constexpr auto YELLOW = "\x1b[103m";
            inline constexpr auto BLUE = "\x1b[104m";
            inline constexpr auto MAGENTA = "\x1b[105m";
            inline constexpr auto CYAN = "\x1b[106m";
            inline constexpr auto WHITE = "\x1b[107m";
        }
    }

    /**
     * ANSI Escape sequences for various text effects
    */
    namespace Effects {
        inline constexpr auto BOLD = "\x1b[1m";
        inline constexpr auto BOLD_RESET = "\x1b[22m";
        inline constexpr auto DIM = "\x1b[2m";
        inline constexpr auto DIM_RESET = "\x1b[22m";
        inline constexpr auto ITALIC = "\x1b[3m";
        inline constexpr auto ITALIC_RESET = "\x1b[23m";
        inline constexpr auto UNDERLINE = "\x1b[4m";
        inline constexpr auto UNDERLINE_RESET = "\x1b[24m";
        inline constexpr auto BLINK = "\x1b[5m";
        inline constexpr auto BLINK_RESET = "\x1b[25m";
        inline constexpr auto INVERSE = "\x1b[7m";
        inline constexpr auto INVERSE_RESET = "\x1b[27m";
        inline constexpr auto HIDE = "\x1b[8m";
        inline constexpr auto HIDE_RESET = "\x1b[28m";
        inline constexpr auto STRIKETHROUGH = "\x1b[9m";
        inline constexpr auto STRIKETHROUGH_RESET = "\x1b[29m";
    }

    /**
     * Resets both colors and the text effects
    */
    inline constexpr auto RESET = "\x1b[0m";
}
