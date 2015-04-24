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
#ifndef __PMS_SPRITEDESC_HPP__
#define __PMS_SPRITEDESC_HPP__

#include "layer_info.hpp"
#include "placement_order.hpp"

#include <list>
#include <map>

namespace pms
{
  class spritedesc
  {
  public:
    struct sprite
    {
      std::string xcf_id;
      std::string name;
      claw::math::rectangle<int> source_box;
      claw::math::rectangle<int> result_box;
      std::list<layer_info> layers;
      std::list<layer_info> mask;

    };

    typedef std::map<std::string, std::string> id_to_file_map;

  private:
    typedef std::list<sprite> sprite_list;

  public:
    typedef sprite_list::iterator sprite_iterator;
    typedef sprite_list::const_iterator const_sprite_iterator;

  public:
    const_sprite_iterator get_sprite_by_name( std::string n ) const;
    void add_sprite( sprite spr );

    const_sprite_iterator sprite_begin() const;
    const_sprite_iterator sprite_end() const;

    sprite_iterator sprite_begin();
    sprite_iterator sprite_end();

    void erase_sprite( sprite_iterator it );

    template<typename F>
    void sort_sprites( F f );

    std::string to_string() const;

  public:
    std::string output_name;
    std::size_t width;
    std::size_t height;
    std::size_t margin;
    placement_order order;
    id_to_file_map xcf;

  private:
    sprite_list sprites;

  };
}

#include "impl/spritedesc.tpp"

#endif
