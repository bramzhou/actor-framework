/******************************************************************************
 *                       ____    _    _____                                   *
 *                      / ___|  / \  |  ___|    C++                           *
 *                     | |     / _ \ | |_       Actor                         *
 *                     | |___ / ___ \|  _|      Framework                     *
 *                      \____/_/   \_|_|                                      *
 *                                                                            *
 * Copyright (C) 2011 - 2014                                                  *
 * Dominik Charousset <dominik.charousset (at) haw-hamburg.de>                *
 *                                                                            *
 * Distributed under the terms and conditions of the BSD 3-Clause License or  *
 * (at your option) under the terms and conditions of the Boost Software      *
 * License 1.0. See accompanying files LICENSE and LICENCE_ALTERNATIVE.       *
 *                                                                            *
 * If you did not receive a copy of the license files, see                    *
 * http://opensource.org/licenses/BSD-3-Clause and                            *
 * http://www.boost.org/LICENSE_1_0.txt.                                      *
 ******************************************************************************/

#ifndef CAF_DETAIL_ATOM_VAL_HPP
#define CAF_DETAIL_ATOM_VAL_HPP

namespace caf {
namespace detail {

namespace {

// encoding table from ASCII to 6bit encoding for atoms
//     ..0 ..1 ..2 ..3 ..4 ..5 ..6 ..7 ..8 ..9 ..A ..B ..C ..D ..E ..F 
// 0..   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
// 1..   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
// 2..   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
// 3..   1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 0,  0,  0,  0,  0,  0,
// 4..   0, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
// 5..  26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,  0,  0,  0,  0, 37,
// 6..   0, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
// 7..  53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63,  0,  0,  0,  0,  0

// decodes 6bit characters to ASCII
constexpr char decoding_table[] = " 0123456789"
                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ_"
                  "abcdefghijklmnopqrstuvwxyz";

} // namespace <anonymous>

constexpr uint64_t encode(char c) {
  return (c >= '0' && c <= '9') // 0..9 => 1..10
         ? c - '0' + 1
         : (c >= 'A' && c <= 'Z') // A..Z => 11..36
           ? c - 'A' + 11
           : (c == '_') // _ => 37
             ? 37
             : (c >= 'a' && c <= 'z') // a..z => 38..63
               ? c - 'a' + 38
               : 0; // everything else is marked invalid
}

constexpr uint64_t next_interim(uint64_t current, char c) {
  return (current << 6) | encode(c);
}

constexpr uint64_t atom_val(const char* cstr, uint64_t interim = 0) {
  return (*cstr == '\0') ?
         interim :
         atom_val(cstr + 1,
            next_interim(interim, *cstr));
}

} // namespace detail
} // namespace caf

#endif // CAF_DETAIL_ATOM_VAL_HPP
