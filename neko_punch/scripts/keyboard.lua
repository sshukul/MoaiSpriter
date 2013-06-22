
KEY_STATE_NONE = 0
KEY_STATE_PRESS = 1
KEY_STATE_HELD = 2
KEY_STATE_RELEASE = 3


KEY_UP = 0
KEY_DOWN = 1
KEY_LEFT = 2
KEY_RIGHT = 3

KEY_MAX = 4

keyPressEvents = {}


-- key codes sent from the host, may be subject to change

KEYCODE_UP = 0xF700
KEYCODE_DOWN = 0xF701
KEYCODE_LEFT = 0xF702
KEYCODE_RIGHT = 0xF703

KEYCODE_MIN = KEYCODE_UP
KEYCODE_MAX = KEYCODE_RIGHT


if(MOAIInputMgr.device.keyboard) then

	for i=0, KEY_MAX do
		keyPressEvents[i] = KEY_STATE_NONE
	end

  	MOAIInputMgr.device.keyboard:setCallback(
		function(key,down)
			if down == true then
				if key >= KEYCODE_MIN and key <= KEYCODE_MAX then
					if keyPressEvents[key - KEYCODE_MIN] == KEY_STATE_NONE then
						keyPressEvents[key - KEYCODE_MIN] = KEY_STATE_PRESS
					end
				end        
			else
				if key >= KEYCODE_MIN and key <= KEYCODE_MAX then
					if keyPressEvents[key - KEYCODE_MIN] == KEY_STATE_HELD or keyPressEvents[key - KEYCODE_MIN] == KEY_STATE_PRESS then
						keyPressEvents[key - KEYCODE_MIN] = KEY_STATE_RELEASE
					end
				end              	
			end
		end
	)
end

function updateKeyboard()
	for i=0, KEY_MAX do		
		if keyPressEvents[i] == KEY_STATE_PRESS then
			keyPressEvents[i] = KEY_STATE_HELD
		elseif keyPressEvents[i] == KEY_STATE_RELEASE then
			keyPressEvents[i] = KEY_STATE_NONE
		end
	end
end

function isKeyDown(key)
	if key < 0 or key >= KEY_MAX then
		return false
	end
	return keyPressEvents[key] == KEY_STATE_PRESS or keyPressEvents[key] == KEY_STATE_HELD
end

function isKeyUp(key)
	if key < 0 or key >= KEY_MAX then
		return false
	end
	return keyPressEvents[key] == KEY_STATE_NONE or keyPressEvents[key] == KEY_STATE_RELEASE
end

function isKeyPressed(key)
	if key < 0 or key >= KEY_MAX then
		return false
	end
	return keyPressEvents[key] == KEY_STATE_PRESS
end

function isKeyHeld(key)
	if key < 0 or key >= KEY_MAX then
		return false
	end
	return keyPressEvents[key] == KEY_STATE_HELD
end

function isKeyReleased(key)
	if key < 0 or key >= KEY_MAX then
		return false
	end
	return keyPressEvents[key] == KEY_STATE_RELEASE
end





