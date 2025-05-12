/*
  This file is part of Pack My Sprites.

  Pack My Sprites is free software: you can redistribute it and/or modify
  it under the terms of the GNU Affero General Public License as published by
  the Free Software Foundation, either version 3 of the License.

  Pack My Sprites is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Affero General Public License for more details.

  You should have received a copy of the GNU Affero General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "pms/gimp/system_interface.hpp"
#include "pms/resources/image.hpp"

#include <boost/optional.hpp>

#include <string>
#include <unordered_map>

namespace pms
{
  namespace resources
  {
    class image_mapping
    {
    public:
      image_mapping();
      image_mapping
      ( const std::string& image_directory,
        const gimp::system_interface& gimp );
      image_mapping(image_mapping&&);
      ~image_mapping();

      void load( const std::string& name, bool crop );

      boost::optional< const image& > get_image
      ( const std::string& name ) const;

      std::string to_string() const;

    private:
      bool load_with_internal_tool
      ( const std::string& name, const std::string& file_path, bool crop );
      void load_with_gimp
      ( const std::string& name, const std::string& file_path );

      std::string execute_xcfinfo_process( const std::string& file_path ) const;

      void parse_xcf_info_header
      ( image& info, const std::string& header ) const;
      void parse_xcf_info_layer
      ( image& info, const std::string& layer_info ) const;

    private:
      std::unordered_map< std::string, image > m_images;
      std::string m_image_directory;
      gimp::system_interface m_gimp;
    };
  }
}
