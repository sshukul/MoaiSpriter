
function angle ( x1, y1, x2, y2 )
	return math.atan2 ( y2 - y1, x2 - x1 ) * ( 180 / math.pi )
end

function distance ( x1, y1, x2, y2 )
	return math.sqrt ((( x2 - x1 ) ^ 2 ) + (( y2 - y1 ) ^ 2 ))
end

-- tests intersection between to AABBs
-- rather than try to test when rects intersect, we test the cases where they
-- dont intersect. If these tests fail, then the rects must intersect
function AABBIntersect ( r1x1, r1y1, r1x2, r1y2, r2x1, r2y1, r2x2, r2y2 )
	if r1x2 < r2x1 then
		return false
	elseif r1y2 < r2y1 then
		return false
	elseif r1x1 > r2x2 then
		return false
	elseif r1y1 > r2y2 then
		return false
	end
		
    return true
end