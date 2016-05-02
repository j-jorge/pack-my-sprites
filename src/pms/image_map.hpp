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
#ifndef __PMS_IMAGE_MAP_HPP__
#define __PMS_IMAGE_MAP_HPP__

#include "gimp_interface.hpp"
#include "image_info.hpp"

#include <string>
#include <map>

namespace pms
{
  class image_map
  {
  private:
    typedef std::map<std::string, image_info> name_to_info_type;

  public:
    image_map();
    image_map( const std::string& image_directory, const gimp_interface& gimp );

    void load( const std::string& name );

    bool has_info( const std::string& name ) const;
    const image_info& get_info( const std::string& name ) const;

    std::string to_string() const;

  private:
    bool load_with_internal_tool
    ( const std::string& name, const std::string& file_path );
    void load_with_gimp
    ( const std::string& name, const std::string& file_path );

    std::string execute_xcfinfo_process( const std::string& file_path ) const;

    void parse_xcf_info_header
    ( image_info& info, const std::string& header ) const;
    void parse_xcf_info_layer
    ( image_info& info, const std::string& layer ) const;

  private:
    name_to_info_type m_image_info;
    std::string m_image_directory;
    gimp_interface m_gimp;

  };

}

#endif
