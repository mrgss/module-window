module MRGSS
  
  class Game
  
    def initialize(title, width, height)
      @screen = Screen.new(title, width, height)      
      @viewport  = @screen.viewport
    end
    
    def start
      @screen.show
    end
    
    def update
      return if @screen.disposed?
      @screen.update
      Keyboard.update
      Mouse.update
    end
    
    def pause
      
    end
    
    def close
      #@screen.dispose
    end
    
    def run
      start
      update while not  @screen.disposed?
    end
    
  end
  
end 