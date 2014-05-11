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
#ifndef __SDC_PLIST_GENERATOR_HPP__
#define __SDC_PLIST_GENERATOR_HPP__

#include "spritedesc_collection.hpp"
#include "working_directory.hpp"

namespace sdc
{
  class plist_generator
  {
  public:
    void generate
    ( std::string spritedesc_file_path, spritedesc_collection desc ) const;

  private:
    void generate_plist( working_directory dir, spritedesc desc ) const;
    void generate_plist( std::ostream& os, spritedesc desc ) const;

    std::string get_plist_header() const;
    std::string get_plist_body( spritedesc desc ) const;
    std::string get_frames_entry( spritedesc desc ) const;
    std::string get_sprite_entry( spritedesc::sprite sprite ) const;
    std::string get_metadata_entry( spritedesc desc ) const;

  };
}

#endif
