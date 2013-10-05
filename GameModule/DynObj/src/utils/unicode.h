
#ifndef UNICODE_H
#define UNICODE_H

/*

// The documentation below comes from the fd32 project.
// The implementation is separate though.

A UTF-8 character is converted to a wide character (UTF-32 or UCS-4)
using the following rules (binary numbers):
\code
UTF-32                     - UTF-8
00000000 00000000 0aaaaaaa - 0aaaaaaa
00000000 00000bbb bbaaaaaa - 110bbbbb 10aaaaaa
00000000 ccccbbbb bbaaaaaa - 1110cccc 10bbbbbb 10aaaaaa
000dddcc ccccbbbb bbaaaaaa - 11110ddd 10cccccc 10bbbbbb 10aaaaaa
\endcode

A UTF-16 character is converted to a wide character (UTF-32 or UCS-4)
using the following rules (binary numbers):
\code
UTF-32                     - UTF-16
00000000 aaaaaaaa aaaaaaaa <-> aaaaaaaa aaaaaaaa
000bbbbb aaaaaaaa aaaaaaaa <-> 110110cc ccaaaaaa  110111aa aaaaaaaa
\endcode
where \c cccc = \c bbbbb - 1.

*/

// \brief  Returns the length in bytes of a UTF8 encoded character
// \param  lead_byte The first byte in the UTF8 encoded Unicode character
// \return Length 
int unicode_utf8_len_(char lead_byte);

// \brief One UTF8 encoded character is read and converted to an int
// \param s      The source buffer
// \param result Pointer to where to put result
// \return The number of bytes used to decode one Unicode character
int unicode_utf8_to_wchar_(int *result, const char *s);


// \brief One unicode character is converted to UTF8. 
// \param s     The destination buffer
// \param wc    Unicode character to be converted
// \param wsize Size of output buffer
// \return The length (in char) of the UTF8 encoded character
int unicode_wchar_to_utf8_(char *s, int wc, int size);

// \brief Convert to common case for string comparison
// \param wc    Unicode character to be converted
int unicode_simple_fold_( int wc );

#endif // UNICODE_H

