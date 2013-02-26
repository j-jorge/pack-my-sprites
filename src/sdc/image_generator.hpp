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

    typedef std::list<std::string> path_list_type;

  public:
    image_generator
    ( bool generate_spritepos, path_list_type scheme_directory,
      std::string gimp_console_program );

    void run( file_to_spritedesc_map files );

  private:
    void process_spritedesc
    ( std::string source, spritedesc_collection desc ) const;

    void execute_gimp_scheme_process( std::string script ) const;

    void generate_output
    ( working_directory dir, xcf_map xcf, spritedesc desc ) const;
    void generate_spritepos( std::ostream& os, const spritedesc& desc ) const;

    std::string get_scheme_path( std::string filename ) const;
    
    void generate_scm
    ( std::ostream& os, working_directory dir, xcf_map xcf,
      spritedesc desc ) const;

    void generate_scm
    ( std::ostream& os, const xcf_info& xcf, const spritedesc::sprite& s,
      const std::string& target_id ) const;

    std::string make_image_varname( const std::string& id ) const;

    void set_sprite_position( spritedesc& desc ) const;
    bool find_place_for
    ( std::list<rectangle_type>& empty_places, spritedesc::sprite& s,
      std::size_t m ) const;

  private:
    /** \brief Tells to generate the spritepos file. */
    bool m_generate_spritepos;

    /** \brief The directories where the scheme scripts are searched. */
    path_list_type m_scheme_directory;

    /** \brief The path to gimp-console executable. */
    std::string m_gimp_console_program;

  }; // class image_generator
} // namespace sdc

#endif // __SDC_IMAGE_GENERATOR_HPP__
