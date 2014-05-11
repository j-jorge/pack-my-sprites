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
 * \brief Implementation of the sdc::image_generator class.
 * \author Julien Jorge
 */
#include "image_generator.hpp"

#include <fstream>
#include <iostream>

#include <limits>

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two sprites by decreasing order of their areas.
 * \param a The left operand.
 * \param b The right operand.
 */
bool sdc::image_generator::sprite_area_comp::operator()
  ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const
{
  return a.result_box.area() > b.result_box.area();
} // image_generator::sprite_area_comp::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two sprites by decreasing order of their heights.
 * \param a The left operand.
 * \param b The right operand.
 */
bool sdc::image_generator::sprite_height_comp::operator()
  ( const spritedesc::sprite& a, const spritedesc::sprite& b ) const
{
  return a.result_box.height > b.result_box.height;
} // image_generator::sprite_height_comp::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param gimp The interface to use to execute the GIMP scripts.
 */
sdc::image_generator::image_generator( gimp_interface gimp )
  : m_gimp( gimp )
{

} // image_generator::image_generator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generates the images of a given file.
 * \param source The file from which the spritedesc was generated.
 * \param desc The spritedesc to process.
 */
void sdc::image_generator::generate
( std::string source, spritedesc_collection desc ) const
{
  const working_directory dir( source );
  typedef spritedesc_collection::spritedesc_list_type::iterator iterator_type;

  for ( iterator_type it=desc.sprite_sheet.begin();
        it!=desc.sprite_sheet.end(); ++it )
    {
      claw::logger << claw::log_verbose
                   << "Generating sprite sheet '" << it->output_name << "'"
                   << std::endl;

      generate_output( dir, desc.xcf, *it );
    }
} // image_generator::generate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generate a sprite sheets.
 * \param dir The directories where the files are searched and written.
 * \param xcf The details of the images to use in the generation.
 * \param desc The sprite sheet to generate.
 */
void sdc::image_generator::generate_output
( working_directory dir, xcf_map xcf, spritedesc desc ) const
{
  std::ostringstream oss;
  generate_scm( oss, dir, xcf, desc );

  gimp_interface::path_list_type includes;
  includes.push_back( "common.scm" );

  m_gimp.run( oss.str(), includes );
} // image_generator::generate_output()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generate the Scheme script that builds the sprites of a given
 *        spritedesc.
 * \param os The stream in which the script is generated.
 * \param dir The directories where the files are searched and written.
 * \param xcf The details of the images to use in the generation.
 * \param desc The sprites to generate.
 */
void sdc::image_generator::generate_scm
( std::ostream& os, working_directory dir, xcf_map xcf, spritedesc desc ) const
{
  os << "(let ( ";

  for ( spritedesc::id_to_file_map::const_iterator it = desc.xcf.begin();
        it != desc.xcf.end(); ++it )
    {
      const std::string xcf_path( dir.get_xcf_path( it->second ) );

      os << "(" << make_image_varname( it->first )
         << " (car (gimp-file-load 1 \"" << xcf_path << "\" \"" << xcf_path
         << "\" )))\n";
    }

  os << "(" << make_image_varname(desc.output_name)
     << " (new-image " << desc.width << ' ' << desc.height << "))\n";

  os << ")\n";

  for ( spritedesc::const_sprite_iterator it = desc.sprite_begin();
        it != desc.sprite_end(); ++it )
    generate_scm
      ( os, xcf.get_info( desc.xcf[it->xcf_id] ), *it, desc.output_name );

  os << "(save-frames \""
     << dir.get_output_image_path(desc.output_name) << "\" "
     << make_image_varname(desc.output_name) << ")\n";

  os << ")\n";
} // image_generator::generate_scm()

/*----------------------------------------------------------------------------*/
/**
 * \brief Generate the Scheme command that builds the given sprite.
 * \param os The stream in which the script is generated.
 * \param xcf The details of the image to use in the generation.
 * \param s The sprite to generate.
 * \param target_id The identifier of the image receiving the sprite in the
 *         script.
 */
void sdc::image_generator::generate_scm
( std::ostream& os, const xcf_info& xcf, const spritedesc::sprite& s,
  const std::string& target_id ) const
{
  // starting with version 3 of the format, the XCF files have layer groups.
  if ( xcf.version >= 3 )
    {
      os << "(create-layer-crop-with-groups " << make_image_varname(s.xcf_id)
         << " '(";

      for ( std::list<layer_info>::const_iterator it=s.layers.begin();
            it != s.layers.end(); ++it )
        os << '"' << xcf.get_layer_name( it->index ) << "\" ";
    }
  else
    {
      os << "(create-layer-crop " << make_image_varname(s.xcf_id) << " '(";

      for ( std::list<layer_info>::const_iterator it=s.layers.begin();
            it != s.layers.end(); ++it )
        os << it->index << ' ';
    }

  os << ") " << s.source_box.position.x << ' ' << s.source_box.position.y << ' '
     << s.source_box.width << ' ' << s.source_box.height << ' '
     << s.result_box.position.x << ' ' << s.result_box.position.y << ' '
     << s.result_box.width << ' ' << s.result_box.height;

  os << ' ' << make_image_varname(target_id) << " '(";

  if ( xcf.version >= 3 )
    {
      for ( std::list<layer_info>::const_iterator it=s.mask.begin();
            it != s.mask.end(); ++it )
        os << '"' << xcf.get_layer_name( it->index ) << "\" ";
    }
  else
    {
      for ( std::list<layer_info>::const_iterator it=s.mask.begin();
            it != s.mask.end(); ++it )
        os << it->index << ' ';
    }

  os << "))\n";
} // image_generator::generate_scm()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the variable representing the xcf file identified with
 *        a given id.
 * \param id The identifier of the xcf file.
 */
std::string
sdc::image_generator::make_image_varname( const std::string& id ) const
{
  return id + "_image";
} // image_generator::make_image_varname()
