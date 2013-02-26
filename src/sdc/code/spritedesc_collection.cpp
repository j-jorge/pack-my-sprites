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
 * \brief Implementation of the sdc::spritedesc_collection class.
 * \author Julien Jorge
 */
#include "spritedesc_collection.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a collection with a default xcf map.
 */
sdc::spritedesc_collection::spritedesc_collection()
{

} // spritedesc_collection::spritedesc_collection()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructs a collection that will use a given xcf_map.
 * \param m The xcf_map to copy in this instance.
 */
sdc::spritedesc_collection::spritedesc_collection( xcf_map m )
  : xcf( m )
{

} // spritedesc_collection::spritedesc_collection()
