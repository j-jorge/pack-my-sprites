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
 * \brief The image generator generates the images described by some
 *        spritedesc files.
 * \author Julien Jorge
 */
#ifndef __SDC_IMAGE_GENERATOR_HPP__
#define __SDC_IMAGE_GENERATOR_HPP__

#include "gimp_interface.hpp"
#include "spritedesc_collection.hpp"
#include "working_directory.hpp"

namespace sdc
{
  /**
   * \brief The image generator generates the images described by some
   *        spritedesc files.
   * \author Julien Jorge
   */
  class image_generator
  {
  private:
    /**
     * \brief This function object is used to compare the sprites by decreasing
     *        order of their areas.
     */
    class sprite_area_comp
    {
    public:
      bool operator()
      ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const;

    }; // class sprite_area_comp

    /**
     * \brief This function object is used to compare the sprites by decreasing
     *        order of their heights.
     */
    class sprite_height_comp
    {
    public:
      bool operator()
      ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const;

    }; // class sprite_height_comp

    /** \brief The structure that describes the rectangles when placing the
        sprites. */
    typedef claw::math::rectangle<std::size_t> rectangle_type;

  public:
    typedef std::map<std::string, spritedesc_collection> file_to_spritedesc_map;

  public:
    explicit image_generator( gimp_interface gimp );

    void generate( std::string source, spritedesc_collection desc ) const;

  private:
    void generate_output
    ( working_directory dir, xcf_map xcf, spritedesc desc ) const;

    void generate_scm
    ( std::ostream& os, working_directory dir, xcf_map xcf,
      spritedesc desc ) const;

    void generate_scm
    ( std::ostream& os, const xcf_info& xcf, const spritedesc::sprite& s,
      const std::string& target_id ) const;

    std::string make_image_varname( const std::string& id ) const;

  private:
    /** \brief The interface to use to execute the GIMP scripts. */
    gimp_interface m_gimp;

  }; // class image_generator
} // namespace sdc

#endif // __SDC_IMAGE_GENERATOR_HPP__
