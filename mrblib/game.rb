module MRGSS
  
  class Game
  
    attr_accessor :viewport
     
    def initialize(title, width, height)
      @screen = Screen.new(title, width, height)      
      @viewport  = @screen.viewport
      @running = false
    end
    
    def start
      @screen.show
      @running = true
    end
    
    def update
      return @running = false if @screen.disposed?
      Keyboard.update
      Mouse.update
      @screen.update
    end
    
    def pause
      
    end
    
    def close
      
    end
    
    def run
      start
      while @running
        update 
      end
    end
    
  end
  
end 