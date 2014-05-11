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
#ifndef __SDC_SPRITE_SHEET_BUILDER_HPP__
#define __SDC_SPRITE_SHEET_BUILDER_HPP__

#include "spritedesc_collection.hpp"

namespace sdc
{
  class sprite_sheet_builder
  {
  private:
    class sprite_area_comp
    {
    public:
      bool operator()
      ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const;

    };

    class sprite_height_comp
    {
    public:
      bool operator()
      ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const;

    };

    typedef claw::math::rectangle<std::size_t> rectangle_type;

  public:
    spritedesc_collection
    build( spritedesc_collection sprite_description ) const;

  private:
    void set_sprite_position( spritedesc& desc ) const;
    bool find_place_for
    ( std::list<rectangle_type>& empty_places, spritedesc::sprite& s,
      std::size_t m ) const;

  };
}

#endif