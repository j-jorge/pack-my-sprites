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
 * \brief Several sprite sheets associated with the information about the xcf
 *        files used in the sprites.
 * \author Julien Jorge
 */
#ifndef __SDC_SPRITEDESC_COLLECTION_HPP__
#define __SDC_SPRITEDESC_COLLECTION_HPP__

#include "spritedesc.hpp"
#include "xcf_map.hpp"

#include <list>

namespace sdc
{
  /**
   * \brief Several sprite sheets associated with the information about the xcf
   *        files used in the sprites.
   * \author Julien Jorge
   */
  class spritedesc_collection
  {
  public:
    /** \brief The type of the collection used to store the sprite sheets. */
    typedef std::list<spritedesc> spritedesc_list_type;

  public:
    spritedesc_collection();
    explicit spritedesc_collection( xcf_map m );

  public:
    /** \brief The sprite sheets of the collection. */
    spritedesc_list_type sprite_sheet;

    /** \brief The details about the xcf files used in the sprites. */
    xcf_map xcf;

  }; // struct spritedesc_collection
} // namespace sdc

#endif // __SDC_SPRITEDESC_COLLECTION_HPP__
