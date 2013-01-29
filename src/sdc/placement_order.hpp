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
 * \brief The heuristic to use to place the sprites.
 * \author Julien Jorge
 */
#ifndef __SDC_PLACEMENT_ORDER_HPP__
#define __SDC_PLACEMENT_ORDER_HPP__

namespace sdc
{
  /**
   * \brief The heuristic to use to place the sprites.
   * \author Julien Jorge
   */
  enum placement_order
    {
      /** \brief The sprites with the largest area are placed first. */
      decreasing_area,
      
      /** \brief The tallest sprites are placed first. */
      decreasing_height,

      /** \brief The sprites are placed in the order they are defined. */
      declaration_order
      
    }; // enum placement_order

} // namespace sdc

#endif // __SDC_PLACEMENT_ORDER_HPP__
