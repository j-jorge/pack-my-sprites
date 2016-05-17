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

#include "pms/app/packer_options.hpp"
#include "pms/layout/sprite_sheet.hpp"

namespace pms
{
  namespace app
  {
    class packer
    {
    public:
      explicit packer( const packer_options& options );

      bool run( const std::vector< std::string >& files ) const;
      bool run( const std::string& file ) const;
      bool run
      ( const std::string& source_file_path, layout::sprite_sheet sheet ) const;

    private:
      boost::optional< layout::sprite_sheet >
      generate_sprite_sheet( const std::string& file_name ) const;

      layout::sprite_sheet generate_sprite_sheet
      ( const std::string& directory, std::istream& n ) const;

    public:
      const packer_options m_options;
      gimp::system_interface m_gimp;
    };
  };
}
