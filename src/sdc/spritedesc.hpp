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
 * \brief The description of a sprite sheet.
 * \author Julien Jorge
 */
#ifndef __SDC_SPRITEDESC_HPP__
#define __SDC_SPRITEDESC_HPP__

#include "layer_info.hpp"
#include "placement_order.hpp"

#include <list>
#include <map>

namespace sdc
{
  /**
   * \brief The content of a a .spritedesc file.
   * \author Julien Jorge
   */
  class spritedesc
  {
  public:
    /**
     * \brief The description of a sprite.
     */
    struct sprite
    {
      /** \brief The identifier of the xcf file from which the sprite is
          made. */
      std::string xcf_id;

      /** \brief The name of the sprite. */
      std::string name;

      /** \brief The bounding box of the original sprite. */
      claw::math::rectangle<int> source_box;

      /** \brief The bounding box of the resulting sprite. */
      claw::math::rectangle<int> result_box;

      /** \brief The layers to use to produce the sprite. */
      std::list<layer_info> layers;

      /** \brief The layers to use for the mask. */
      std::list<layer_info> mask;

    }; // struct sprite

    /** \brief The type of the map providing the name of the xcf file associated
        with a given id. */
    typedef std::map<std::string, std::string> id_to_file_map;

  private:
    /** \brief The type of the list containing the sprites. */
    typedef std::list<sprite> sprite_list;

  public:
    /** \brief The type of the iterators on the sprites. */
    typedef sprite_list::iterator sprite_iterator;

    /** \brief The type of the constant iterators on the sprites. */
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

  public:
    /** \brief The name of the file in which the sprites are saved. */
    std::string output_name;

    /** \brief The width of the resulting image. */
    std::size_t width;

    /** \brief The height of the resulting image. */
    std::size_t height;

    /** \brief The margin between the sprites. */
    std::size_t margin;

    /** \brief The order in which the sprites are placed. */
    placement_order order;

    /** \brief The xcf files to use. The key is the id in the .spritedesc file,
        the value is the name of the file. */
    id_to_file_map xcf;

  private:
    /** \brief The sprites described in the .spritedesc file. */
    sprite_list sprites;

  }; // struct spritedesc
} // namespace sdc

#include "impl/spritedesc.tpp"

#endif // __SDC_SPRITEDESC_HPP__
