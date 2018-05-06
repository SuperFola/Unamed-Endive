module RPG
  class Map
    def initialize(width, height)
      @tileset_id = 1
      @width = width
      @height = height
      @autoplay_bgm = false
      @bgm = RPG::AudioFile.new
      @autoplay_bgs = false
      @bgs = RPG::AudioFile.new("", 80)
      @encounter_list = []
      @encounter_step = 30
      @data = Table.new(width, height, 3)
      @events = {}
    end
    attr_accessor :tileset_id
    attr_accessor :width
    attr_accessor :height
    attr_accessor :autoplay_bgm
    attr_accessor :bgm
    attr_accessor :autoplay_bgs
    attr_accessor :bgs
    attr_accessor :encounter_list
    attr_accessor :encounter_step
    attr_accessor :data
    attr_accessor :events
  end
end
module RPG
  class AudioFile
    def initialize(name = "", volume = 100, pitch = 100)
      @name = name
      @volume = volume
      @pitch = pitch
    end
    attr_accessor :name
    attr_accessor :volume
    attr_accessor :pitch
  end
end
module RPG
  class Event
    def initialize(x, y)
      @id = 0
      @name = ""
      @x = x
      @y = y
      @pages = [RPG::Event::Page.new]
    end
    attr_accessor :id
    attr_accessor :name
    attr_accessor :x
    attr_accessor :y
    attr_accessor :pages
  end
end
module RPG
  class Event
    class Page
      def initialize
        @condition = RPG::Event::Page::Condition.new
        @graphic = RPG::Event::Page::Graphic.new
        @move_type = 0
        @move_speed = 3
        @move_frequency = 3
        @move_route = RPG::MoveRoute.new
        @walk_anime = true
        @step_anime = false
        @direction_fix = false
        @through = false
        @always_on_top = false
        @trigger = 0
        @list = [RPG::EventCommand.new]
      end
      attr_accessor :condition
      attr_accessor :graphic
      attr_accessor :move_type
      attr_accessor :move_speed
      attr_accessor :move_frequency
      attr_accessor :move_route
      attr_accessor :walk_anime
      attr_accessor :step_anime
      attr_accessor :direction_fix
      attr_accessor :through
      attr_accessor :always_on_top
      attr_accessor :trigger
      attr_accessor :list
    end
  end
end
module RPG
  class Event
    class Page
      class Condition
        def initialize
          @switch1_valid = false
          @switch2_valid = false
          @variable_valid = false
          @self_switch_valid = false
          @switch1_id = 1
          @switch2_id = 1
          @variable_id = 1
          @variable_value = 0
          @self_switch_ch = "A"
        end
        attr_accessor :switch1_valid
        attr_accessor :switch2_valid
        attr_accessor :variable_valid
        attr_accessor :self_switch_valid
        attr_accessor :switch1_id
        attr_accessor :switch2_id
        attr_accessor :variable_id
        attr_accessor :variable_value
        attr_accessor :self_switch_ch
      end
    end
  end
end
module RPG
  class Event
    class Page
      class Graphic
        def initialize
          @tile_id = 0
          @character_name = ""
          @character_hue = 0
          @direction = 2
          @pattern = 0
          @opacity = 255
          @blend_type = 0
        end
        attr_accessor :tile_id
        attr_accessor :character_name
        attr_accessor :character_hue
        attr_accessor :direction
        attr_accessor :pattern
        attr_accessor :opacity
        attr_accessor :blend_type
      end
    end
  end
end
module RPG
  class MoveRoute
    def initialize
      @repeat = true
      @skippable = false
      @list = [RPG::MoveCommand.new]
    end
    attr_accessor :repeat
    attr_accessor :skippable
    attr_accessor :list
  end
end

module RPG
  class MoveCommand
    def initialize(code = 0, parameters = [])
      @code = code
      @parameters = parameters
    end
    attr_accessor :code
    attr_accessor :parameters
  end
end
module RPG
  class EventCommand
    def initialize(code = 0, indent = 0, parameters = [])
      @code = code
      @indent = indent
      @parameters = parameters
    end
    attr_accessor :code
    attr_accessor :indent
    attr_accessor :parameters
  end
end

=begin 
class Table
  def initialize(xsize, ysize = 1, zsize = 1)
    @xsize = xsize
    @ysize = ysize
    @zsize = zsize
  end
  def [](x,y = 1,z = 1)
    @xsize = x
    @ysize = y
    @zsize = z
  end
  def xsize
    return @xsize
  end
  def ysize
    return @ysize
  end
  def zsize
    return @zsize
  end
  def self._load(data)
    Table.new(data)
  end
end
=end
class Table # utilisation de ce script : http://www.hbgames.org/forums/viewtopic.php?t=49838
  def initialize(x, y = 1, z = 1)
     @xsize, @ysize, @zsize = x, y, z
     @data = Array.new(x * y * z, 0)
  end
  def [](x, y = 0, z = 0)
     @data[x + y * @xsize + z * @xsize * @ysize]
  end
  def []=(*args)
     x = args[0]
     y = args.size > 2 ? args[1] :0
     z = args.size > 3 ? args[2] :0
     v = args.pop
     @data[x + y * @xsize + z * @xsize * @ysize] = v
  end
  def _dump(d = 0)
     s = [3].pack('L')
     s += [@xsize].pack('L') + [@ysize].pack('L') + [@zsize].pack('L')
     s += [@xsize * @ysize * @zsize].pack('L')
     for z in 0...@zsize
        for y in 0...@ysize
           for x in 0...@xsize
              s += [@data[x + y * @xsize + z * @xsize * @ysize]].pack('S')
           end
        end
     end
     s
  end
  def self._load(s)
     size = s[0, 4].unpack('L')[0]
     nx = s[4, 4].unpack('L')[0]
     ny = s[8, 4].unpack('L')[0]
     nz = s[12, 4].unpack('L')[0]
     data = []
     pointer = 20
     loop do
        data.push(*s[pointer, 2].unpack('S'))
        pointer += 2
        break if pointer > s.size - 1
     end
     t = Table.new(nx, ny, nz)
     n = 0
     for z in 0...nz
        for y in 0...ny
           for x in 0...nx
              t[x, y, z] = data[n]
              n += 1
           end
        end
     end
     t
  end
  attr_reader(:xsize, :ysize, :zsize, :data)
end
#==============================================================================
# ** Game_Map
#------------------------------------------------------------------------------
#  This class handles the map. It includes scrolling and passable determining
#  functions. Refer to "$game_map" for the instance of this class.
#==============================================================================

class Game_Map
  #--------------------------------------------------------------------------
  # * Public Instance Variables
  #--------------------------------------------------------------------------
  attr_accessor :tileset_name             # tileset file name
  attr_accessor :map                      # map file 
  attr_accessor :autotile_names           # autotile file name
  attr_accessor :panorama_name            # panorama file name
  attr_accessor :panorama_hue             # panorama hue
  attr_accessor :fog_name                 # fog file name
  attr_accessor :fog_hue                  # fog hue
  attr_accessor :fog_opacity              # fog opacity level
  attr_accessor :fog_blend_type           # fog blending method
  attr_accessor :fog_zoom                 # fog zoom rate
  attr_accessor :fog_sx                   # fog sx
  attr_accessor :fog_sy                   # fog sy
  attr_accessor :battleback_name          # battleback file name
  attr_accessor :display_x                # display x-coordinate * 128
  attr_accessor :display_y                # display y-coordinate * 128
  attr_accessor :need_refresh             # refresh request flag
  attr_reader   :passages                 # passage table
  attr_reader   :priorities               # prioroty table
  attr_reader   :terrain_tags             # terrain tag table
  attr_reader   :events                   # events
  attr_reader   :fog_ox                   # fog x-coordinate starting point
  attr_reader   :fog_oy                   # fog y-coordinate starting point
  attr_reader   :fog_tone                 # fog color tone
  #--------------------------------------------------------------------------
  # * Object Initialization
  #--------------------------------------------------------------------------
  def initialize
    @map_id = 0
    @display_x = 0
    @display_y = 0
  end
  #--------------------------------------------------------------------------
  # * Setup
  #     map_id : map ID
  #--------------------------------------------------------------------------
  def setup(map_id)
    # Put map ID in @map_id memory
    @map_id = map_id
    # Load map from file and set @map
    @map = load_data(sprintf("Map%03d.rxdata", @map_id))
    
    # set tile set information in opening instance variables
    $data_tilesets      = load_data("Tilesets.rxdata")
    tileset = $data_tilesets[@map.tileset_id]
    @tileset_name = tileset.tileset_name
    @autotile_names = tileset.autotile_names
    @passages = tileset.passages
    @priorities = tileset.priorities
    @terrain_tags = tileset.terrain_tags
    # Initialize displayed coordinates
    @display_x = 0
    @display_y = 0
    # Clear refresh request flag
    @need_refresh = false
    # Set map event data
    @events = {}
    for i in @map.events.keys
      @events[i] = @map.events[i]
    end
    # Set common event data
  end
  #--------------------------------------------------------------------------
  # * Get Map ID
  #--------------------------------------------------------------------------
  def map_id
    return @map_id
  end
  def valid?(x, y)
    return (x >= 0 and x < width and y >= 0 and y < height)
  end
  #--------------------------------------------------------------------------
  # * Get Width
  #--------------------------------------------------------------------------
  def width
    return @map.width
  end
  #--------------------------------------------------------------------------
  # * Get Height
  #--------------------------------------------------------------------------
  def height
    return @map.height
  end

  def passable?(x, y, d, self_event = nil)
    # If coordinates given are outside of the map
    unless valid?(x, y)
      # impassable
	  puts "not valid"
      return false
	  
    end
    # Change direction (0,2,4,6,8,10) to obstacle bit (0,1,2,4,8,0)
    bit = (1 << (d / 2 - 1)) & 0x0f
    # Loop in all events
    # Loop searches in order from top of layer
    for i in [2, 1, 0]
      # Get tile ID
      tile_id = @map.data[x, y, i]
      # Tile ID acquistion failure
      if tile_id == nil
		puts "aie n'existe pas"
        # impassable
        return false
      # If obstacle bit is set
      elsif @passages[tile_id] & bit != 0
		puts "passage unidirectionnel"
        # impassable
        return false
      # If obstacle bit is set in all directions
      elsif @passages[tile_id] & 0x0f == 0x0f
        # impassable
		puts "impassable"
        return false
      # If priorities other than that are 0
      elsif @priorities[tile_id] == 0
        # passable
		puts "passable"
        return true
      end
    end
    # passable
    return true
  end
  #--------------------------------------------------------------------------
  # * Determine Thicket
  #     x          : x-coordinate
  #     y          : y-coordinate
  #--------------------------------------------------------------------------
  def bush?(x, y)
    if @map_id != 0
      for i in [2, 1, 0]
        tile_id = data[x, y, i]
        if tile_id == nil
          return false
        elsif @passages[tile_id] & 0x40 == 0x40
          return true
        end
      end
    end
    return false
  end
  #--------------------------------------------------------------------------
  # * Determine Counter
  #     x          : x-coordinate
  #     y          : y-coordinate
  #--------------------------------------------------------------------------
  def counter?(x, y)
    if @map_id != 0
      for i in [2, 1, 0]
        tile_id = data[x, y, i]
        if tile_id == nil
          return false
        elsif @passages[tile_id] & 0x80 == 0x80
          return true
        end
      end
    end
    return false
  end
  #--------------------------------------------------------------------------
  # * Get Terrain Tag
  #     x          : x-coordinate
  #     y          : y-coordinate
  #--------------------------------------------------------------------------
  def terrain_tag(x, y)
    if @map_id != 0
      for i in [2, 1, 0]
        tile_id = data[x, y, i]
        if tile_id == nil
          return 0
        elsif @terrain_tags[tile_id] > 0
          return @terrain_tags[tile_id]
        end
      end
    end
    return 0
  end
  #--------------------------------------------------------------------------
  # * Get Designated Position Event ID
  #     x          : x-coordinate
  #     y          : y-coordinate
  #--------------------------------------------------------------------------
  def check_event(x, y)
    for event in $game_map.events.values
      if event.x == x and event.y == y
        return event.id
      end
    end
  end
  #--------------------------------------------------------------------------
  # * Start Scroll
  #     direction : scroll direction
  #     distance  : scroll distance
  #     speed     : scroll speed
  #--------------------------------------------------------------------------

  #--------------------------------------------------------------------------
  # * Determine if Scrolling
  #--------------------------------------------------------------------------

end
module RPG
  class Tileset
    def initialize
      @id = 0
      @name = ""
      @tileset_name = ""
      @autotile_names = [""]*7
      @panorama_name = ""
      @panorama_hue = 0
      @fog_name = ""
      @fog_hue = 0
      @fog_opacity = 64
      @fog_blend_type = 0
      @fog_zoom = 200
      @fog_sx = 0
      @fog_sy = 0
      @battleback_name = ""
      @passages = Table.new(384)
      @priorities = Table.new(384)
      @priorities[0] = 5
      @terrain_tags = Table.new(384)
    end
    attr_accessor :id
    attr_accessor :name
    attr_accessor :tileset_name
    attr_accessor :autotile_names
    attr_accessor :panorama_name
    attr_accessor :panorama_hue
    attr_accessor :fog_name
    attr_accessor :fog_hue
    attr_accessor :fog_opacity
    attr_accessor :fog_blend_type
    attr_accessor :fog_zoom
    attr_accessor :fog_sx
    attr_accessor :fog_sy
    attr_accessor :battleback_name
    attr_accessor :passages
    attr_accessor :priorities
    attr_accessor :terrain_tags
  end
end
def tiles_id(map)
  @map = load_data("#{map}")
  #Création des Arrays contenant les datas de la map
  tiles_id_0 = []
  tiles_id_1 = []
  tiles_id_2 = []
  # Création des incrémenteurs
  l = 0
  m = 0
  n = 0
  # Boucle pour déterminer le tile utilisé
  for y in 0...@map.height
    for x in 0...@map.width
      for i in [2, 1, 0]

      # Get tile ID
      case i
      when 0 # Layer 0
        tiles_id_0[l] = @map.data[x, y, i]
        if tiles_id_0[l] == nil
          tiles_id_0[l] = 0
        end
        tile_x = (tiles_id_0[l] - 384) % 8 # Index sur la ligne (colonne)
        tile_y = (tiles_id_0[l] - 384) / 8 # index de la ligne
        if tile_y < 0
          tile_y = 99999
        end
        tiles_id_0[l] = [tile_x,tile_y]

        l+=1
      when 1 # Layer 1
        tiles_id_1[m] = @map.data[x, y, i]
        if tiles_id_1[m] == nil
          tiles_id_1[m] = 99999 # Sans tiles
        end
        tile_x = (tiles_id_1[m] - 384) % 8
        tile_y = (tiles_id_1[m] - 384) / 8
        if tile_y < 0
          tile_y = 99999
        end
        tiles_id_1[m] = [tile_x,tile_y]
        m+=1
      when 2 # layer 2
        tiles_id_2[n] = @map.data[x, y, i]
        if tiles_id_2[n] == nil
          tiles_id_2[n] = 99999
        end
        tile_x = (tiles_id_2[n] - 384) % 8
        tile_y = (tiles_id_2[n] - 384) / 8
        if tile_y < 0
          tile_y = 99999
        end
        tiles_id_2[n] = [tile_x,tile_y]
		puts tiles_id_2[n]
        n+=1
      end
      end
    end
  end
  puts tiles_id_2
  return tiles_id_0, tiles_id_1, tiles_id_2
end
def load_data(filename)
  File.open(filename, "rb") { |f|
    obj = Marshal.load(f)
  }
end

def id(line_n,index_l, line_s = 8)
  return line_n * line_s + index_l if line_n < 99999
  return 99999
end

def fichier_umd(map_name) #Création du fichier umd
  map_id = Array.new
  player_x = Array.new
  player_y = Array.new
  player_bex = Array.new
  player_bey = []
  i = 0
  #Récupération des téléportations
  u = Game_Map.new
  u.setup(map_name)
  for j in u.map.events.keys
    for page in u.map.events[j].pages.reverse
      for liste in page.list
        if liste.code == 201
          player_bex[i] = u.map.events[j].x
          player_bey[i] = u.map.events[j].y
          map_id[i] = liste.parameters[1]
          player_x[i] = liste.parameters[2]
          player_y[i] = liste.parameters[3]
          i += 1
        end
      end
    end
  end
  i = 0
  passages = Array.new #Gestion des colliders
  for y in 0...u.height
    for x in 0...u.width
      k  = u.passable?(x,y,10)
	  puts "#{k} vaut false" if k == false
      passages[i] = k
      i += 1
    end
  end
  fy,gy,hy = tiles_id(sprintf("Map%03d.rxdata",map_name)) #obtention des coordonnées des tiles sur le tileset
  if not File.exists?("0.umd")
    files = File.new("0.umd","w")
	files.puts("{")
    if map_id[0] != nil
      files.puts("\"tp\":[")
      j = 0
      while j < map_id.size
        mapload = load_data(sprintf("Map%03d.rxdata", map_id[j]))
        files.puts("{\"tomap\":#{map_id[j]},\"oncase\":#{player_bex[j] + player_bey[j] * u.map.width},\"tocase\": #{player_x[j] + player_y[j] * mapload.width}},") if (j + 1) < map_id.size
		files.puts("{\"tomap\":#{map_id[j]},\"oncase\":#{player_bex[j] + player_bey[j] * u.map.width},\"tocase\": #{player_x[j] + player_y[j] * mapload.width}}") if (j + 1) >= map_id.size
        j +=1
		
      end
      
      files.puts("],")
    end
    files.puts("\"map3\":[")
    i = 0
    for t in fy #layer 1
      ids = id(t[1],t[0])
      files.puts("{\"colliding\":#{!passages[i]},\"id\": #{ids}},") if i < (fy.size - 1)
	  files.puts("{\"colliding\":#{!passages[i]},\"id\": #{ids}}") if i >= (fy.size - 1)
	  puts "#{i} met true" if !passages[i] == true
      i += 1

    end
    files.puts("],\n")
    files.puts("\"width\":#{u.width},")
    files.puts("\"height\":#{u.height},")
    files.puts("\"map2\":[")
    i = 0
    for t in gy #layer 2
      ids = id(t[1],t[0])
      files.puts("{\"colliding\":#{!passages[i]},\"id\": #{ids}},") if i < (gy.size - 1)
	  files.puts("{\"colliding\":#{!passages[i]},\"id\": #{ids}}") if i >= (gy.size - 1)
      i += 1
    end
    files.puts("],\"map\":[")
    i = 0

	for t in hy #layer 3
      ids = id(t[1],t[0])
	  puts "ids : #{ids}"
      files.puts("{\"colliding\":#{!passages[i]},\"id\": #{ids}},") if i < (hy.size - 1)
	  files.puts("{\"colliding\":#{!passages[i]},\"id\": #{ids}}") if i >= (hy.size - 1)
      i += 1

    end
    files.puts("]}")
  end

end
puts "Quel fichier voulez-vous convertir (en numéro plz les 001 ne sont pas acceptés) ?"
u = gets.chomp.to_i
puts "Vous avez choisi la Map00#{u}.rxdata"
fichier_umd(u)
puts " Appuyez pour fermer..."
gets
