function draw()
    cls()
    
    local t = time()
    local center_x = 64
    local center_y = 64
    local radius = 30
    
    -- Slow rotation for better visibility
    local angle = t * 0.1 -- very slow
    
    -- Main rotating point
    local x = center_x + cos(angle) * radius
    local y = center_y + sin(angle) * radius
    
    putp(x, y, 8) -- red point
    
    -- Draw center
    putp(center_x, center_y, 7) -- white point in center
    
    -- Draw circle for reference
    for i = 0, 15 do
        local ref_angle = i / 16
        local ref_x = center_x + cos(ref_angle) * radius
        local ref_y = center_y + sin(ref_angle) * radius
        putp(ref_x, ref_y, 5) -- gray circle points
    end
    
    -- Show direction: arrow from center to point
    local steps = 10
    for i = 1, steps do
        local progress = i / steps
        local arrow_x = center_x + cos(angle) * radius * progress
        local arrow_y = center_y + sin(angle) * radius * progress
        putp(arrow_x, arrow_y, 12) -- blue arrow
    end
    
    -- Mark initial position (angle = 0)
    local start_x = center_x + cos(0) * radius
    local start_y = center_y + sin(0) * radius
    putp(start_x, start_y, 11) -- green start mark
end