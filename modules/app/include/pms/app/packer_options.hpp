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

#include <vector>
#include <string>

namespace pms
{
  namespace generators
  {
    enum class rotation_direction;
  }
    
  namespace app
  {
    enum class atlas_format;
    
    class packer_options
    {
    public:
      packer_options();

      atlas_format get_atlas_format() const;
      void set_atlas_format( atlas_format t );
      
      generators::rotation_direction get_rotation_direction() const;
      void disable_rotation();
      
      bool should_crop() const;
      void disable_crop();
      
      const std::vector< std::string >& get_scheme_directory() const;
      void add_scheme_directories( const std::vector< std::string >& dirs );
      
      const std::string& get_gimp_console_program() const;
      void set_gimp_console_program( const std::string& gimp );
      
    private:
      atlas_format m_atlas_format;
      bool m_allow_rotation;
      bool m_allow_crop;
      
      std::vector< std::string > m_scheme_directory;
      std::string m_gimp_console_program;
    };
  };
}
