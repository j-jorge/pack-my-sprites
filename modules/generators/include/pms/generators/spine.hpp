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
    
    class spine
    {
    public:
      void generate
      ( const std::string& spritedesc_file_path,
        const layout::atlas& atlas ) const;

    private:
      void generate_file
      ( const detail::working_directory& dir,
        const layout::atlas& atlas ) const;

      std::string get_page
      ( const std::string& base_name, const layout::atlas_page& desc ) const;
      std::string get_page_header
      ( const std::string& base_name, const layout::atlas_page& desc ) const;
      std::string
      get_sprite_entry( const layout::atlas_page::sprite& sprite ) const;
    };
  }
}
