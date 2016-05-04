require "moai_spriter_plugin/tpsloader"
require "moai_spriter_plugin/spriter"
require "moai_spriter_plugin/utils"

screenWidth = MOAIEnvironment.screenWidth
screenHeight = MOAIEnvironment.screenHeight

if screenWidth == nil then 
	screenWidth = 1280 
end

if screenHeight == nil then 
	screenHeight = 720 
end

screenMinX = -(screenWidth / 2)
screenMaxX = screenWidth / 2
screenMinY = -(screenHeight / 2)
screenMaxY = screenHeight / 2


MOAISim.openWindow("Moai Spriter Demo", screenWidth, screenHeight)

viewport = MOAIViewport.new()
viewport:setSize(screenWidth, screenHeight)
viewport:setScale(screenWidth, screenHeight)

layer = MOAILayer.new ()
layer:setViewport( viewport )
layer:setClearColor( 1, 1, 1, 1 )

-- depreciated function call ... what should I do instead?
MOAIRenderMgr.pushRenderPass ( layer )

gfxQuads, names = tpsloader ( 'resources/scorpion/scorpion_pieces.lua', 'resources/scorpion/scorpion_pieces.png' )
sprite = spriter("resources/scorpion/scorpion.lua", gfxQuads, names)

anim = sprite:createAnim ( "Walk" )
anim:insertProps ( layer )
anim:start ()
anim.root:addLoc(0, -200)
--anim.root:setScl(.5, .5)
