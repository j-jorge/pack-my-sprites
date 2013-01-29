/*
  This file is part of Pack My Sprites.

  Pack My Sprites is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, version 3 of the License.

  Pack My Sprites is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Pack My Sprites.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * \file
 * \brief Version of the tool.
 * \author Julien Jorge
 */
#ifndef __SDC_VERSION_HPP__
#define __SDC_VERSION_HPP__

#define SDC_TO_STR_BIS(v) # v
#define SDC_TO_STR(v) SDC_TO_STR_BIS(v)

#define SDC_MAJOR_VERSION 1
#define SDC_MINOR_VERSION 0
#define SDC_RELEASE_NUMBER 0
#define SDC_VERSION_STRING "Pack My Sprites, "                          \
  SDC_TO_STR(SDC_MAJOR_VERSION) "." SDC_TO_STR(SDC_MINOR_VERSION)       \
  "." SDC_TO_STR(SDC_RELEASE_NUMBER)

#endif // __SDC_VERSION_HPP__
