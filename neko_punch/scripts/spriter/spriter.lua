-- 
--  spriter.lua
--  sprinter-moai
--  
--  Created by Jon Baker on 2012-11-09.
--  Distributed under MPL-2.0 licence (http://opensource.org/licenses/MPL-2.0)
-- 

local texture
local curves = {}

local function insertProps ( self, layer )
    for i, v in ipairs ( self.props ) do
        layer:insertProp ( v )
    end
end

-- This convenience function is added here for anyone using the 
-- RapaNui framework in combination with Moai SDK
local function insertPropsRN ( self , highestPriority )
  self.rnprops = {}
    for i, v in ipairs ( self.props ) do
    o = RNObject.new()
    o.prop = v
    o.prop.name = v.name
    o.prop:setIndex(1)
    if highestPriority ~= nil then
      highestPriority = highestPriority + 1
      o.prop:setPriority(highestPriority)
    end
    o:setLocatingMode(CENTERED_MODE)
    o.isAnim = true
    local parentGroup = RNFactory.mainGroup

    RNFactory.screen:addRNObject(o)
    table.insert ( self.rnprops, i, o )
    end
  return highestPriority
end

local function removeProps ( self, layer )

    for i, v in ipairs ( self.props ) do
        layer:removeProp ( v )
    end
end

function comparePropPriorities(a,b)
  if b == nil or b:getPriority() == nil then
    return false
  elseif a == nil or a:getPriority() == nil then
    return true
  else 
    return a:getPriority() < b:getPriority()
  end
end

local function createAnim ( self, name, x, y, scaleX, scaleY, reverseFlag )
    local layerSize = 9;

    local player = MOAIAnim.new ()
    player:reserveLinks ( (#self.curves[name] * layerSize) )
    
    local root = MOAITransform.new ()
    local props = {}
    
    for i, curveSet in orderedPairs ( self.curves[name] ) do
        local prop = MOAIProp2D.new ()
        prop:setParent ( root )
        prop:setDeck ( self.texture )
        prop:setPriority( curveSet.priority )
        prop:setBlendMode( MOAIProp.GL_SRC_ALPHA, MOAIProp.GL_ONE_MINUS_SRC_ALPHA )
        prop.name = curveSet.id.name
        self.scaleX = 1
        self.scaleY = 1
            
        local c = ( i - 1 ) * layerSize
        player:setLink ( c + 1, curveSet.id, prop, MOAIProp2D.ATTR_INDEX )
        player:setLink ( c + 2, curveSet.x, prop, MOAITransform.ATTR_X_LOC )
        player:setLink ( c + 3, curveSet.y, prop, MOAITransform.ATTR_Y_LOC )
        player:setLink ( c + 4, curveSet.r, prop, MOAITransform.ATTR_Z_ROT )
        player:setLink ( c + 5, curveSet.xs, prop, MOAITransform.ATTR_X_SCL )
        player:setLink ( c + 6, curveSet.ys, prop, MOAITransform.ATTR_Y_SCL )
        player:setLink ( c + 7, curveSet.px, prop, MOAITransform.ATTR_X_PIV )
        player:setLink ( c + 8, curveSet.py, prop, MOAITransform.ATTR_Y_PIV )
        player:setLink ( c + 9, curveSet.a, prop, MOAIColor.ATTR_A_COL )
        player:setCurve(curveSet.id)
        table.insert ( props, i, prop )
    end
    table.sort(props, comparePropPriorities)
    if reverseFlag then
        player:setMode(MOAITimer.LOOP_REVERSE)
    else
        player:setMode(MOAITimer.LOOP)
    end

    if scaleX and scaleY then
        root:setScl(scaleX, scaleY)
    end
    if x and y then
        root:setLoc(x, y)
    end
    player.root = root
    player.props = props
    
    player.insertProps = insertProps
    player.insertPropsRN = insertPropsRN
    player.removeProps = removeProps
    
    player:apply ( 0 )
    
    return player
end

function spriter(filename, deck, names)
    local anims = dofile ( filename )
    curves = {}
    texture = deck 
    for anim, objects in orderedPairs ( anims ) do
        --print("\n\nAdding animation " .. anim .. "\n\n")
        
        local animCurves = {}
        for i, object in orderedPairs ( objects ) do
            local numKeys = #object

            -- Texture ID
            local idCurve = MOAIAnimCurve.new ()
            idCurve:reserveKeys ( numKeys )

            -- Location
            local xCurve = MOAIAnimCurve.new ()
            xCurve:reserveKeys ( numKeys )

            local yCurve = MOAIAnimCurve.new ()
            yCurve:reserveKeys ( numKeys )
      
            -- Z-Index
            --local zCurve = MOAIAnimCurve.new ()
            --zCurve:reserveKeys ( numKeys )  

            -- Rotation
            local rCurve = MOAIAnimCurve.new ()
            rCurve:reserveKeys ( numKeys )

            -- Scale
            local sxCurve = MOAIAnimCurve.new ()
            sxCurve:reserveKeys ( numKeys )

            local syCurve = MOAIAnimCurve.new ()
            syCurve:reserveKeys ( numKeys )
      
            -- Alpha
            local aCurve = MOAIAnimCurve.new ()
            aCurve:reserveKeys ( numKeys )

            -- Pivot
            local pxCurve = MOAIAnimCurve.new ()
            pxCurve:reserveKeys ( numKeys )

            local pyCurve = MOAIAnimCurve.new ()
            pyCurve:reserveKeys ( numKeys )
      
            local prevTexture = nil
            local prevFrame = nil
            for ii, frame in orderedPairs ( object ) do
                if frame.texture then
                    time = frame.time / 1000
                    idCurve:setKey ( ii, time, names[frame.texture], MOAIEaseType.FLAT)
                    idCurve.name = frame.texture
                    xCurve:setKey  ( ii, time, frame.x, MOAIEaseType.LINEAR)
                    yCurve:setKey  ( ii, time, frame.y, MOAIEaseType.LINEAR)
          
                    frame.angleWithSpin = frame.angle
                    if prevFrame ~= nil then
                        if frame.angle < prevFrame.angle and prevFrame.spin == 1 then
                          frame.angleWithSpin = frame.angle + 360
                        elseif frame.angle > prevFrame.angle and prevFrame.spin == -1 then
                          frame.angleWithSpin = frame.angle - 360
                        end
                        
                        if prevFrame.angleWithSpin >= 360 and prevFrame.angle < 360 then
                          local numRotations = math.floor(math.abs(prevFrame.angleWithSpin / 360))
                          if numRotations == 0 then
                            numRotations = 1
                          end
                          frame.angleWithSpin = frame.angleWithSpin + (360 * numRotations)
                        elseif prevFrame.angleWithSpin <= 0 and prevFrame.angle > 0 then
                          local numRotations = math.floor(math.abs(prevFrame.angleWithSpin / 360)) + 1
                          frame.angleWithSpin = frame.angleWithSpin - (360 * numRotations)
                        end
                    end
                    if frame.alpha == nil then
                        frame.alpha = 1
                    end

                    rCurve:setKey  ( ii, time, frame.angleWithSpin, MOAIEaseType.LINEAR)                    
                    sxCurve:setKey ( ii, time, frame.scale_x, MOAIEaseType.LINEAR)
                    syCurve:setKey ( ii, time, frame.scale_y, MOAIEaseType.LINEAR)
                    aCurve:setKey ( ii, time, frame.alpha, MOAIEaseType.LINEAR)
                    pxCurve:setKey ( ii, time, frame.pivot_x, MOAIEaseType.LINEAR )
                    pyCurve:setKey ( ii, time, frame.pivot_y, MOAIEaseType.LINEAR )
                    prevTexture = frame.texture
                    prevFrame = frame
                end
            end
    
            local curveSet = {}

            curveSet.id = idCurve
            curveSet.x = xCurve
            curveSet.y = yCurve
            curveSet.r = rCurve
            curveSet.xs = sxCurve
            curveSet.ys = syCurve
            curveSet.a = aCurve
            curveSet.px = pxCurve
            curveSet.py = pyCurve
            curveSet.priority = object[1].zindex
            table.insert ( animCurves, i, curveSet )            
        end
        curves[anim] = animCurves
    end
    local sprite = {}
    sprite.curves = curves
    sprite.texture = texture
    sprite.createAnim = createAnim

    return sprite
end