/*
  This file is part of Pack My Sprites.

  Pack My Sprites is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published by
  the Free Software Foundation, either version 3 of the License.

  Pack My Sprites is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.
  
  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#define PMS_TO_STR_BIS(v) # v
#define PMS_TO_STR(v) PMS_TO_STR_BIS(v)

#define PMS_MAJOR_VERSION 1
#define PMS_MINOR_VERSION 2
#define PMS_RELEASE_NUMBER 0
#define PMS_VERSION_STRING "Pack My Sprites, "                          \
  PMS_TO_STR(PMS_MAJOR_VERSION) "." PMS_TO_STR(PMS_MINOR_VERSION)       \
  "." PMS_TO_STR(PMS_RELEASE_NUMBER)
