require "scripts/keyboard"
require "scripts/math"
require "scripts/spriter/tpsloader"
require "scripts/spriter/spriter"
require "scripts/spriter/utils"

screenWidth = MOAIEnvironment.screenWidth
screenHeight = MOAIEnvironment.screenHeight

if screenWidth == nil then 
	screenWidth = 640 
end

if screenHeight == nil then 
	screenHeight = 480 
end

screenMinX = -(screenWidth / 2)
screenMaxX = screenWidth / 2
screenMinY = -(screenHeight / 2)
screenMaxY = screenHeight / 2


MOAISim.openWindow("Neko Punch", screenWidth, screenHeight)

viewport = MOAIViewport.new()
viewport:setSize(screenWidth, screenHeight)
viewport:setScale(screenWidth, screenHeight)

layer = MOAILayer.new ()
layer:setViewport( viewport )
layer:setClearColor( 1, 0.41, 0.70, 1 )

-- depreciated function call ... what should I do instead?
MOAIRenderMgr.pushRenderPass ( layer )

gfxQuads, names = tpsloader ( 'resources/boy/boy_sheet.lua', 'resources/boy/boy_sheet.png' )
sprite = spriter("resources/boy/boy.lua", gfxQuads, names)

anim = sprite:createAnim ( "standing" )
anim:insertProps ( layer )
anim:start ()
anim.root:addLoc(0, -200)

mainThread = MOAIThread.new ()
mainThread:run ( 

		function ()

			while not gameOver do

				coroutine.yield ()

				local x, y = anim.root:getLoc()
				
				
				if isKeyDown(KEY_UP) == true then											
					if y + 68 < screenMaxY then
						anim.root:addLoc(0, 10)
					end
				end
				if isKeyDown(KEY_DOWN) == true then
					if y - 68 > screenMinY then
						anim.root:addLoc(0, -10)
					end
				end

				-- always do last to avoid loosing events (just the way moai processes events)
				updateKeyboard()
			end
		end
	)
