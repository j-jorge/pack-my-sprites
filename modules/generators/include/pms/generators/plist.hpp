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

#include "pms/layout/atlas.hpp"

namespace pms
{
  namespace generators
  {
    namespace detail
    {
      class working_directory;
    }
    
    class plist
    {
    public:
      void generate
      ( const std::string& spritedesc_file_path,
        const layout::atlas& atlas ) const;

    private:
      void generate_plist
      ( const detail::working_directory& dir, std::size_t index,
        const layout::atlas& atlas ) const;
      void generate_plist
      ( std::ostream& os, const std::string& base_name,
        const layout::description& desc ) const;

      std::string get_plist_header() const;
      std::string get_plist_body
      ( const std::string& base_name, const layout::description& desc ) const;
      std::string get_frames_entry( const layout::description& desc ) const;
      std::string
      get_sprite_entry( const layout::description::sprite& sprite ) const;
      std::string get_metadata_entry
      ( const std::string& base_name, const layout::description& desc ) const;
    };
  }
}
